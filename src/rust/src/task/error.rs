/// 任务管理错误
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum TaskError {
    /// 任务ID指针无效
    InvalidId,
    /// 参数指针为空
    ParamNull,
    /// 任务名称为空
    NameEmpty,
    /// 任务入口函数为空
    EntryNull,
    /// 任务优先级错误
    PriorityError,
    /// 栈大小过大
    StackSizeTooLarge,
    /// 栈大小过小
    StackSizeTooSmall,
    /// 内存不足
    OutOfMemory,
    /// 没有可用的空闲任务
    NoFreeTasks,
    /// 栈未对齐
    StackNotAligned,
    /// 任务被锁定无法删除
    DeleteLocked,
    /// 尝试操作系统任务
    OperateSystemTask,
    /// 任务未创建
    NotCreated,
    /// 任务未被挂起
    NotSuspended,
    /// 任务已经被挂起
    AlreadySuspended,
    /// 任务被锁定无法挂起
    SuspendLocked,
    /// 在中断中尝试延时
    DelayInInterrupt,
    /// 在锁定状态下尝试延时
    DelayInLock,
    /// 在中断中尝试让出CPU
    YieldInInterrupt,
    /// 在锁定状态下尝试让出CPU
    YieldInLock,
    /// 没有足够的同优先级任务进行让出操作
    YieldNotEnoughTask,
}

/// 将TaskError转换为错误码
impl From<TaskError> for u32 {
    fn from(error: TaskError) -> Self {
        match error {
            TaskError::InvalidId => ERRNO_TSK_ID_INVALID,
            TaskError::ParamNull => ERRNO_TSK_PTR_NULL,
            TaskError::NameEmpty => ERRNO_TSK_NAME_EMPTY,
            TaskError::EntryNull => ERRNO_TSK_ENTRY_NULL,
            TaskError::PriorityError => ERRNO_TSK_PRIOR_ERROR,
            TaskError::StackSizeTooLarge => ERRNO_TSK_STKSZ_TOO_LARGE,
            TaskError::StackSizeTooSmall => ERRNO_TSK_STKSZ_TOO_SMALL,
            TaskError::OutOfMemory => ERRNO_TSK_NO_MEMORY,
            TaskError::NoFreeTasks => ERRNO_TSK_TCB_UNAVAILABLE,
            TaskError::StackNotAligned => ERRNO_TSK_STKSZ_NOT_ALIGN,
            TaskError::DeleteLocked => ERRNO_TSK_DELETE_LOCKED,
            TaskError::OperateSystemTask => ERRNO_TSK_OPERATE_SYSTEM_TASK,
            TaskError::NotCreated => ERRNO_TSK_NOT_CREATED,
            TaskError::NotSuspended => ERRNO_TSK_NOT_SUSPENDED,
            TaskError::AlreadySuspended => ERRNO_TSK_ALREADY_SUSPENDED,
            TaskError::SuspendLocked => ERRNO_TSK_SUSPEND_LOCKED,
            TaskError::DelayInInterrupt => ERRNO_TSK_DELAY_IN_INT,
            TaskError::DelayInLock => ERRNO_TSK_DELAY_IN_LOCK,
            TaskError::YieldInInterrupt => ERRNO_TSK_YIELD_IN_INT,
            TaskError::YieldInLock => ERRNO_TSK_YIELD_IN_LOCK,
            TaskError::YieldNotEnoughTask => ERRNO_TSK_YIELD_NOT_ENOUGH_TASK,
        }
    }
}

const ERRNO_TSK_NO_MEMORY: u32 = 0x03000200;
const ERRNO_TSK_PTR_NULL: u32 = 0x02000201;
const ERRNO_TSK_STKSZ_NOT_ALIGN: u32 = 0x02000202;
const ERRNO_TSK_PRIOR_ERROR: u32 = 0x02000203;
const ERRNO_TSK_ENTRY_NULL: u32 = 0x02000204;
const ERRNO_TSK_NAME_EMPTY: u32 = 0x02000205;
const ERRNO_TSK_STKSZ_TOO_SMALL: u32 = 0x02000206;
const ERRNO_TSK_ID_INVALID: u32 = 0x02000207;
const ERRNO_TSK_ALREADY_SUSPENDED: u32 = 0x02000208;
const ERRNO_TSK_NOT_SUSPENDED: u32 = 0x02000209;
const ERRNO_TSK_NOT_CREATED: u32 = 0x0200020a;
const ERRNO_TSK_DELETE_LOCKED: u32 = 0x0300020b;
const ERRNO_TSK_DELAY_IN_INT: u32 = 0x0200020d;
const ERRNO_TSK_DELAY_IN_LOCK: u32 = 0x0200020e;
const ERRNO_TSK_YIELD_IN_LOCK: u32 = 0x0200020f;
const ERRNO_TSK_YIELD_NOT_ENOUGH_TASK: u32 = 0x02000210;
const ERRNO_TSK_TCB_UNAVAILABLE: u32 = 0x02000211;
const ERRNO_TSK_OPERATE_SYSTEM_TASK: u32 = 0x02000214;
const ERRNO_TSK_SUSPEND_LOCKED: u32 = 0x03000215;
const ERRNO_TSK_STKSZ_TOO_LARGE: u32 = 0x02000220;
const ERRNO_TSK_YIELD_IN_INT: u32 = 0x02000224;

/// 从u32错误码转换为TaskError
impl TryFrom<u32> for TaskError {
    type Error = ();

    fn try_from(errno: u32) -> Result<Self, Self::Error> {
        match errno {
            ERRNO_TSK_ID_INVALID => Ok(TaskError::InvalidId),
            ERRNO_TSK_PTR_NULL => Ok(TaskError::ParamNull),
            ERRNO_TSK_NAME_EMPTY => Ok(TaskError::NameEmpty),
            ERRNO_TSK_ENTRY_NULL => Ok(TaskError::EntryNull),
            ERRNO_TSK_PRIOR_ERROR => Ok(TaskError::PriorityError),
            ERRNO_TSK_STKSZ_TOO_LARGE => Ok(TaskError::StackSizeTooLarge),
            ERRNO_TSK_STKSZ_TOO_SMALL => Ok(TaskError::StackSizeTooSmall),
            ERRNO_TSK_NO_MEMORY => Ok(TaskError::OutOfMemory),
            ERRNO_TSK_TCB_UNAVAILABLE => Ok(TaskError::NoFreeTasks),
            ERRNO_TSK_STKSZ_NOT_ALIGN => Ok(TaskError::StackNotAligned),
            ERRNO_TSK_DELETE_LOCKED => Ok(TaskError::DeleteLocked),
            ERRNO_TSK_OPERATE_SYSTEM_TASK => Ok(TaskError::OperateSystemTask),
            ERRNO_TSK_NOT_CREATED => Ok(TaskError::NotCreated),
            ERRNO_TSK_NOT_SUSPENDED => Ok(TaskError::NotSuspended),
            ERRNO_TSK_ALREADY_SUSPENDED => Ok(TaskError::AlreadySuspended),
            ERRNO_TSK_SUSPEND_LOCKED => Ok(TaskError::SuspendLocked),
            ERRNO_TSK_DELAY_IN_INT => Ok(TaskError::DelayInInterrupt),
            ERRNO_TSK_DELAY_IN_LOCK => Ok(TaskError::DelayInLock),
            ERRNO_TSK_YIELD_IN_INT => Ok(TaskError::YieldInInterrupt),
            ERRNO_TSK_YIELD_IN_LOCK => Ok(TaskError::YieldInLock),
            ERRNO_TSK_YIELD_NOT_ENOUGH_TASK => Ok(TaskError::YieldNotEnoughTask),
            _ => Err(()),
        }
    }
}

impl core::fmt::Display for TaskError {
    fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
        match self {
            TaskError::InvalidId => write!(f, "Invalid task ID"),
            TaskError::ParamNull => write!(f, "Parameter pointer is null"),
            TaskError::NameEmpty => write!(f, "Task name is empty"),
            TaskError::EntryNull => write!(f, "Task entry function is null"),
            TaskError::PriorityError => write!(f, "Task priority error"),
            TaskError::StackSizeTooLarge => write!(f, "Stack size too large"),
            TaskError::StackSizeTooSmall => write!(f, "Stack size too small"),
            TaskError::OutOfMemory => write!(f, "Out of memory for task"),
            TaskError::NoFreeTasks => write!(f, "No free tasks available"),
            TaskError::StackNotAligned => write!(f, "Task stack not aligned"),
            TaskError::DeleteLocked => write!(f, "Task delete locked"),
            TaskError::OperateSystemTask => write!(f, "Cannot operate system task"),
            TaskError::NotCreated => write!(f, "Task not created"),
            TaskError::NotSuspended => write!(f, "Task not suspended"),
            TaskError::AlreadySuspended => write!(f, "Task already suspended"),
            TaskError::SuspendLocked => write!(f, "Task suspend locked"),
            TaskError::DelayInInterrupt => write!(f, "Delay in interrupt context"),
            TaskError::DelayInLock => write!(f, "Delay in lock context"),
            TaskError::YieldInInterrupt => write!(f, "Yield in interrupt context"),
            TaskError::YieldInLock => write!(f, "Yield in lock context"),
            TaskError::YieldNotEnoughTask => write!(f, "Not enough tasks to yield"),
        }
    }
}
