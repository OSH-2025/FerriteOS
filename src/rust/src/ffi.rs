use crate::task::TaskCB;

unsafe extern "C" {
    #[link_name = "ArchCurrTaskGetWrapper"]
    unsafe fn c_curr_task_get() -> *mut TaskCB;

    #[link_name = "ArchCurrTaskSetWrapper"]
    unsafe fn c_curr_task_set(val: *const core::ffi::c_void);

    #[link_name = "ArchIntLockedWrapper"]
    unsafe fn c_arch_int_locked() -> u32;

    #[link_name = "ArchIntLockWrapper"]
    unsafe fn c_arch_int_lock() -> u32;

    #[link_name = "ArchIntRestoreWrapper"]
    unsafe fn c_arch_int_restore(int_save: u32);

    #[link_name = "OsTaskScheduleWrapper"]
    unsafe fn c_os_task_schedule(new_task: *mut TaskCB, run_task: *mut TaskCB);

    #[link_name = "WfiWrapper"]
    unsafe fn c_wfi();
}

#[inline]
pub(crate) fn curr_task_get() -> *mut TaskCB {
    unsafe { c_curr_task_get() }
}

#[inline]
pub(crate) fn curr_task_set(task: *const TaskCB) {
    unsafe { c_curr_task_set(task as *const core::ffi::c_void) }
}

#[inline]
pub(crate) fn arch_int_locked() -> bool {
    unsafe { c_arch_int_locked() != 0 }
}

#[inline]
pub(crate) fn arch_int_lock() -> u32 {
    unsafe { c_arch_int_lock() }
}

#[inline]
pub(crate) fn arch_int_restore(int_save: u32) {
    unsafe { c_arch_int_restore(int_save) }
}

#[inline]
pub(crate) fn os_task_schedule(new_task: *mut TaskCB, run_task: *mut TaskCB) {
    unsafe { crate::ffi::c_os_task_schedule(new_task, run_task) }
}

#[inline]
pub(crate) fn wfi() {
    unsafe { c_wfi() }
}
