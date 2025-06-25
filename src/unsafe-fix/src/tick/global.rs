use core::sync::atomic::{AtomicU64, Ordering};

/// 每个CPU核的系统tick计数器
#[unsafe(export_name = "g_tickCount")]
pub static TICK_COUNT: [AtomicU64; 1] = [AtomicU64::new(0)];

/// 获取tick计数
#[allow(dead_code)]
pub fn get_current_tick_count() -> u64 {
    TICK_COUNT[0].load(Ordering::Acquire)
}

/// 增加tick计数
#[allow(dead_code)]
pub fn increment_tick_count() {
    TICK_COUNT[0].fetch_add(1, Ordering::Release);
}
