use crate::interrupt::types::InterruptController;
use core::sync::atomic::{AtomicPtr, Ordering};

/// 全局中断控制器操作接口（使用原子指针确保线程安全）
#[unsafe(export_name = "g_hwiOps")]
pub static INTERRUPT_CONTROLLER: AtomicPtr<InterruptController> =
    AtomicPtr::new(core::ptr::null_mut());

/// 注册中断控制器操作接口
pub fn register_hwi_controller(ops: *mut InterruptController) {
    INTERRUPT_CONTROLLER.store(ops, Ordering::Release);
}

/// 获取全局中断控制器操作接口
#[allow(dead_code)]
pub fn get_interrupt_controller() -> Option<&'static InterruptController> {
    let ops_ptr = INTERRUPT_CONTROLLER.load(Ordering::Acquire);
    unsafe { ops_ptr.as_ref() }
}

// C兼容接口，保持与原C代码的兼容性
#[unsafe(export_name = "OsHwiControllerReg")]
pub extern "C" fn os_hwi_controller_reg(ops: *mut InterruptController) {
    register_hwi_controller(ops);
}
