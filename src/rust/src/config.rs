pub const OK: u32 = 0;
pub const NOK: u32 = 1;
pub const OS_INVALID: u32 = u32::MAX;
pub const WAIT_FOREVER : u32 = u32::MAX;

#[cfg(feature = "time_slice")]
pub const KERNEL_TIMESLICE_TIMEOUT: u16 = 2;

pub const STACK_POINT_ALIGN_SIZE: u32 = 8;

pub const TASK_PRIORITY_LOWEST: u16 = 31;
pub const TASK_LIMIT: u32 = 64;
pub const TASK_DEFAULT_STACK_SIZE: u32 = 24576;
pub const TASK_MIN_STACK_SIZE: u32 = 2048;
pub const TASK_IDLE_STACK_SIZE: u32 = 2048;

// tick
pub const SYS_CLOCK: u32 = 0x6000000;
pub const TICK_PER_SECOND: u32 = 1000;

// stack
pub const STACK_MAGIC_WORD: usize = 0xCCCCCCCC;
pub const STACK_INIT_PATTERN: usize = 0xCACACACA;

/// mutex
pub const MUX_LIMIT: u32 = 1024;
