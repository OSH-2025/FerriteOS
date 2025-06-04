pub mod error;

unsafe extern "C" {
    #[link_name = "LOS_QueueReadCopy"]
    unsafe fn c_los_queue_read_copy(
        queue_id: u32,
        buffer_addr: *mut core::ffi::c_void,
        buffer_size: *mut u32,
        timeout: u32,
    ) -> u32;

    #[link_name = "LOS_QueueCreate"]
    unsafe fn c_los_queue_create(len: u16, queue_id: *mut u32, max_msg_size: u16) -> u32;

    #[link_name = "LOS_QueueWriteCopy"]
    unsafe fn c_los_queue_write_copy(
        queue_id: u32,
        buffer_addr: *const core::ffi::c_void,
        buffer_size: u32,
        timeout: u32,
    ) -> u32;
}

pub fn los_queue_read_copy(
    queue_id: u32,
    buffer_addr: *mut core::ffi::c_void,
    buffer_size: *mut u32,
    timeout: u32,
) -> u32 {
    unsafe { c_los_queue_read_copy(queue_id, buffer_addr, buffer_size, timeout) }
}

pub fn los_queue_create(len: u16, queue_id: *mut u32, max_msg_size: u16) -> u32 {
    unsafe { c_los_queue_create(len, queue_id, max_msg_size) }
}

pub fn los_queue_write_copy(
    queue_id: u32,
    buffer_addr: *const core::ffi::c_void,
    buffer_size: u32,
    timeout: u32,
) -> u32 {
    unsafe { c_los_queue_write_copy(queue_id, buffer_addr, buffer_size, timeout) }
}
