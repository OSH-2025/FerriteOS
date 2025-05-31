use crate::ffi::bindings::{arch_int_lock, arch_int_restore, arch_int_unlock};

pub mod types;

#[inline]
pub fn int_lock() -> u32 {
    arch_int_lock()
}

#[inline]
pub fn int_unlock() -> u32 {
    arch_int_unlock()
}

#[inline]
pub fn int_restore(int_save: u32) {
    arch_int_restore(int_save);
}

unsafe extern "C" {
    #[link_name = "IntActive"]
    unsafe fn c_int_active() -> usize;
}

#[inline]
pub fn is_int_active() -> bool {
    unsafe { c_int_active() != 0 }
}
