#[cfg(feature = "ipc_event")]
use crate::event::types::EventCB;
use crate::{
    container_of, offset_of,
    utils::{list::LinkedList, sortlink::SortLinkList},
};
use bitflags::bitflags;
use core::{
    ffi::{c_char, c_void},
    fmt,
};

/// 任务入口函数类型
pub type TaskEntryFunc = Option<extern "C" fn(*mut c_void)>;

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
    #[cfg(feature = "ipc_event")]
    pub event: EventCB,

    /// 事件掩码
    #[cfg(feature = "ipc_event")]
    pub event_mask: u32,

    /// 事件模式
    #[cfg(feature = "ipc_event")]
    pub event_mode: u32,

    /// 分配给队列的内存
    pub msg: *mut c_void,

    /// 优先级位图
    pub priority_bitmap: u32,

    /// 任务信号
    pub signal: TaskSignal,

    /// 剩余时间片
    #[cfg(feature = "time_slice")]
    pub time_slice: u16,
}

// event::types::EventCB,
impl TaskCB {
    pub const UNINIT: Self = Self {
        stack_pointer: core::ptr::null_mut(),
        task_status: TaskStatus::UNUSED,
        priority: 0,
        task_flags: TaskFlags::empty(),
        usr_stack: 0,
        stack_size: 0,
        top_of_stack: core::ptr::null_mut(),
        task_id: 0,
        task_entry: None,
        task_sem: core::ptr::null_mut(),
        #[cfg(feature = "compat_posix")]
        thread_join: core::ptr::null_mut(),
        #[cfg(feature = "compat_posix")]
        thread_join_retval: core::ptr::null_mut(),
        task_mux: core::ptr::null_mut(),
        args: core::ptr::null_mut(),
        task_name: core::ptr::null(),
        pend_list: LinkedList::UNINIT,
        sort_list: SortLinkList::UNINIT,
        #[cfg(feature = "ipc_event")]
        event: EventCB::new(),
        #[cfg(feature = "ipc_event")]
        event_mask: 0,
        #[cfg(feature = "ipc_event")]
        event_mode: 0,
        msg: core::ptr::null_mut(),
        priority_bitmap: 0,
        signal: TaskSignal::empty(),
        #[cfg(feature = "time_slice")]
        time_slice: 0,
    };

    pub fn name(&self) -> &str {
        unsafe {
            core::ffi::CStr::from_ptr(self.task_name)
                .to_str()
                .unwrap_or("unknown")
        }
    }

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

    #[inline]
    pub fn is_system_task(&self) -> bool {
        self.task_flags.contains(TaskFlags::SYSTEM)
    }

    #[inline]
    pub fn set_system_task(&mut self) {
        self.task_flags.insert(TaskFlags::SYSTEM);
    }

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

impl fmt::Display for TaskCB {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "TaskCB {{ id: {}, name: '{}', status: {:?}, priority: {}, flags: {:?}, stack_size: {}, stack_pointer: {:p} }}",
            self.task_id,
            self.name(),
            self.task_status,
            self.priority,
            self.task_flags,
            self.stack_size,
            self.stack_pointer
        )
    }
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

        /// 任务阻塞状态掩码
        const BLOCKED = Self::DELAY.bits() | Self::PEND.bits() | Self::SUSPEND.bits();
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
