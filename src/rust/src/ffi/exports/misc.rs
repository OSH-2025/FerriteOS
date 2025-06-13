use crate::{
    task::manager::delay::task_delay,
    tick::milliseconds_to_ticks,
    utils::{align::align_up, memdump::dump_region},
};

#[unsafe(export_name = "LOS_Align")]
pub extern "C" fn los_align(addr: u32, boundary: u32) -> u32 {
    return align_up(addr, boundary);
}

/// 毫秒级休眠
///
/// # 参数
///
/// * `msecs` - 休眠毫秒数
#[unsafe(export_name = "LOS_Msleep")]
pub extern "C" fn los_msleep(msecs: u32) {
    let interval = if msecs == 0 {
        0
    } else {
        milliseconds_to_ticks(msecs).saturating_add(1)
    };
    let _ = task_delay(interval);
}

#[unsafe(export_name = "OsDumpMemByte")]
pub extern "C" fn os_dump_mem_byte(length: usize, addr: usize) {
    dump_region(addr, length);
}

#[unsafe(export_name = "OsArraySort")]
pub extern "C" fn os_array_sort() {
    todo!("OsArraySort is not implemented yet");
}
