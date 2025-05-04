/// Information about memory usage for a specific task.
#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct TaskMemUsedInfo {
    /// Memory currently used by the task.
    pub mem_used: u32,

    /// Peak memory usage by the task.
    pub mem_peak: u32,
}

/// Memory statistics for the system, including per-task memory usage.
#[repr(C)]
pub struct Memstat {
    /// Total memory currently used.
    pub mem_total_used: u32,

    /// Peak total memory usage.
    pub mem_total_peak: u32,

    /// Memory usage statistics for each task.
    pub task_memstats: [TaskMemUsedInfo; TASK_NUM],
}

// TODO 通过 menuconfig 进行配置
const LOSCFG_BASE_CORE_TSK_LIMIT: usize = 64;

/// extra 1 blocks is for extra temparary task
pub const TASK_NUM: usize = LOSCFG_BASE_CORE_TSK_LIMIT + 1;

unsafe extern "C" {
    #[link_name = "OsMemstatTaskUsedInc"]
    unsafe fn os_memstat_task_used_inc_wrapper(stat: &mut Memstat, used_size: u32, task_id: u32);

    #[link_name = "OsMemstatTaskUsedDec"]
    unsafe fn os_memstat_task_used_dec_wrapper(stat: &mut Memstat, used_size: u32, task_id: u32);
}

pub fn os_memstat_task_used_inc(stat: &mut Memstat, used_size: u32, task_id: u32) {
    unsafe { os_memstat_task_used_inc_wrapper(stat, used_size, task_id) }
}

pub fn os_memstat_task_used_dec(stat: &mut Memstat, used_size: u32, task_id: u32) {
    unsafe { os_memstat_task_used_dec_wrapper(stat, used_size, task_id) }
}
