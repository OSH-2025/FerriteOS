use crate::{
    container_of,
    event::EventCB,
    offset_of,
    utils::{list::LinkedList, sortlink::SortLinkList},
};
use bitflags::bitflags;
use core::ffi::{c_char, c_void};

/// 任务入口函数类型
pub type TaskEntryFunc = Option<extern "C" fn(*mut c_void) -> *mut c_void>;

/// 任务初始化参数结构体
#[repr(C)]
#[derive(Debug)]
pub struct TaskInitParam {
    /// 任务入口函数
    pub task_entry: TaskEntryFunc,

    /// 任务优先级
    pub priority: u16,

    /// 任务参数
    pub args: *mut c_void,

    /// 任务栈大小
    pub stack_size: u32,

    /// 任务名称
    pub name: *const c_char,

    /// 任务属性标志
    pub task_attr: TaskAttr,
}

impl Default for TaskInitParam {
    fn default() -> Self {
        Self {
            task_entry: None,
            priority: 0,
            args: core::ptr::null_mut(),
            stack_size: 0,
            name: core::ptr::null(),
            task_attr: TaskAttr::empty(),
        }
    }
}

impl TaskInitParam {
    #[inline]
    pub fn is_detached(&self) -> bool {
        self.task_attr.contains(TaskAttr::DETACHED)
    }
}

/// 任务控制块
#[repr(C)]
#[derive(Debug)]
pub struct TaskCB {
    /// 任务栈指针
    pub stack_pointer: *mut c_void,

    /// 任务状态
    pub task_status: TaskStatus,

    /// 任务优先级
    pub priority: u16,

    /// 任务扩展标志
    pub task_flags: TaskFlags,

    /// 用户栈标志
    pub usr_stack: u16,

    /// 任务栈大小
    pub stack_size: u32,

    /// 任务栈顶
    pub top_of_stack: *mut c_void,

    /// 任务ID
    pub task_id: u32,

    /// 任务入口函数
    pub task_entry: TaskEntryFunc,

    /// 任务持有的信号量
    pub task_sem: *mut c_void,

    #[cfg(feature = "compat_posix")]
    pub thread_join: *mut c_void,

    #[cfg(feature = "compat_posix")]
    pub thread_join_retval: *mut c_void,

    /// 任务持有的互斥锁
    pub task_mux: *mut c_void,

    /// 任务参数
    pub args: *mut c_void,

    /// 任务名称
    pub task_name: *const c_char,

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
    pub msg: *mut c_void,

    /// 优先级位图
    pub priority_bitmap: u32,

    /// 任务信号
    pub signal: TaskSignal,

    /// 剩余时间片
    #[cfg(feature = "timeslice")]
    pub time_slice: u16,
}

impl TaskCB {
    #[inline]
    pub fn from_pend_list(ptr: *mut LinkedList) -> &'static mut TaskCB {
        let task_ptr = container_of!(ptr, TaskCB, pend_list);
        unsafe { &mut *task_ptr }
    }

    #[inline]
    pub fn clear_all_flags(&mut self) {
        self.task_flags = TaskFlags::empty();
    }

    // #[inline]
    // pub fn is_detached(&self) -> bool {
    //     self.task_flags.contains(TaskFlags::DETACHED)
    // }

    #[inline]
    pub fn set_detached(&mut self, detached: bool) {
        if detached {
            self.task_flags.insert(TaskFlags::DETACHED);
        } else {
            self.task_flags.remove(TaskFlags::DETACHED);
        }
    }

    // #[inline]
    // pub fn is_system_task(&self) -> bool {
    //     self.task_flags.contains(TaskFlags::SYSTEM)
    // }

    // #[inline]
    // pub fn set_system_task(&mut self, is_system: bool) {
    //     if is_system {
    //         self.task_flags.insert(TaskFlags::SYSTEM);
    //     } else {
    //         self.task_flags.remove(TaskFlags::SYSTEM);
    //     }
    // }

    // #[inline]
    // pub fn set_signal(&mut self, signal: TaskSignal) {
    //     self.signal.insert(signal);
    // }

    // #[inline]
    // pub fn clear_signal(&mut self, signal: TaskSignal) {
    //     self.signal.remove(signal);
    // }

    #[inline]
    pub fn clear_all_signals(&mut self) {
        self.signal = TaskSignal::empty();
    }

    // #[inline]
    // pub fn has_kill_signal(&self) -> bool {
    //     self.signal.contains(TaskSignal::KILL)
    // }

    // #[inline]
    // pub fn has_suspend_signal(&self) -> bool {
    //     self.signal.contains(TaskSignal::SUSPEND)
    // }

    // #[inline]
    // pub fn signal_kill(&mut self) {
    //     self.signal.insert(TaskSignal::KILL);
    // }

    // #[inline]
    // pub fn signal_suspend(&mut self) {
    //     self.signal.insert(TaskSignal::SUSPEND);
    // }
}

bitflags! {
    #[derive(Debug, Copy, Clone, Eq, PartialEq)]
    #[repr(transparent)]
    pub struct TaskStatus: u16 {
        const UNUSED = 0x0001;    // 任务控制块未使用
        const SUSPEND = 0x0002;   // 任务被挂起
        const READY = 0x0004;     // 任务就绪
        const PEND = 0x0008;      // 任务阻塞
        const RUNNING = 0x0010;   // 任务运行中
        const DELAY = 0x0020;     // 任务延时
        const TIMEOUT = 0x0040;   // 等待事件超时
        const PEND_TIME = 0x0080; // 任务等待特定时间
    }
}

bitflags! {
    #[derive(Debug, Copy, Clone, Eq, PartialEq)]
    #[repr(transparent)]
    pub struct TaskFlags: u16 {
        /// 任务自动删除标志
        const DETACHED = 0x0001;
        /// 系统级任务标志
        const SYSTEM = 0x0002;
    }
}

bitflags! {
    #[derive(Debug, Copy, Clone, Eq, PartialEq)]
    #[repr(transparent)]
    pub struct TaskAttr: u32 {
        /// 任务属性：分离
        const DETACHED = 0x0100;
    }
}

bitflags! {
    /// 任务信号类型
    #[derive(Debug, Copy, Clone, Eq, PartialEq)]
    #[repr(transparent)]
    pub struct TaskSignal: u32 {
        /// 杀死任务信号
        const KILL = 1;
        /// 挂起任务信号
        const SUSPEND = 2;
    }
}

impl From<u32> for TaskAttr {
    fn from(value: u32) -> Self {
        Self::from_bits_truncate(value)
    }
}
