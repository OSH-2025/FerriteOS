pub type SystemResult<T> = Result<T, SystemError>;

/// 系统级通用错误类型
#[derive(Debug, Clone, PartialEq)]
pub enum SystemError {
    /// 任务相关错误
    Task(TaskError),
    /// 内存管理错误
    Memory(MemoryError),
    /// 中断相关错误
    Interrupt(InterruptError),
}

/// 任务管理错误
#[derive(Debug, Clone, PartialEq)]
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

/// 内存管理错误
#[derive(Debug, Clone, PartialEq)]
pub enum MemoryError {
    OutOfMemory,
    InvalidAddress,
    AlignmentError,
    PermissionDenied,
    FragmentationError,
    PoolFull,
    InvalidSize,
}

/// 中断管理错误
#[derive(Debug, Clone, PartialEq)]
pub enum InterruptError {
    InvalidNumber,
    AlreadyRegistered,
    NotRegistered,
    PriorityInvalid,
    HandlerNull,
    ControllerNotReady,
}

impl From<TaskError> for SystemError {
    fn from(err: TaskError) -> Self {
        SystemError::Task(err)
    }
}

impl From<MemoryError> for SystemError {
    fn from(err: MemoryError) -> Self {
        SystemError::Memory(err)
    }
}

impl From<InterruptError> for SystemError {
    fn from(err: InterruptError) -> Self {
        SystemError::Interrupt(err)
    }
}

impl From<SystemError> for u32 {
    fn from(error: SystemError) -> Self {
        match error {
            SystemError::Task(err) => u32::from(err),
            SystemError::Memory(_err) => todo!(),
            SystemError::Interrupt(_err) => todo!(),
        }
    }
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

pub const ERRNO_TSK_NO_MEMORY: u32 = 0x03000200;
pub const ERRNO_TSK_PTR_NULL: u32 = 0x02000201;
pub const ERRNO_TSK_STKSZ_NOT_ALIGN: u32 = 0x02000202;
pub const ERRNO_TSK_PRIOR_ERROR: u32 = 0x02000203;
pub const ERRNO_TSK_ENTRY_NULL: u32 = 0x02000204;
pub const ERRNO_TSK_NAME_EMPTY: u32 = 0x02000205;
pub const ERRNO_TSK_STKSZ_TOO_SMALL: u32 = 0x02000206;
pub const ERRNO_TSK_ID_INVALID: u32 = 0x02000207;
pub const ERRNO_TSK_ALREADY_SUSPENDED: u32 = 0x02000208;
pub const ERRNO_TSK_NOT_SUSPENDED: u32 = 0x02000209;
pub const ERRNO_TSK_NOT_CREATED: u32 = 0x0200020a;
pub const ERRNO_TSK_DELETE_LOCKED: u32 = 0x0300020b;
pub const ERRNO_TSK_DELAY_IN_INT: u32 = 0x0200020d;
pub const ERRNO_TSK_DELAY_IN_LOCK: u32 = 0x0200020e;
pub const ERRNO_TSK_YIELD_IN_LOCK: u32 = 0x0200020f;
pub const ERRNO_TSK_YIELD_NOT_ENOUGH_TASK: u32 = 0x02000210;
pub const ERRNO_TSK_TCB_UNAVAILABLE: u32 = 0x02000211;
pub const ERRNO_TSK_OPERATE_SYSTEM_TASK: u32 = 0x02000214;
pub const ERRNO_TSK_SUSPEND_LOCKED: u32 = 0x03000215;
pub const ERRNO_TSK_STKSZ_TOO_LARGE: u32 = 0x02000220;
pub const ERRNO_TSK_YIELD_IN_INT: u32 = 0x02000224;
