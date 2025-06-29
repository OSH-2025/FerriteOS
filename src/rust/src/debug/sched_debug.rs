use core::ptr;
use core::ffi::CStr;

// 类型定义
type UINT32 = u32;
type UINT64 = u64;
type BOOL = bool;

// 常量定义
const HIGHTASKPRI: u32 = 16;
const NS_PER_MS: u64 = 1000000;
const DECIMAL_TO_PERCENTAGE: u64 = 100;

// 假设的配置常量
const LOSCFG_KERNEL_CORE_NUM: usize = 4;
const KERNEL_TSK_LIMIT: usize = 1024;
const OS_TASK_STATUS_UNUSED: u32 = 0x0001;
const OS_TASK_STATUS_RUNNING: u32 = 0x0010;
const OS_TASK_PRIORITY_LOWEST: u32 = 31;
const OS_TASK_INVALID_CPUID: u32 = 0xFFFFFFFF;
const OS_TICK_INT_NUM: usize = 0;

#[cfg(feature = "smp")]
const IPI_INTERRUPT_LIMIT: usize = 16;

// 结构体定义
#[repr(C)]
struct StatPercpu {
    idle_runtime: UINT64,
    idle_starttime: UINT64,
    high_task_runtime: UINT64,
    high_task_starttime: UINT64,
    sum_priority: UINT64,
    priority_switch: UINT32,
    high_task_switch: UINT32,
    contex_switch: UINT32,
    hwi_num: UINT32,
    #[cfg(feature = "smp")]
    ipi_irq_num: UINT32,
}

impl Default for StatPercpu {
    fn default() -> Self {
        Self {
            idle_runtime: 0,
            idle_starttime: 0,
            high_task_runtime: 0,
            high_task_starttime: 0,
            sum_priority: 0,
            priority_switch: 0,
            high_task_switch: 0,
            contex_switch: 0,
            hwi_num: 0,
            #[cfg(feature = "smp")]
            ipi_irq_num: 0,
        }
    }
}

#[repr(C)]
struct SchedPercpu {
    runtime: UINT64,
    contex_switch: UINT32,
}

#[repr(C)]
struct SchedStat {
    all_runtime: UINT64,
    start_runtime: UINT64,
    all_context_switch: UINT32,
    sched_percpu: [SchedPercpu; LOSCFG_KERNEL_CORE_NUM],
}

#[repr(C)]
struct LosTaskCB {
    task_id: UINT32,
    task_status: UINT32,
    priority: UINT32,
    task_name: [u8; 32], // 假设任务名长度
    sched_stat: SchedStat,
    #[cfg(feature = "smp")]
    curr_cpu: UINT32,
    #[cfg(feature = "smp")]
    cpu_affi_mask: UINT64,
}

// 全局静态变量
#[cfg(feature = "debug_sched_statistics")]
static mut G_STATISTICS_START_FLAG: BOOL = false;

#[cfg(feature = "debug_sched_statistics")]
static mut G_STATISTICS_START_TIME: UINT64 = 0;

#[cfg(feature = "debug_sched_statistics")]
static mut G_STAT_PERCPU: [StatPercpu; LOSCFG_KERNEL_CORE_NUM] = 
    [StatPercpu {
        idle_runtime: 0,
        idle_starttime: 0,
        high_task_runtime: 0,
        high_task_starttime: 0,
        sum_priority: 0,
        priority_switch: 0,
        high_task_switch: 0,
        contex_switch: 0,
        hwi_num: 0,
        #[cfg(feature = "smp")]
        ipi_irq_num: 0,
    }; LOSCFG_KERNEL_CORE_NUM];

// 外部函数声明
extern "C" {
    fn ArchCurrCpuid() -> UINT32;
    fn OsGetIdleTaskId() -> UINT32;
    fn LOS_CurrNanosec() -> UINT64;
    fn PRINTK(format: *const i8, ...);
    fn PRINT_WARN(format: *const i8, ...);
    fn SCHEDULER_LOCK(int_save: &mut UINT32);
    fn SCHEDULER_UNLOCK(int_save: UINT32);
    static g_taskCBArray: *mut LosTaskCB;
    fn strcmp(s1: *const i8, s2: *const i8) -> i32;
    fn memset_s(dest: *mut u8, dest_max: usize, c: i32, count: usize) -> i32;
}

#[cfg(feature = "debug_sched_statistics")]
fn os_sched_statistics_per_cpu(run_task: &LosTaskCB, new_task: &LosTaskCB) {
    unsafe {
        if !G_STATISTICS_START_FLAG {
            return;
        }

        let cpu_id = ArchCurrCpuid() as usize;
        let idle_task_id = OsGetIdleTaskId();
        let now = LOS_CurrNanosec();

        if cpu_id >= LOSCFG_KERNEL_CORE_NUM {
            return;
        }

        G_STAT_PERCPU[cpu_id].contex_switch += 1;

        // 任务从非空闲切换到空闲
        if run_task.task_id != idle_task_id && new_task.task_id == idle_task_id {
            G_STAT_PERCPU[cpu_id].idle_starttime = now;
        }

        // 任务从空闲切换到非空闲
        if run_task.task_id == idle_task_id && new_task.task_id != idle_task_id {
            let runtime = now - G_STAT_PERCPU[cpu_id].idle_starttime;
            G_STAT_PERCPU[cpu_id].idle_runtime += runtime;
            G_STAT_PERCPU[cpu_id].idle_starttime = 0;
        }

        // 从低优先级任务切换到高优先级任务
        if run_task.priority >= HIGHTASKPRI && new_task.priority < HIGHTASKPRI {
            G_STAT_PERCPU[cpu_id].high_task_starttime = now;
        }

        // 从高优先级任务切换到低优先级任务
        if run_task.priority < HIGHTASKPRI && new_task.priority >= HIGHTASKPRI {
            let runtime = now - G_STAT_PERCPU[cpu_id].high_task_starttime;
            G_STAT_PERCPU[cpu_id].high_task_runtime += runtime;
            G_STAT_PERCPU[cpu_id].high_task_starttime = 0;
        }

        if new_task.priority < HIGHTASKPRI {
            G_STAT_PERCPU[cpu_id].high_task_switch += 1;
        }

        if new_task.task_id != idle_task_id {
            G_STAT_PERCPU[cpu_id].sum_priority += new_task.priority as UINT64;
            G_STAT_PERCPU[cpu_id].priority_switch += 1;
        }
    }
}

pub fn os_sched_statistics(run_task: &mut LosTaskCB, new_task: &mut LosTaskCB) {
    unsafe {
        let cpu_id = ArchCurrCpuid() as usize;
        let now = LOS_CurrNanosec();

        if cpu_id >= LOSCFG_KERNEL_CORE_NUM {
            return;
        }

        // 计算运行时间
        let runtime = now - run_task.sched_stat.start_runtime;

        // 更新运行任务的统计信息
        run_task.sched_stat.sched_percpu[cpu_id].runtime += runtime;
        run_task.sched_stat.all_runtime += runtime;

        // 更新新任务的统计信息
        new_task.sched_stat.sched_percpu[cpu_id].contex_switch += 1;
        new_task.sched_stat.all_context_switch += 1;
        new_task.sched_stat.start_runtime = now;

        #[cfg(feature = "debug_sched_statistics")]
        os_sched_statistics_per_cpu(run_task, new_task);
    }
}

pub fn os_hwi_statistics(int_num: usize) {
    #[cfg(feature = "debug_sched_statistics")]
    unsafe {
        let cpu_id = ArchCurrCpuid() as usize;

        if !G_STATISTICS_START_FLAG || int_num == OS_TICK_INT_NUM || cpu_id >= LOSCFG_KERNEL_CORE_NUM {
            return;
        }

        G_STAT_PERCPU[cpu_id].hwi_num += 1;

        #[cfg(feature = "smp")]
        if int_num < IPI_INTERRUPT_LIMIT {
            G_STAT_PERCPU[cpu_id].ipi_irq_num += 1;
        }
    }
}

pub fn os_shell_cmd_dump_sched() {
    unsafe {
        let header = b"\nTask                          TID              Total Time     Total CST     CPU                   Time           CST\n\0";
        let separator = b"----                          ---      ------------------    ----------    ----     ------------------    ----------\n\0";
        
        PRINTK(header.as_ptr() as *const i8);
        PRINTK(separator.as_ptr() as *const i8);

        for loop_idx in 0..KERNEL_TSK_LIMIT {
            let task_cb = (g_taskCBArray as *mut LosTaskCB).add(loop_idx);
            if (*task_cb).task_status & OS_TASK_STATUS_UNUSED != 0 {
                continue;
            }

            // 将任务名转换为null终止的字符串进行打印
            let task_name = (*task_cb).task_name.as_ptr() as *const i8;
            let task_format = b"%-30s0x%-6x%+16.3f ms  %10u\n\0";
            PRINTK(task_format.as_ptr() as *const i8,
                   task_name,
                   (*task_cb).task_id,
                   ((*task_cb).sched_stat.all_runtime as f64) / (NS_PER_MS as f64),
                   (*task_cb).sched_stat.all_context_switch);

            for cpu_id in 0..LOSCFG_KERNEL_CORE_NUM {
                #[cfg(feature = "smp")]
                {
                    let affinity = (*task_cb).cpu_affi_mask;
                    if (1u64 << cpu_id) & affinity == 0 {
                        continue;
                    }
                }

                let cpu_format = b"                                                                           CPU%u    %+16.3f ms  %12u\n\0";
                PRINTK(cpu_format.as_ptr() as *const i8,
                       cpu_id,
                       ((*task_cb).sched_stat.sched_percpu[cpu_id].runtime as f64) / (NS_PER_MS as f64),
                       (*task_cb).sched_stat.sched_percpu[cpu_id].contex_switch);
            }
        }

        let end_newline = b"\n\0";
        PRINTK(end_newline.as_ptr() as *const i8);
    }
}

#[cfg(feature = "debug_sched_statistics")]
pub fn os_statistics_show(statistics_past_time: UINT64) {
    unsafe {
        let time_format = b"\nPassed Time: %+16.3f ms\n\0";
        PRINTK(time_format.as_ptr() as *const i8, 
               (statistics_past_time as f64) / (NS_PER_MS as f64));

        let separator = b"--------------------------------\n\0";
        PRINTK(separator.as_ptr() as *const i8);

        #[cfg(feature = "smp")]
        let header = b"CPU       Idle(%%)      ContexSwitch    HwiNum       Avg Pri      HiTask(%%)	   HiTask SwiNum       HiTask P(ms)      MP Hwi\n\0";
        #[cfg(not(feature = "smp"))]
        let header = b"CPU       Idle(%%)      ContexSwitch    HwiNum       Avg Pri      HiTask(%%)	   HiTask SwiNum       HiTask P(ms)\n\0";
        PRINTK(header.as_ptr() as *const i8);

        #[cfg(feature = "smp")]
        let table_sep = b"----    ---------      -----------    --------    ---------     ----------         ------------       ----------        ------\n\0";
        #[cfg(not(feature = "smp"))]
        let table_sep = b"----    ---------      -----------    --------    ---------     ----------         ------------       ----------\n\0";
        PRINTK(table_sep.as_ptr() as *const i8);

        for cpu_id in 0..LOSCFG_KERNEL_CORE_NUM {
            let idle_percentage = (G_STAT_PERCPU[cpu_id].idle_runtime as f64 / statistics_past_time as f64) * DECIMAL_TO_PERCENTAGE as f64;
            let avg_priority = if G_STAT_PERCPU[cpu_id].priority_switch == 0 {
                OS_TASK_PRIORITY_LOWEST as f64
            } else {
                G_STAT_PERCPU[cpu_id].sum_priority as f64 / G_STAT_PERCPU[cpu_id].priority_switch as f64
            };
            let high_task_percentage = (G_STAT_PERCPU[cpu_id].high_task_runtime as f64 / statistics_past_time as f64) * DECIMAL_TO_PERCENTAGE as f64;
            let high_task_period = if G_STAT_PERCPU[cpu_id].high_task_switch == 0 {
                0.0
            } else {
                (G_STAT_PERCPU[cpu_id].high_task_runtime as f64 / G_STAT_PERCPU[cpu_id].high_task_switch as f64) / NS_PER_MS as f64
            };

            #[cfg(feature = "smp")]
            {
                let format = b"CPU%u   %+10.3f%14u%14u   %+11.3f   %+11.3f%14u              %+11.3f  %11u\n\0";
                PRINTK(format.as_ptr() as *const i8,
                       cpu_id,
                       idle_percentage,
                       G_STAT_PERCPU[cpu_id].contex_switch,
                       G_STAT_PERCPU[cpu_id].hwi_num,
                       avg_priority,
                       high_task_percentage,
                       G_STAT_PERCPU[cpu_id].high_task_switch,
                       high_task_period,
                       G_STAT_PERCPU[cpu_id].ipi_irq_num);
            }
            #[cfg(not(feature = "smp"))]
            {
                let format = b"CPU%u   %+10.3f%14u%14u   %+11.3f   %+11.3f%14u              %+11.3f\n\0";
                PRINTK(format.as_ptr() as *const i8,
                       cpu_id,
                       idle_percentage,
                       G_STAT_PERCPU[cpu_id].contex_switch,
                       G_STAT_PERCPU[cpu_id].hwi_num,
                       avg_priority,
                       high_task_percentage,
                       G_STAT_PERCPU[cpu_id].high_task_switch,
                       high_task_period);
            }
        }

        let end_newline = b"\n\0";
        PRINTK(end_newline.as_ptr() as *const i8);
    }
}

#[cfg(feature = "debug_sched_statistics")]
pub fn os_shell_statistics_start() {
    unsafe {
        let mut int_save: UINT32 = 0;
        SCHEDULER_LOCK(&mut int_save);

        if G_STATISTICS_START_FLAG {
            SCHEDULER_UNLOCK(int_save);
            let warn_msg = b"mp static has started\n\0";
            PRINT_WARN(warn_msg.as_ptr() as *const i8);
            return;
        }

        G_STATISTICS_START_TIME = LOS_CurrNanosec();

        for loop_idx in 0..KERNEL_TSK_LIMIT {
            let task_cb = (g_taskCBArray as *mut LosTaskCB).add(loop_idx);
            if (*task_cb).task_status & OS_TASK_STATUS_RUNNING == 0 {
                continue;
            }

            #[cfg(feature = "smp")]
            let cpu_id = (*task_cb).curr_cpu as usize;
            #[cfg(not(feature = "smp"))]
            let cpu_id = 0usize;

            if cpu_id == OS_TASK_INVALID_CPUID as usize || cpu_id >= LOSCFG_KERNEL_CORE_NUM {
                continue;
            }

            let idle_core_name = b"IdleCore000\0";
            let task_name = (*task_cb).task_name.as_ptr() as *const i8;
            
            if strcmp(task_name, idle_core_name.as_ptr() as *const i8) == 0 {
                G_STAT_PERCPU[cpu_id].idle_starttime = G_STATISTICS_START_TIME;
            }

            if (*task_cb).priority < HIGHTASKPRI {
                G_STAT_PERCPU[cpu_id].high_task_starttime = G_STATISTICS_START_TIME;
                G_STAT_PERCPU[cpu_id].high_task_switch += 1;
            }

            if strcmp(task_name, idle_core_name.as_ptr() as *const i8) != 0 {
                G_STAT_PERCPU[cpu_id].sum_priority += (*task_cb).priority as UINT64;
                G_STAT_PERCPU[cpu_id].priority_switch += 1;
            }
        }

        G_STATISTICS_START_FLAG = true;
        SCHEDULER_UNLOCK(int_save);

        let start_msg = b"mp static start\n\0";
        PRINTK(start_msg.as_ptr() as *const i8);
    }
}

#[cfg(feature = "debug_sched_statistics")]
pub fn os_shell_statistics_stop() {
    unsafe {
        let mut int_save: UINT32 = 0;
        SCHEDULER_LOCK(&mut int_save);

        if !G_STATISTICS_START_FLAG {
            SCHEDULER_UNLOCK(int_save);
            let warn_msg = b"Please set mp static start\n\0";
            PRINT_WARN(warn_msg.as_ptr() as *const i8);
            return;
        }

        G_STATISTICS_START_FLAG = false;
        let statistics_stop_time = LOS_CurrNanosec();
        let statistics_past_time = statistics_stop_time - G_STATISTICS_START_TIME;

        for loop_idx in 0..KERNEL_TSK_LIMIT {
            let task_cb = (g_taskCBArray as *mut LosTaskCB).add(loop_idx);
            if (*task_cb).task_status & OS_TASK_STATUS_RUNNING == 0 {
                continue;
            }

            #[cfg(feature = "smp")]
            let cpu_id = (*task_cb).curr_cpu as usize;
            #[cfg(not(feature = "smp"))]
            let cpu_id = 0usize;

            if cpu_id == OS_TASK_INVALID_CPUID as usize || cpu_id >= LOSCFG_KERNEL_CORE_NUM {
                continue;
            }

            let idle_core_name = b"IdleCore000\0";
            let task_name = (*task_cb).task_name.as_ptr() as *const i8;

            if strcmp(task_name, idle_core_name.as_ptr() as *const i8) == 0 {
                let runtime = statistics_stop_time - G_STAT_PERCPU[cpu_id].idle_starttime;
                G_STAT_PERCPU[cpu_id].idle_runtime += runtime;
                G_STAT_PERCPU[cpu_id].idle_starttime = 0;
            }

            if (*task_cb).priority < HIGHTASKPRI {
                let runtime = statistics_stop_time - G_STAT_PERCPU[cpu_id].high_task_starttime;
                G_STAT_PERCPU[cpu_id].high_task_runtime += runtime;
                G_STAT_PERCPU[cpu_id].high_task_starttime = 0;
            }
        }

        SCHEDULER_UNLOCK(int_save);
        os_statistics_show(statistics_past_time);

        // 清零统计数据
        memset_s(G_STAT_PERCPU.as_mut_ptr() as *mut u8, 
                core::mem::size_of_val(&G_STAT_PERCPU), 
                0, 
                core::mem::size_of_val(&G_STAT_PERCPU));
        G_STATISTICS_START_TIME = 0;
    }
}
