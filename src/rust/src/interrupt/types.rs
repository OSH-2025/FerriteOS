use core::ffi::c_char;

use crate::error::{ErrorCode, InterruptError, SystemError, SystemResult};

pub type InterruptHandlerFn = Option<extern "C" fn()>;

/// 中断处理信息结构体
#[repr(C)]
#[derive(Debug)]
pub struct InterruptHandler {
    /// 用户注册的回调函数
    pub hook: InterruptHandlerFn,
    /// 中断响应计数
    pub resp_count: u32,
}

impl InterruptHandler {
    /// 创建新的中断处理信息
    pub const fn new() -> Self {
        Self {
            hook: None,
            resp_count: 0,
        }
    }

    /// 重置中断处理信息
    pub fn reset(&mut self) {
        self.hook = None;
        self.resp_count = 0;
    }

    /// 增加响应计数
    pub fn increment_count(&mut self) {
        self.resp_count = self.resp_count.saturating_add(1);
    }

    /// 检查是否已注册处理函数
    pub fn is_registered(&self) -> bool {
        self.hook.is_some()
    }
}

impl Default for InterruptHandler {
    fn default() -> Self {
        Self::new()
    }
}

/// 硬件中断控制器操作接口
#[repr(C)]
pub struct InterruptController {
    /// 触发中断
    pub trigger_irq: Option<extern "C" fn(hwi_num: u32) -> u32>,
    /// 清除中断
    pub clear_irq: Option<extern "C" fn(hwi_num: u32) -> u32>,
    /// 使能中断
    pub enable_irq: Option<extern "C" fn(hwi_num: u32) -> u32>,
    /// 禁用中断
    pub disable_irq: Option<extern "C" fn(hwi_num: u32) -> u32>,
    /// 设置中断优先级
    pub set_irq_priority: Option<extern "C" fn(hwi_num: u32, priority: u8) -> u32>,
    /// 获取当前中断号
    pub get_cur_irq_num: Option<extern "C" fn() -> u32>,
    /// 获取中断版本信息
    pub get_irq_version: Option<extern "C" fn() -> *const c_char>,
    /// 获取中断处理表单
    pub get_handle_form: Option<extern "C" fn(hwi_num: u32) -> *mut InterruptHandler>,
    /// 处理中断入口
    pub handle_irq: Option<extern "C" fn()>,
}

impl InterruptController {
    /// 安全触发中断
    pub fn trigger_irq_with_check(&self, hwi_num: u32) -> SystemResult<()> {
        match self.trigger_irq {
            Some(func) => ErrorCode(func(hwi_num)).into(),
            None => Err(SystemError::Interrupt(InterruptError::ProcFuncNull)),
        }
    }

    /// 安全清除中断
    pub fn clear_irq_with_check(&self, hwi_num: u32) -> SystemResult<()> {
        match self.clear_irq {
            Some(func) => ErrorCode(func(hwi_num)).into(),
            None => Err(SystemError::Interrupt(InterruptError::ProcFuncNull)),
        }
    }

    /// 安全使能中断
    pub fn enable_irq_with_check(&self, hwi_num: u32) -> SystemResult<()> {
        match self.enable_irq {
            Some(func) => ErrorCode(func(hwi_num)).into(),
            None => Err(SystemError::Interrupt(InterruptError::ProcFuncNull)),
        }
    }

    /// 安全禁用中断
    pub fn disable_irq_with_check(&self, hwi_num: u32) -> SystemResult<()> {
        match self.disable_irq {
            Some(func) => ErrorCode(func(hwi_num)).into(),
            None => Err(SystemError::Interrupt(InterruptError::ProcFuncNull)),
        }
    }

    /// 安全设置中断优先级
    pub fn set_irq_priority_with_check(&self, hwi_num: u32, priority: u8) -> SystemResult<()> {
        match self.set_irq_priority {
            Some(func) => ErrorCode(func(hwi_num, priority)).into(),
            None => Err(SystemError::Interrupt(InterruptError::ProcFuncNull)),
        }
    }

    /// 安全获取当前中断号
    pub fn get_cur_irq_num_with_check(&self) -> SystemResult<u32> {
        match self.get_cur_irq_num {
            Some(func) => Ok(func()),
            None => Err(SystemError::Interrupt(InterruptError::ProcFuncNull)),
        }
    }

    /// 安全获取中断版本
    pub fn get_irq_version_with_check(&self) -> SystemResult<*const c_char> {
        match self.get_irq_version {
            Some(func) => Ok(func()),
            None => Err(SystemError::Interrupt(InterruptError::ProcFuncNull)),
        }
    }

    /// 获取中断处理表单
    pub fn get_handle_form_with_check(&self, hwi_num: u32) -> SystemResult<&mut InterruptHandler> {
        match self.get_handle_form {
            Some(func) => match unsafe { func(hwi_num).as_mut() } {
                Some(handle_form) => Ok(handle_form),
                None => Err(SystemError::Interrupt(InterruptError::NumInvalid)),
            },
            None => Err(SystemError::Interrupt(InterruptError::ProcFuncNull)),
        }
    }

    /// 安全处理中断
    pub fn handle_irq_with_check(&self) {
        match self.handle_irq {
            Some(func) => {
                func();
            }
            None => {}
        }
    }
}
