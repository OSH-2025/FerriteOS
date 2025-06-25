use crate::interrupt::types::InterruptController;
use core::sync::atomic::{AtomicPtr, AtomicU32, Ordering};

/// 全局中断控制器操作接口（使用原子指针确保线程安全）
#[unsafe(export_name = "g_hwiOps")]
pub static INTERRUPT_CONTROLLER: AtomicPtr<InterruptController> =
    AtomicPtr::new(core::ptr::null_mut());

/// 注册中断控制器操作接口
#[inline]
pub fn register_interrupt_controller(ops: *mut InterruptController) {
    INTERRUPT_CONTROLLER.store(ops, Ordering::Release);
}

/// 获取全局中断控制器操作接口
#[inline]
pub fn get_interrupt_controller() -> Option<&'static InterruptController> {
    let ops_ptr = INTERRUPT_CONTROLLER.load(Ordering::Acquire);
    unsafe { ops_ptr.as_ref() }
}

#[unsafe(export_name = "g_intCount")]
pub static IRQ_NESTING_COUNTS: [AtomicU32; 1] = [AtomicU32::new(0)];

/// 获取当前CPU的中断嵌套计数
///
/// # Returns
/// 当前CPU的中断嵌套层数
#[inline]
pub fn irq_nesting_count_get() -> u32 {
    IRQ_NESTING_COUNTS[0].load(Ordering::Acquire)
}

/// 设置当前CPU的中断嵌套计数
///
/// # Arguments
/// * `val` - 要设置的计数值
#[inline]
pub fn irq_nesting_count_set(val: u32) {
    IRQ_NESTING_COUNTS[0].store(val, Ordering::Release);
}

/// 增加当前CPU的中断嵌套计数
///
/// # Returns
/// 增加后的计数值
#[inline]
pub fn irq_nesting_count_inc() {
    IRQ_NESTING_COUNTS[0].fetch_add(1, Ordering::AcqRel);
}

/// 减少当前CPU的中断嵌套计数
///
/// # Returns
/// 减少后的计数值
#[inline]
pub fn irq_nesting_count_dec() {
    IRQ_NESTING_COUNTS[0].fetch_sub(1, Ordering::AcqRel);
}
