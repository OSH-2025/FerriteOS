unsafe extern "C" {
    // kernel/base/mem/bestfit/los_memory.c
    #[link_name = "GetOsSysMemSizeWrapper"]
    unsafe fn get_os_sys_mem_size_wrapper() -> u32;
}

#[inline]
pub fn get_os_sys_mem_size() -> u32 {
    unsafe { get_os_sys_mem_size_wrapper() }
}

pub const LOS_OK: u32 = 0;
pub const LOS_NOK: u32 = 1;
pub const OS_INVALID: u32 = u32::MAX;
