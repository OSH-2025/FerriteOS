use core::ffi::c_void;

use crate::task::types::TaskCB;

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

    #[link_name = "OsTaskStackInit"]
    unsafe fn c_task_stack_init(
        task_id: u32,
        stack_size: u32,
        top_stack: *mut c_void,
    ) -> *mut c_void;
}

#[inline]
pub fn get_current_task() -> &'static mut TaskCB {
    unsafe { c_curr_task_get().as_mut().expect("Current task is null") }
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
    unsafe { c_os_task_schedule(new_task, run_task) }
}

#[inline]
pub(crate) fn wfi() {
    unsafe { c_wfi() }
}

#[inline]
pub(crate) fn task_stack_init(
    task_id: u32,
    stack_size: u32,
    top_stack: *mut c_void,
) -> *mut c_void {
    unsafe { c_task_stack_init(task_id, stack_size, top_stack) }
}
