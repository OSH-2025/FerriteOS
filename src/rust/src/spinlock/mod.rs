use crate::hwi;

#[repr(C)]
pub struct Spinlock {
    pub raw_lock: u32,
}

#[inline]
pub fn los_spin_lock_save(_lock: *mut Spinlock, int_save: &mut u32) {
    *int_save = hwi::los_int_lock();
}

#[inline]
pub fn los_spin_unlock_restore(_lock: *mut Spinlock, int_save: u32) {
    hwi::los_int_restore(int_save);
}
