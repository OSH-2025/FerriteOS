use crate::task::TaskCB;

unsafe extern "C" {
    #[link_name = "OsCurrTaskGetWrapper"]
    unsafe fn c_get_current_task() -> *mut TaskCB;
}

pub fn get_current_task() -> *mut TaskCB {
    // 在函数内部使用unsafe，调用外部C函数
    unsafe { c_get_current_task() }
}
