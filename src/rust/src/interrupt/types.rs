use core::ffi::c_char;

use crate::error::{InterruptError, SystemError, SystemResult};

/// 中断处理信息结构体
#[repr(C)]
#[derive(Debug)]
pub struct InterruptHandler {
    /// 用户注册的回调函数
    pub hook: Option<extern "C" fn()>,
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
    #[allow(dead_code)]
    pub fn reset(&mut self) {
        self.hook = None;
        self.resp_count = 0;
    }

    /// 增加响应计数
    #[allow(dead_code)]
    pub fn increment_count(&mut self) {
        self.resp_count = self.resp_count.saturating_add(1);
    }

    /// 检查是否已注册处理函数
    #[allow(dead_code)]
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
    #[allow(dead_code)]
    pub fn safe_trigger_irq(&self, hwi_num: u32) -> SystemResult<u32> {
        match self.trigger_irq {
            Some(func) => Ok(func(hwi_num)),
            None => Err(SystemError::Interrupt(InterruptError::ProcFuncNull)),
        }
    }

    /// 安全清除中断
    #[allow(dead_code)]
    pub fn safe_clear_irq(&self, hwi_num: u32) -> SystemResult<u32> {
        match self.clear_irq {
            Some(func) => Ok(func(hwi_num)),
            None => Err(SystemError::Interrupt(InterruptError::ProcFuncNull)),
        }
    }

    /// 安全使能中断
    #[allow(dead_code)]
    pub fn safe_enable_irq(&self, hwi_num: u32) -> SystemResult<u32> {
        match self.enable_irq {
            Some(func) => Ok(func(hwi_num)),
            None => Err(SystemError::Interrupt(InterruptError::ProcFuncNull)),
        }
    }

    /// 安全禁用中断
    #[allow(dead_code)]
    pub fn safe_disable_irq(&self, hwi_num: u32) -> SystemResult<u32> {
        match self.disable_irq {
            Some(func) => Ok(func(hwi_num)),
            None => Err(SystemError::Interrupt(InterruptError::ProcFuncNull)),
        }
    }

    /// 安全设置中断优先级
    #[allow(dead_code)]
    pub fn safe_set_irq_priority(&self, hwi_num: u32, priority: u8) -> SystemResult<u32> {
        match self.set_irq_priority {
            Some(func) => Ok(func(hwi_num, priority)),
            None => Err(SystemError::Interrupt(InterruptError::ProcFuncNull)),
        }
    }

    /// 安全获取当前中断号
    #[allow(dead_code)]
    pub fn safe_get_cur_irq_num(&self) -> SystemResult<u32> {
        match self.get_cur_irq_num {
            Some(func) => Ok(func()),
            None => Err(SystemError::Interrupt(InterruptError::ProcFuncNull)),
        }
    }

    /// 安全获取中断版本
    #[allow(dead_code)]
    pub fn safe_get_irq_version(&self) -> SystemResult<*const c_char> {
        match self.get_irq_version {
            Some(func) => Ok(func()),
            None => Err(SystemError::Interrupt(InterruptError::ProcFuncNull)),
        }
    }

    /// 获取中断处理表单
    #[allow(dead_code)]
    pub fn get_handle_form(&self, hwi_num: u32) -> SystemResult<&mut InterruptHandler> {
        match self.get_handle_form {
            Some(func) => match unsafe { func(hwi_num).as_mut() } {
                Some(handle_form) => Ok(handle_form),
                None => Err(SystemError::Interrupt(InterruptError::NumInvalid)),
            },
            None => Err(SystemError::Interrupt(InterruptError::ProcFuncNull)),
        }
    }

    /// 安全处理中断
    #[allow(dead_code)]
    pub fn safe_handle_irq(&self) -> SystemResult<()> {
        match self.handle_irq {
            Some(func) => {
                func();
                Ok(())
            }
            None => Err(SystemError::Interrupt(InterruptError::ProcFuncNull)),
        }
    }
}
