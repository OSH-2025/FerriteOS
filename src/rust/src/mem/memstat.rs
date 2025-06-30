use super::{defs::*, mempool::LosMemPoolInfo};

#[repr(C)]
pub struct TaskMemUsedInfo {
    pub mem_used: u32,
    pub mem_peak: u32,
}

#[repr(C)]
pub struct Memstat {
    pub mem_total_used: u32,
    pub mem_total_peak: u32,
    pub task_memstats: [TaskMemUsedInfo; TASK_NUM],
}

// TODO 通过 menuconfig 进行配置
const LOSCFG_BASE_CORE_TSK_LIMIT: usize = 64;

/// extra 1 blocks is for extra temparary task
pub const TASK_NUM: usize = LOSCFG_BASE_CORE_TSK_LIMIT + 1;

pub fn os_memstat_task_used_inc(stat: &mut Memstat, used_size: u32, task_id: u32) {
    let record = usize::min(task_id as usize, TASK_NUM - 1);
    stat.task_memstats[record].mem_used += used_size;
    stat.task_memstats[record].mem_peak = u32::max(
        stat.task_memstats[record].mem_peak,
        stat.task_memstats[record].mem_used,
    );
    stat.mem_total_used += used_size;
    stat.mem_total_peak = u32::max(stat.mem_total_peak, stat.mem_total_used);
    // unsafe {
    //     dprintf(
    //         b"mem used of task '%d': 0x%x, increase size: 0x%x\n\0" as *const u8,
    //         task_id,
    //         stat.task_memstats[record].mem_used,
    //         used_size,
    //     );
    // }
}

pub fn os_memstat_task_used_dec(stat: &mut Memstat, used_size: u32, task_id: u32) {
    let record = usize::min(task_id as usize, TASK_NUM - 1);
    if stat.task_memstats[record].mem_used < used_size {
        // unsafe {
        //     dprintf(
        //         b"mem used of task '%d': 0x%x, decrease size: 0x%x\n\0" as *const u8,
        //         task_id,
        //         stat.task_memstats[record].mem_used,
        //         used_size,
        //     );
        // }
        return;
    }
    stat.task_memstats[record].mem_used -= used_size;
    stat.mem_total_used -= used_size;
    // unsafe {
    //     dprintf(
    //         b"mem used of task '%d': 0x%x, decrease size: 0x%x\n\0" as *const u8,
    //         task_id,
    //         stat.task_memstats[record].mem_used,
    //         used_size,
    //     );
    // }
}

fn os_mem_task_usage(stat: &Memstat, task_id: u32) -> u32 {
    let record = usize::min(task_id as usize, TASK_NUM - 1);
    stat.task_memstats[record].mem_used
}

pub fn os_memstat_task_usage(task_id: u32) -> u32 {
    // TODO LOSCFG_MEM_MUL_POOL
    unsafe {
        let pool = os_sys_mem_addr() as *mut LosMemPoolInfo;
        let stat = &(*pool).stat;
        os_mem_task_usage(stat, task_id)
    }
}

fn os_mem_task_clear(stat: &mut Memstat, task_id: u32) {
    let record = usize::min(task_id as usize, TASK_NUM - 1);

    if stat.task_memstats[record].mem_used != 0 {
        // unsafe {
        //     dprintf(
        //         b"mem used of task '%d' is 0x%x, not zero when task being deleted\n\0" as *const u8,
        //         task_id,
        //         stat.task_memstats[record].mem_used,
        //     );
        // }
    }
    stat.task_memstats[record].mem_used = 0;
    stat.task_memstats[record].mem_peak = 0;
    // unsafe {
    //     dprintf(
    //         b"mem used of task '%d' is cleared\n\0" as *const u8,
    //         task_id,
    //     );
    // }
}

pub fn os_memstat_task_clear(task_id: u32) {
    unsafe {
        let pool = os_sys_mem_addr() as *mut LosMemPoolInfo;
        let stat = &mut (*pool).stat;
        os_mem_task_clear(stat, task_id);
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn OsMemstatTaskClear(task_id: u32) {
    os_memstat_task_clear(task_id);
}

#[unsafe(no_mangle)]
pub extern "C" fn OsMemstatTaskUsage(task_id: u32) -> u32 {
    os_memstat_task_usage(task_id)
}
