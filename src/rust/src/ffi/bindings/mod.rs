use crate::task::types::TaskCB;
use core::ffi::{c_char, c_void};

unsafe extern "C" {
    #[link_name = "ArchCurrTaskGetWrapper"]
    unsafe fn c_curr_task_get() -> *mut TaskCB;

    #[link_name = "ArchCurrTaskSetWrapper"]
    unsafe fn c_curr_task_set(val: *const core::ffi::c_void);

    #[link_name = "ArchIntLockedWrapper"]
    unsafe fn c_arch_int_locked() -> u32;

    #[link_name = "ArchIntLockWrapper"]
    unsafe fn c_arch_int_lock() -> u32;

    #[link_name = "ArchIntUnlockWrapper"]
    unsafe fn c_arch_int_unlock() -> u32;

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

    #[link_name = "ArchIrqInit"]
    unsafe fn c_arch_irq_init();

    #[link_name = "HalClockInit"]
    unsafe fn c_hal_clock_init();

    #[link_name = "HalClockStart"]
    unsafe fn c_hal_clock_start();

    #[link_name = "HalClockGetCycles"]
    unsafe fn c_hal_clock_get_cycles() -> u64;

    #[link_name = "HalDelayUs"]
    unsafe fn c_hal_delay_us(usecs: u32);

    #[link_name = "dprintf"]
    unsafe fn c_dprintf(fmt: *const c_char, ...);
}

#[inline]
pub fn hal_clock_init() {
    unsafe { c_hal_clock_init() }
}

#[inline]
pub fn hal_clock_start() {
    unsafe { c_hal_clock_start() }
}

#[inline]
pub fn hal_clock_get_cycles() -> u64 {
    unsafe { c_hal_clock_get_cycles() }
}

#[inline]
pub fn hal_delay_us(usecs: u32) {
    unsafe { c_hal_delay_us(usecs) }
}

#[inline]
pub fn get_current_task() -> &'static mut TaskCB {
    unsafe { c_curr_task_get().as_mut().expect("Current task is null") }
}

#[inline]
pub fn curr_task_set(task: *const TaskCB) {
    unsafe { c_curr_task_set(task as *const core::ffi::c_void) }
}

#[inline]
pub fn arch_int_locked() -> bool {
    unsafe { c_arch_int_locked() != 0 }
}

#[inline]
pub fn arch_int_lock() -> u32 {
    unsafe { c_arch_int_lock() }
}

#[inline]
pub fn arch_int_unlock() -> u32 {
    unsafe { c_arch_int_unlock() }
}

#[inline]
pub fn arch_int_restore(int_save: u32) {
    unsafe { c_arch_int_restore(int_save) }
}

#[inline]
pub fn os_task_schedule(new_task: *mut TaskCB, run_task: *mut TaskCB) {
    unsafe { c_os_task_schedule(new_task, run_task) }
}

#[inline]
pub fn wfi() {
    unsafe { c_wfi() }
}

#[inline]
pub fn task_stack_init(task_id: u32, stack_size: u32, top_stack: *mut c_void) -> *mut c_void {
    unsafe { c_task_stack_init(task_id, stack_size, top_stack) }
}

#[inline]
pub fn arch_irq_init() {
    unsafe { c_arch_irq_init() }
}

#[inline]
pub fn dprintf(fmt: *const c_char) {
    unsafe { c_dprintf(fmt) }
}
