use crate::tick::{
    delay_microseconds, delay_milliseconds, get_cpu_cycles, get_current_nanoseconds, get_cycles_per_tick, get_tick_count, handle_tick, initialize_tick, milliseconds_to_ticks, start_tick, ticks_to_milliseconds
};

#[unsafe(export_name = "OsTickInit")]
pub extern "C" fn os_tick_init() {
    initialize_tick();
}

#[unsafe(export_name = "OsTickStart")]
pub extern "C" fn os_tick_start() {
    start_tick();
}

#[unsafe(export_name = "OsTickHandler")]
pub extern "C" fn os_tick_handler() {
    handle_tick();
}

#[unsafe(export_name = "LOS_TickCountGet")]
pub extern "C" fn los_tick_count_get() -> u64 {
    get_tick_count()
}

#[unsafe(export_name = "LOS_CyclePerTickGet")]
pub extern "C" fn los_cycle_per_tick_get() -> u32 {
    get_cycles_per_tick()
}

#[unsafe(export_name = "LOS_GetCpuCycle")]
pub extern "C" fn los_get_cpu_cycle() -> u64 {
    get_cpu_cycles()
}

#[unsafe(export_name = "LOS_CurrNanosec")]
pub extern "C" fn los_curr_nanosec() -> u64 {
    get_current_nanoseconds()
}

/// 将毫秒转换为系统Tick数
///
/// # Arguments
/// * `millisec` - 毫秒数
///
/// # Returns
/// 对应的系统Tick数，使用向上取整确保延迟不会过短
///
/// # Note
/// 当输入为u32::MAX时，返回u32::MAX表示无限等待
#[unsafe(export_name = "LOS_MS2Tick")]
pub extern "C" fn los_ms_to_tick(millisec: u32) -> u32 {
    milliseconds_to_ticks(millisec)
}

/// 将系统Tick数转换为毫秒
///
/// # Arguments
/// * `tick` - 系统Tick数
///
/// # Returns
/// 对应的毫秒数
#[unsafe(export_name = "LOS_Tick2MS")]
pub extern "C" fn los_tick_to_ms(tick: u32) -> u32 {
    ticks_to_milliseconds(tick)
}

/// 微秒级延迟
///
/// # Arguments
/// * `usecs` - 延迟的微秒数
///
/// # Safety
/// 调用硬件抽象层延迟函数，在中断上下文中使用需要谨慎
#[unsafe(export_name = "LOS_Udelay")]
pub extern "C" fn los_udelay(usecs: u32) {
    delay_microseconds(usecs);
}

/// 毫秒级延迟
///
/// # Arguments
/// * `msecs` - 延迟的毫秒数
///
/// # Note
/// 为避免溢出，对于大的毫秒值会分批处理
#[unsafe(export_name = "LOS_Mdelay")]
pub extern "C" fn los_mdelay(msecs: u32) {
    delay_milliseconds(msecs);
}
