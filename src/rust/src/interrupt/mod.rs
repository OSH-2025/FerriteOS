use crate::{
    ffi::bindings::{arch_int_lock, arch_int_restore, arch_int_unlock, arch_irq_init},
    result::{SystemError, SystemResult},
};
use core::ffi::c_char;
use error::InterruptError;
use global::{
    get_interrupt_controller, irq_nesting_count_dec, irq_nesting_count_get, irq_nesting_count_inc,
};
use types::{InterruptHandler, InterruptHandlerFn};

pub mod error;
pub mod global;
pub mod types;

#[inline]
pub fn disable_interrupts() -> u32 {
    arch_int_lock()
}

#[inline]
pub fn enable_interrupts() -> u32 {
    arch_int_unlock()
}

#[inline]
pub fn restore_interrupt_state(int_save: u32) {
    arch_int_restore(int_save);
}

/// 检查当前是否处于中断上下文
#[inline]
pub fn is_int_active() -> bool {
    get_interrupt_nesting_count() != 0
}

/// 当前CPU的中断嵌套计数
pub fn get_interrupt_nesting_count() -> u32 {
    let int_save = disable_interrupts();
    let count = irq_nesting_count_get();
    restore_interrupt_state(int_save);
    count
}

/// 删除硬件中断处理程序（内部函数）
fn unregister_interrupt_handler(hwi_form: &mut InterruptHandler, irq_id: u32) -> SystemResult<()> {
    let int_save = disable_interrupts();

    // 清除处理函数和响应计数
    hwi_form.reset();

    // 检查并调用禁用中断函数
    let result = if let Some(controller) = get_interrupt_controller() {
        controller.disable_irq_with_check(irq_id)
    } else {
        Err(SystemError::Interrupt(InterruptError::ProcFuncNull))
    };

    restore_interrupt_state(int_save);
    result
}

/// 创建硬件中断处理程序（内部函数）
fn register_interrupt_handler(
    hwi_form: &mut InterruptHandler,
    hwi_handler: InterruptHandlerFn,
) -> SystemResult<()> {
    let int_save = disable_interrupts();

    let result = if !hwi_form.is_registered() {
        hwi_form.hook = hwi_handler;
        Ok(())
    } else {
        Err(SystemError::Interrupt(InterruptError::AlreadyCreated))
    };

    restore_interrupt_state(int_save);
    result
}

/// 创建硬件中断处理程序
pub fn register_interrupt(
    hwi_num: u32,
    hwi_prio: u8,
    hwi_handler: InterruptHandlerFn,
) -> SystemResult<()> {
    // 检查处理函数是否为空
    if hwi_handler.is_none() {
        return Err(SystemError::Interrupt(InterruptError::ProcFuncNull));
    }

    // 获取中断控制器
    let controller =
        get_interrupt_controller().ok_or(SystemError::Interrupt(InterruptError::ProcFuncNull))?;

    // 获取中断处理信息
    let hwi_form = controller.get_handle_form_with_check(hwi_num)?;

    // 创建中断处理程序
    register_interrupt_handler(hwi_form, hwi_handler)?;

    // 设置中断优先级（如果支持）
    match controller.set_irq_priority_with_check(hwi_num, hwi_prio) {
        Ok(()) => Ok(()),
        Err(err) => {
            // 如果设置优先级失败，清理已创建的中断
            let _ = unregister_interrupt_handler(hwi_form, hwi_num);
            return Err(err);
        }
    }
}

/// 删除硬件中断处理程序
pub fn unregister_interrupt(hwi_num: u32) -> SystemResult<()> {
    // 获取中断控制器
    let controller =
        get_interrupt_controller().ok_or(SystemError::Interrupt(InterruptError::ProcFuncNull))?;

    // 获取中断处理信息
    let hwi_form = controller.get_handle_form_with_check(hwi_num)?;
    // 删除中断处理程序
    unregister_interrupt_handler(hwi_form, hwi_num)
}

/// 触发硬件中断
pub fn trigger_interrupt(hwi_num: u32) -> SystemResult<()> {
    let controller =
        get_interrupt_controller().ok_or(SystemError::Interrupt(InterruptError::ProcFuncNull))?;

    controller.trigger_irq_with_check(hwi_num)
}

/// 使能硬件中断
pub fn enable_interrupt(hwi_num: u32) -> SystemResult<()> {
    let controller =
        get_interrupt_controller().ok_or(SystemError::Interrupt(InterruptError::ProcFuncNull))?;

    controller.enable_irq_with_check(hwi_num)
}

/// 禁用硬件中断
pub fn disable_interrupt(hwi_num: u32) -> SystemResult<()> {
    let controller =
        get_interrupt_controller().ok_or(SystemError::Interrupt(InterruptError::ProcFuncNull))?;

    controller.disable_irq_with_check(hwi_num)
}

/// 清除硬件中断
pub fn clear_interrupt(hwi_num: u32) -> SystemResult<()> {
    let controller =
        get_interrupt_controller().ok_or(SystemError::Interrupt(InterruptError::ProcFuncNull))?;

    controller.clear_irq_with_check(hwi_num)
}

/// 设置硬件中断优先级
pub fn set_interrupt_priority(hwi_num: u32, priority: u8) -> SystemResult<()> {
    let controller =
        get_interrupt_controller().ok_or(SystemError::Interrupt(InterruptError::ProcFuncNull))?;

    controller.set_irq_priority_with_check(hwi_num, priority)
}

/// 中断处理
pub fn handle_interrupt(_hwi_num: u32, hwi_form: &mut InterruptHandler) {
    // 增加中断嵌套计数
    irq_nesting_count_inc();

    #[cfg(feature = "debug_sched_statistics")]
    OsHwiStatistics(_hwi_num);

    // 增加响应计数
    hwi_form.increment_count();

    // 调用用户注册的中断处理函数
    if let Some(handler) = hwi_form.hook {
        handler();
    }

    // 减少中断嵌套计数
    irq_nesting_count_dec();
}

pub fn interrupt_entry() {
    if let Some(controller) = get_interrupt_controller() {
        controller.handle_irq_with_check();
    }
}

/// 硬件中断初始化
pub fn initialize_interrupt() {
    arch_irq_init();
}

/// 获取中断处理信息
///
/// # Arguments
/// * `hwi_num` - 硬件中断号
///
/// # Returns
/// * `Some(&HwiHandleInfo)` - 中断处理信息
/// * `None` - 无效的中断号或控制器未注册
pub fn get_interrupt_handler(hwi_num: u32) -> SystemResult<&'static mut InterruptHandler> {
    let controller =
        get_interrupt_controller().ok_or(SystemError::Interrupt(InterruptError::ProcFuncNull))?;
    controller.get_handle_form_with_check(hwi_num)
}

/// 获取中断响应计数
///
/// # Arguments
/// * `hwi_num` - 硬件中断号
///
/// # Returns
/// * `Ok(count)` - 响应计数
/// * `Err(SystemError)` - 无效的中断号
pub fn get_interrupt_count(hwi_num: u32) -> SystemResult<u32> {
    match get_interrupt_handler(hwi_num) {
        Ok(hwi_info) => Ok(hwi_info.resp_count),
        Err(err) => Err(err),
    }
}

/// 获取当前中断号
///
/// # Returns
/// * `Ok(irq_num)` - 当前中断号
/// * `Err(SystemError)` - 控制器未注册或函数未实现
pub fn get_current_interrupt_number() -> SystemResult<u32> {
    let controller =
        get_interrupt_controller().ok_or(SystemError::Interrupt(InterruptError::ProcFuncNull))?;

    controller.get_cur_irq_num_with_check()
}

/// 检查中断是否已注册
///
/// # Arguments
/// * `num` - 中断号
///
/// # Returns
/// * `true` - 中断已注册
/// * `false` - 中断未注册
pub fn is_interrupt_registered(num: u32) -> bool {
    if let Ok(hwi_info) = get_interrupt_handler(num) {
        hwi_info.hook.is_some()
    } else {
        false
    }
}

/// 获取中断版本信息
///
/// # Returns
/// * `Some(version)` - 版本字符串
/// * `None` - 控制器未注册或函数未实现
pub fn get_interrupt_version() -> SystemResult<*const c_char> {
    let controller =
        get_interrupt_controller().ok_or(SystemError::Interrupt(InterruptError::ProcFuncNull))?;

    controller.get_irq_version_with_check()
}
