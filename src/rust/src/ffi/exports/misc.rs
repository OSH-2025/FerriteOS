use crate::{
    task::manager::delay::task_delay, tick::milliseconds_to_ticks, utils::memdump::dump_region,
};

/// 毫秒级休眠
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
