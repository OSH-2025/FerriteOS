use crate::{
    ffi::bindings::{hal_clock_init, hal_clock_start},
    interrupt::{disable_interrupts, restore_interrupt_state},
    task::timer::task_scan,
};
use global::increment_tick_count;
pub mod global;

pub use clock::*;
pub use convert::*;
pub use delay::*;

/// 初始化系统Tick
pub fn initialize_tick() {
    hal_clock_init();
}

/// 启动系统Tick
pub fn start_tick() {
    hal_clock_start();
}

pub fn handle_tick() {
    #[cfg(feature = "software_timer")]
    use crate::swtmr::swtmr_scan;

    #[cfg(feature = "time_slice")]
    use crate::task::sched::timeslice_check;

    // 禁用中断以确保原子操作
    let int_save = disable_interrupts();

    // 增加当前CPU的tick计数
    increment_tick_count();

    // 恢复中断状态
    restore_interrupt_state(int_save);

    // 处理时间片（如果启用）
    #[cfg(feature = "time_slice")]
    timeslice_check();

    // 处理任务超时
    task_scan();

    // 处理软件定时器（如果启用）
    #[cfg(feature = "software_timer")]
    swtmr_scan();
}

pub mod clock {
    const NS_PER_SECOND: u64 = 1_000_000_000; // 每秒纳秒数
    use crate::{
        config::{SYS_CLOCK, TICK_PER_SECOND},
        ffi::bindings::hal_clock_get_cycles,
        interrupt::{disable_interrupts, restore_interrupt_state},
        tick::global::get_current_tick_count,
    };

    pub fn get_tick_count() -> u64 {
        // 禁用中断以确保原子操作
        let int_save = disable_interrupts();

        // 读取核心0的tick计数
        let tick = get_current_tick_count();

        // 恢复中断状态
        restore_interrupt_state(int_save);

        tick
    }

    pub const fn get_cycles_per_tick() -> u32 {
        SYS_CLOCK / TICK_PER_SECOND
    }

    // 获取CPU周期计数
    pub fn get_cpu_cycles() -> u64 {
        hal_clock_get_cycles()
    }

    pub fn get_current_nanoseconds() -> u64 {
        let cycle = get_cpu_cycles();
        ((cycle as u128) * (NS_PER_SECOND as u128) / (SYS_CLOCK as u128)) as u64
    }
}

/// 时间单位转换模块
pub mod convert {
    use crate::config::TICK_PER_SECOND;
    const MS_PER_SECOND: u64 = 1000;

    /// 将毫秒转换为系统Tick数
    ///
    /// 使用向上取整确保延迟时间不会过短
    pub fn milliseconds_to_ticks(millisec: u32) -> u32 {
        // 特殊情况：无限等待
        if millisec == u32::MAX {
            return u32::MAX;
        }

        // 避免溢出，使用u64进行中间计算
        let millisec = millisec as u64;

        // 向上取整：(delay_ms * ticks_per_sec + ms_per_sec - 1) / ms_per_sec
        let ticks = (millisec * TICK_PER_SECOND as u64 + MS_PER_SECOND - 1) / MS_PER_SECOND;

        // 确保结果在u32范围内
        ticks.min(u32::MAX as u64) as u32
    }

    /// 将系统Tick数转换为毫秒
    pub fn ticks_to_milliseconds(tick: u32) -> u32 {
        // 使用u64避免中间计算溢出
        let tick = tick as u64;

        // 先乘后除以保持精度
        let ms = (tick * MS_PER_SECOND) / TICK_PER_SECOND as u64;

        // 确保结果在u32范围内
        ms.min(u32::MAX as u64) as u32
    }
}

pub mod delay {
    use crate::ffi::bindings::hal_delay_us;

    const US_PER_MS: u32 = 1000; // 每毫秒的微秒数

    /// 毫秒延迟函数
    ///
    /// # Arguments
    /// * `msecs` - 延迟的毫秒数
    ///
    /// # Implementation Notes
    /// 为防止乘法溢出，将大的延迟值分解为多个较小的延迟
    pub fn delay_milliseconds(msecs: u32) {
        if msecs == 0 {
            return;
        }

        // 计算最大安全的毫秒数，避免 msecs * US_PER_MS 溢出
        let max_safe_ms = u32::MAX / US_PER_MS;

        // 计算一次可以安全延迟的最大微秒数
        let max_delay_us = max_safe_ms * US_PER_MS;

        let mut remaining_ms = msecs;

        // 处理大的延迟值，分批执行
        while remaining_ms > max_safe_ms {
            hal_delay_us(max_delay_us);
            remaining_ms -= max_safe_ms;
        }

        // 处理剩余的延迟
        if remaining_ms > 0 {
            hal_delay_us(remaining_ms * US_PER_MS);
        }
    }

    /// 精确的微秒延迟
    ///
    /// # Arguments
    /// * `usecs` - 延迟的微秒数（64位）
    pub fn delay_microseconds(usecs: u32) {
        hal_delay_us(usecs);
    }
}
