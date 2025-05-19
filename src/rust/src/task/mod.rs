//! 任务相关定义

use core::ffi::c_void;  // 添加这一行导入c_void类型

/// 排序链表节点结构体
#[repr(C)]
pub struct SortLinkList {
    pub sort_link_node: crate::event::LOS_DL_LIST,
    pub idx_roll_num: u32,
}

#[repr(C)]
pub struct LosTaskCB {
    pub stack_pointer: *mut c_void,     /* Task stack pointer */
    pub task_status: u16,               /* Task status */
    pub priority: u16,                  /* Task priority */
    pub task_flags_usr_stack: u32,      /* Task flags和usr_stack合并为一个字段 */
    pub stack_size: u32,                /* Task stack size */
    pub top_of_stack: usize,            /* Task stack top */
    pub task_id: u32,                   /* Task ID */
    pub task_entry: extern "C" fn(*mut c_void),  /* Task entrance function */
    pub task_sem: *mut c_void,          /* Task-held semaphore */
    
    #[cfg(feature = "compat_posix")]
    pub thread_join: *mut c_void,       /* pthread adaption */
    #[cfg(feature = "compat_posix")]
    pub thread_join_retval: *mut c_void,/* pthread adaption */
    
    pub task_mux: *mut c_void,          /* Task-held mutex */
    
    #[cfg(feature = "obsolete_api")]
    pub args: [usize; 4],               /* Parameter array */
    #[cfg(not(feature = "obsolete_api"))]
    pub args: *mut c_void,              /* Single parameter */
    
    pub task_name: *mut i8,             /* Task name */
    pub pend_list: crate::event::LOS_DL_LIST,  /* Task pend node */
    pub sort_list: SortLinkList,        /* Task sortlink node */
    
    #[cfg(feature = "base_ipc_event")]
    pub event: EVENT_CB_S,              /* Event control block */
    pub event_mask: u32,                /* Event mask */
    pub event_mode: u32,                /* Event mode */
    pub event_result: u32,              /* 新增字段：保存匹配的事件结果 */
    
    pub msg: *mut c_void,               /* Memory allocated to queues */
    pub pri_bit_map: u32,               /* BitMap for priority changes */
    pub signal: u32,                    /* Task signal */
    
    // 根据编译条件添加其他字段...
    #[cfg(feature = "base_core_timeslice")]
    pub time_slice: u16,                /* Remaining time slice */
    
    #[cfg(feature = "kernel_smp")]
    pub curr_cpu: u16,                 /* Current CPU core number */
    #[cfg(feature = "kernel_smp")]
    pub last_cpu: u16,                 /* Last CPU core number */
    #[cfg(feature = "kernel_smp")]
    pub timer_cpu: u32,                /* Timer CPU core number */
    #[cfg(feature = "kernel_smp")]
    pub cpu_affi_mask: u16,            /* CPU affinity mask */
}