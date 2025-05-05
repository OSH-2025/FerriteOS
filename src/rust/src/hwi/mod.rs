unsafe extern "C" {
    #[link_name = "LOS_IntLock"]
    unsafe fn los_int_lock_wrapper() -> u32;

    #[link_name = "LOS_IntRestore"]
    unsafe fn los_int_restore_wrapper(int_save: u32);
}

#[inline]
pub fn los_int_lock() -> u32 {
    unsafe { los_int_lock_wrapper() }
}

#[inline]
pub fn los_int_restore(int_save: u32) {
    unsafe { los_int_restore_wrapper(int_save) }
}
