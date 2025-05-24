use crate::event::EventCB;
use crate::utils::list::LinkedList;
use crate::utils::sortlink::SortLinkList;

/// 任务入口函数类型
pub type TaskEntryFunc =
    Option<extern "C" fn(param: *mut core::ffi::c_void) -> *mut core::ffi::c_void>;

/// 任务初始化参数结构体
#[repr(C)]
pub struct TaskInitParam {
    /// 任务入口函数
    pub pfn_task_entry: TaskEntryFunc,

    /// 任务优先级
    pub task_prio: u16,

    /// 任务参数
    pub p_args: *mut core::ffi::c_void,

    /// 任务栈大小
    pub stack_size: u32,

    /// 任务名称
    pub name: *const u8,

    /// 保留字段，用于指定任务是否自动删除
    pub resved: u32,
}

impl Default for TaskInitParam {
    fn default() -> Self {
        Self {
            pfn_task_entry: None,
            task_prio: 0,
            p_args: core::ptr::null_mut(),
            stack_size: 0,
            name: core::ptr::null(),
            resved: 0,
        }
    }
}

/// 任务控制块
#[repr(C)]
pub struct TaskCB {
    /// 任务栈指针
    pub stack_pointer: *mut core::ffi::c_void,

    /// 任务状态
    pub task_status: u16,

    /// 任务优先级
    pub priority: u16,

    /// 任务扩展标志
    pub task_flags: u16,

    /// 用户栈标志
    pub usr_stack: u16,

    /// 任务栈大小
    pub stack_size: u32,

    /// 任务栈顶
    pub top_of_stack: usize,

    /// 任务ID
    pub task_id: u32,

    /// 任务入口函数
    pub task_entry: TaskEntryFunc,

    /// 任务持有的信号量
    pub task_sem: *mut core::ffi::c_void,

    #[cfg(feature = "compat_posix")]
    pub thread_join: *mut core::ffi::c_void,

    #[cfg(feature = "compat_posix")]
    pub thread_join_retval: *mut core::ffi::c_void,

    /// 任务持有的互斥锁
    pub task_mux: *mut core::ffi::c_void,

    /// 任务参数
    pub args: *mut core::ffi::c_void,

    /// 任务名称
    pub task_name: *mut i8,

    /// 任务挂起节点
    pub pend_list: LinkedList,

    /// 任务排序链表节点
    pub sort_list: SortLinkList,

    /// 事件控制块
    pub event: EventCB,

    /// 事件掩码
    pub event_mask: u32,

    /// 事件模式
    pub event_mode: u32,

    /// 分配给队列的内存
    pub msg: *mut core::ffi::c_void,

    /// 优先级位图，用于记录任务优先级的变化，优先级不能大于31
    pub pri_bit_map: u32,

    /// 任务信号
    pub signal: u32,

    /// 剩余时间片
    #[cfg(feature = "timeslice")]
    pub time_slice: u16,
}

