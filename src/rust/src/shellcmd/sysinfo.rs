//! 系统资源信息命令实现

use crate::print_common;
use crate::shellcmd::types::{CmdType, ShellCmd};
use core::ffi::c_char;

// 导入必要的系统资源相关类型和函数
use crate::task::types::{TaskCB,
                         TaskStatus};
use crate::ffi::bindings::{arch_int_lock as los_int_lock, arch_int_restore as los_int_restore};

#[cfg(feature = "base_ipc_sem")]
use crate::semaphore::types::{SemaphoreControlBlock as LosSemCB,
                               SemaphoreState};

#[cfg(feature = "base_ipc_mux")]
use crate::mutex::types::{MutexControlBlock as LosMuxCB,
                          MutexState};

#[cfg(feature = "base_ipc_queue")]
use crate::queue::types::{QueueControlBlock as LosQueueCB,
                          QueueState};

#[cfg(feature = "base_core_swtmr")]
use crate::swtmr::LosSwtmrCB;

// 常量定义
const OS_ERROR: u32 = u32::MAX; // 等同于 C 中的 (UINT32)(-1)
const LOSCFG_BASE_CORE_TSK_LIMIT: u32 = 64; 
const KERNEL_TSK_LIMIT: u32 = LOSCFG_BASE_CORE_TSK_LIMIT;

#[cfg(feature = "base_ipc_sem")]
const LOSCFG_BASE_IPC_SEM_LIMIT: u32 = 1024; 

#[cfg(feature = "base_ipc_mux")]
const LOSCFG_BASE_IPC_MUX_LIMIT: u32 = 1024; 

#[cfg(feature = "base_ipc_queue")]
const LOSCFG_BASE_IPC_QUEUE_LIMIT: u32 = 1024; 

#[cfg(feature = "base_core_swtmr")]
const LOSCFG_BASE_CORE_SWTMR_LIMIT: u32 = 1024; 

#[cfg(feature = "base_ipc_sem")]
const SEM_SPLIT_BIT: u32 = 16;

#[cfg(feature = "base_ipc_mux")]
const MUX_SPLIT_BIT: u32 = 16;

// 状态常量
const OS_TASK_STATUS_UNUSED: TaskStatus = TaskStatus::UNUSED;
const OS_SWTMR_STATUS_UNUSED: u8 = 0;

/// 获取活动任务数量
pub fn os_shell_cmd_task_cnt_get() -> u32 {
    let mut task_cnt = 0;
    
    unsafe {
        // 关中断
        let int_save = los_int_lock();
        
        // 遍历任务控制块数组
        for loop_idx in 0..KERNEL_TSK_LIMIT {
            let task_cb = (g_taskCBArray as *mut TaskCB).offset(loop_idx as isize);
            if (*task_cb).task_status != OS_TASK_STATUS_UNUSED {
                continue;
            }
            task_cnt += 1;
        }
        
        // 开中断
        los_int_restore(int_save);
    }
    
    task_cnt
}

#[cfg(feature = "base_ipc_sem")]
#[inline]
unsafe fn get_sem_index(sem_id: u32) -> u32 {
    sem_id & ((1u32 << SEM_SPLIT_BIT) - 1)
}

#[cfg(feature = "base_ipc_sem")]
#[inline]
unsafe fn get_sem(sem_id: u32) -> *mut LosSemCB {
    g_allSem.offset(get_sem_index(sem_id) as isize)
}

/// 获取信号量数量
#[cfg(feature = "base_ipc_sem")]
pub fn os_shell_cmd_sem_cnt_get() -> u32 {
    let mut sem_cnt = 0;
    
    unsafe {
        // 关中断
        let int_save = los_int_lock();
        
        // 遍历信号量控制块
        for loop_idx in 0..LOSCFG_BASE_IPC_SEM_LIMIT {
            let sem_node = get_sem(loop_idx);
            if (*sem_node).sem_stat == SemaphoreState::Used {
                sem_cnt += 1;
            }
        }
        
        // 开中断
        los_int_restore(int_save);
    }
    
    sem_cnt
}

#[cfg(feature = "base_ipc_mux")]
#[inline]
unsafe fn get_mux_index(mux_id: u32) -> u32 {
    mux_id & ((1u32 << MUX_SPLIT_BIT) - 1)
}

#[cfg(feature = "base_ipc_mux")]
#[inline]
unsafe fn get_mux(mux_id: u32) -> *mut LosMuxCB {
    g_allMux.offset(get_mux_index(mux_id) as isize)
}

/// 获取互斥锁数量
#[cfg(feature = "base_ipc_mux")]
pub fn os_shell_cmd_mux_cnt_get() -> u32 {
    let mut mux_cnt = 0;
    
    unsafe {
        // 关中断
        let int_save = los_int_lock();
        
        // 遍历互斥锁控制块
        for loop_idx in 0..LOSCFG_BASE_IPC_MUX_LIMIT {

            let mux_node = get_mux(loop_idx);
            if (*mux_node).mux_stat == MutexState::Used {
                mux_cnt += 1;
            }
        }
        
        // 开中断
        los_int_restore(int_save);
    }
    
    mux_cnt
}

/// 获取队列数量
#[cfg(feature = "base_ipc_queue")]
pub fn os_shell_cmd_queue_cnt_get() -> u32 {
    let mut queue_cnt = 0;
    
    unsafe {
        // 关中断
        let int_save = los_int_lock();
        
        // 遍历队列控制块
        let mut queue_cb = g_allQueue;
        for _ in 0..LOSCFG_BASE_IPC_QUEUE_LIMIT {

            if (*queue_cb).queue_state == QueueState::Used {
                queue_cnt += 1;
            }
            queue_cb = queue_cb.offset(1);
        }
        
        // 开中断
        los_int_restore(int_save);
    }
    
    queue_cnt
}

/// 获取软件定时器数量
#[cfg(feature = "base_core_swtmr")]
pub fn os_shell_cmd_swtmr_cnt_get() -> u32 {
    let mut swtmr_cnt = 0;
    
    unsafe {
        // 关中断
        let int_save = los_int_lock();
        
        // 遍历软件定时器控制块
        let mut swtmr_cb = g_swtmrCBArray;
        for _ in 0..LOSCFG_BASE_CORE_SWTMR_LIMIT {
            if (*swtmr_cb).state != OS_SWTMR_STATUS_UNUSED {
                swtmr_cnt += 1;
            }
            swtmr_cb = swtmr_cb.offset(1);
        }
        
        // 开中断
        los_int_restore(int_save);
    }
    
    swtmr_cnt
}

/// 获取并打印系统信息
pub fn os_shell_cmd_system_info_get() {
    print_common!("\n   Module    Used      Total\n");
    print_common!("--------------------------------\n");
    
    // 任务信息
    print_common!("   Task      {:<10}{:<10}\n",
        os_shell_cmd_task_cnt_get(),
        LOSCFG_BASE_CORE_TSK_LIMIT);
    
    // 信号量信息
    #[cfg(feature = "base_ipc_sem")]
    print_common!("   Sem       {:<10}{:<10}\n",
        os_shell_cmd_sem_cnt_get(),
        LOSCFG_BASE_IPC_SEM_LIMIT);
    
    // 互斥锁信息
    #[cfg(feature = "base_ipc_mux")]
    print_common!("   Mutex     {:<10}{:<10}\n",
        os_shell_cmd_mux_cnt_get(),
        LOSCFG_BASE_IPC_MUX_LIMIT);
    
    // 队列信息
    #[cfg(feature = "base_ipc_queue")]
    print_common!("   Queue     {:<10}{:<10}\n",
        os_shell_cmd_queue_cnt_get(),
        LOSCFG_BASE_IPC_QUEUE_LIMIT);
    
    // 软件定时器信息
    #[cfg(feature = "base_core_swtmr")]
    print_common!("   SwTmr     {:<10}{:<10}\n",
        os_shell_cmd_swtmr_cnt_get(),
        LOSCFG_BASE_CORE_SWTMR_LIMIT);
}

/// 系统信息命令实现
pub fn cmd_systeminfo(argc: i32, argv: *const *const u8) -> u32 {
    if argc == 0 {
        os_shell_cmd_system_info_get();
        return 0;
    }
    
    // 如果有不支持的参数，打印错误信息
    let arg = unsafe { 
        if !argv.is_null() && !(*argv).is_null() {
            core::ffi::CStr::from_ptr(*argv as *const u8)
                .to_str()
                .unwrap_or("unknown")
        } else {
            "unknown"
        }
    };
    
    print_common!("systeminfo: invalid option {}\n\
                  Systeminfo has NO ARGS.\n", arg);
    OS_ERROR
}

/// 命令入口函数
#[unsafe(no_mangle)]
pub unsafe extern "C" fn rust_systeminfo_cmd(argc: i32, argv: *const *const u8) -> u32 {
    cmd_systeminfo(argc, argv)
}


// 注册systeminfo命令
#[unsafe(no_mangle)]  // 防止编译器修改符号名
#[used]
#[unsafe(link_section = ".liteos.table.shellcmd.data")]
pub static systeminfo_shellcmd: ShellCmd = ShellCmd {
    cmd_type: CmdType::Ex,
    cmd_key: b"systeminfo\0".as_ptr() as *const c_char,
    para_num: 1,
    cmd_hook: rust_systeminfo_cmd,
};

// 外部符号引用
unsafe extern "C" {
    static g_taskCBArray: *const TaskCB;
    
    #[cfg(feature = "base_ipc_sem")]
    static mut g_allSem: *mut LosSemCB;
    
    #[cfg(feature = "base_ipc_mux")]
    static mut g_allMux: *mut LosMuxCB;
    
    #[cfg(feature = "base_ipc_queue")]
    static g_allQueue: *mut LosQueueCB;
    
    #[cfg(feature = "base_core_swtmr")]
    static g_swtmrCBArray: *mut LosSwtmrCB;
}