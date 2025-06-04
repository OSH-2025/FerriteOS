//! 消息队列错误定义

/// 消息队列操作错误
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u32)]
pub enum QueueError {
    /// 内存分配失败，无法创建队列
    CreateNoMemory,
    /// 队列最大消息尺寸过大
    SizeTooBig,
    /// 队列控制块不可用（超出系统队列数量上限）
    CbUnavailable,
    /// 队列未找到（无效的队列ID）
    NotFound,
    /// 在调度锁定状态下等待队列
    PendInLock,
    /// 队列操作超时
    Timeout,
    /// 队列被任务使用中，无法删除
    InTaskUse,
    /// 在中断中写入队列但设置了超时
    WriteInInterrupt,
    /// 队列未创建
    NotCreate,
    /// 队列读写不同步
    InTaskWrite,
    /// 创建队列时传入空指针
    CreatePtrNull,
    /// 创建队列时参数为零
    ParaIsZero,
    /// 无效的队列句柄
    Invalid,
    /// 读取队列时传入空指针
    ReadPtrNull,
    /// 读取队列时缓冲区大小无效
    ReadSizeInvalid,
    /// 写入队列时传入空指针
    WritePtrNull,
    /// 写入队列时缓冲区大小为零
    WriteSizeIsZero,
    /// 写入队列时缓冲区大小超过队列节点大小
    WriteSizeTooBig,
    /// 队列已满
    IsFull,
    /// 获取队列信息时传入空指针
    PtrNull,
    /// 在中断中读取队列但设置了超时
    ReadInInterrupt,
    /// 队列为空
    IsEmpty,
    /// 读取队列时缓冲区大小过小
    ReadSizeTooSmall,
}

// 错误码常量定义
const ERRNO_QUEUE_CREATE_NO_MEMORY: u32 = 0x02000602;
const ERRNO_QUEUE_SIZE_TOO_BIG: u32 = 0x02000603;
const ERRNO_QUEUE_CB_UNAVAILABLE: u32 = 0x02000604;
const ERRNO_QUEUE_NOT_FOUND: u32 = 0x02000605;
const ERRNO_QUEUE_PEND_IN_LOCK: u32 = 0x02000606;
const ERRNO_QUEUE_TIMEOUT: u32 = 0x02000607;
const ERRNO_QUEUE_IN_TSKUSE: u32 = 0x02000608;
const ERRNO_QUEUE_WRITE_IN_INTERRUPT: u32 = 0x02000609;
const ERRNO_QUEUE_NOT_CREATE: u32 = 0x0200060a;
const ERRNO_QUEUE_IN_TSKWRITE: u32 = 0x0200060b;
const ERRNO_QUEUE_CREAT_PTR_NULL: u32 = 0x0200060c;
const ERRNO_QUEUE_PARA_ISZERO: u32 = 0x0200060d;
const ERRNO_QUEUE_INVALID: u32 = 0x0200060e;
const ERRNO_QUEUE_READ_PTR_NULL: u32 = 0x0200060f;
const ERRNO_QUEUE_READSIZE_IS_INVALID: u32 = 0x02000610;
const ERRNO_QUEUE_WRITE_PTR_NULL: u32 = 0x02000612;
const ERRNO_QUEUE_WRITESIZE_ISZERO: u32 = 0x02000613;
const ERRNO_QUEUE_WRITE_SIZE_TOO_BIG: u32 = 0x02000615;
const ERRNO_QUEUE_ISFULL: u32 = 0x02000616;
const ERRNO_QUEUE_PTR_NULL: u32 = 0x02000617;
const ERRNO_QUEUE_READ_IN_INTERRUPT: u32 = 0x02000618;
const ERRNO_QUEUE_ISEMPTY: u32 = 0x0200061d;
const ERRNO_QUEUE_READ_SIZE_TOO_SMALL: u32 = 0x0200061f;

impl From<QueueError> for u32 {
    fn from(err: QueueError) -> u32 {
        match err {
            QueueError::CreateNoMemory => ERRNO_QUEUE_CREATE_NO_MEMORY,
            QueueError::SizeTooBig => ERRNO_QUEUE_SIZE_TOO_BIG,
            QueueError::CbUnavailable => ERRNO_QUEUE_CB_UNAVAILABLE,
            QueueError::NotFound => ERRNO_QUEUE_NOT_FOUND,
            QueueError::PendInLock => ERRNO_QUEUE_PEND_IN_LOCK,
            QueueError::Timeout => ERRNO_QUEUE_TIMEOUT,
            QueueError::InTaskUse => ERRNO_QUEUE_IN_TSKUSE,
            QueueError::WriteInInterrupt => ERRNO_QUEUE_WRITE_IN_INTERRUPT,
            QueueError::NotCreate => ERRNO_QUEUE_NOT_CREATE,
            QueueError::InTaskWrite => ERRNO_QUEUE_IN_TSKWRITE,
            QueueError::CreatePtrNull => ERRNO_QUEUE_CREAT_PTR_NULL,
            QueueError::ParaIsZero => ERRNO_QUEUE_PARA_ISZERO,
            QueueError::Invalid => ERRNO_QUEUE_INVALID,
            QueueError::ReadPtrNull => ERRNO_QUEUE_READ_PTR_NULL,
            QueueError::ReadSizeInvalid => ERRNO_QUEUE_READSIZE_IS_INVALID,
            QueueError::WritePtrNull => ERRNO_QUEUE_WRITE_PTR_NULL,
            QueueError::WriteSizeIsZero => ERRNO_QUEUE_WRITESIZE_ISZERO,
            QueueError::WriteSizeTooBig => ERRNO_QUEUE_WRITE_SIZE_TOO_BIG,
            QueueError::IsFull => ERRNO_QUEUE_ISFULL,
            QueueError::PtrNull => ERRNO_QUEUE_PTR_NULL,
            QueueError::ReadInInterrupt => ERRNO_QUEUE_READ_IN_INTERRUPT,
            QueueError::IsEmpty => ERRNO_QUEUE_ISEMPTY,
            QueueError::ReadSizeTooSmall => ERRNO_QUEUE_READ_SIZE_TOO_SMALL,
        }
    }
}

impl TryFrom<u32> for QueueError {
    type Error = ();

    fn try_from(errno: u32) -> Result<Self, Self::Error> {
        match errno {
            ERRNO_QUEUE_CREATE_NO_MEMORY => Ok(QueueError::CreateNoMemory),
            ERRNO_QUEUE_SIZE_TOO_BIG => Ok(QueueError::SizeTooBig),
            ERRNO_QUEUE_CB_UNAVAILABLE => Ok(QueueError::CbUnavailable),
            ERRNO_QUEUE_NOT_FOUND => Ok(QueueError::NotFound),
            ERRNO_QUEUE_PEND_IN_LOCK => Ok(QueueError::PendInLock),
            ERRNO_QUEUE_TIMEOUT => Ok(QueueError::Timeout),
            ERRNO_QUEUE_IN_TSKUSE => Ok(QueueError::InTaskUse),
            ERRNO_QUEUE_WRITE_IN_INTERRUPT => Ok(QueueError::WriteInInterrupt),
            ERRNO_QUEUE_NOT_CREATE => Ok(QueueError::NotCreate),
            ERRNO_QUEUE_IN_TSKWRITE => Ok(QueueError::InTaskWrite),
            ERRNO_QUEUE_CREAT_PTR_NULL => Ok(QueueError::CreatePtrNull),
            ERRNO_QUEUE_PARA_ISZERO => Ok(QueueError::ParaIsZero),
            ERRNO_QUEUE_INVALID => Ok(QueueError::Invalid),
            ERRNO_QUEUE_READ_PTR_NULL => Ok(QueueError::ReadPtrNull),
            ERRNO_QUEUE_READSIZE_IS_INVALID => Ok(QueueError::ReadSizeInvalid),
            ERRNO_QUEUE_WRITE_PTR_NULL => Ok(QueueError::WritePtrNull),
            ERRNO_QUEUE_WRITESIZE_ISZERO => Ok(QueueError::WriteSizeIsZero),
            ERRNO_QUEUE_WRITE_SIZE_TOO_BIG => Ok(QueueError::WriteSizeTooBig),
            ERRNO_QUEUE_ISFULL => Ok(QueueError::IsFull),
            ERRNO_QUEUE_PTR_NULL => Ok(QueueError::PtrNull),
            ERRNO_QUEUE_READ_IN_INTERRUPT => Ok(QueueError::ReadInInterrupt),
            ERRNO_QUEUE_ISEMPTY => Ok(QueueError::IsEmpty),
            ERRNO_QUEUE_READ_SIZE_TOO_SMALL => Ok(QueueError::ReadSizeTooSmall),
            _ => Err(()),
        }
    }
}

impl core::fmt::Display for QueueError {
    fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
        let desc = match self {
            Self::CreateNoMemory => "Failed to allocate memory for queue creation",
            Self::SizeTooBig => "Message size is too big for queue",
            Self::CbUnavailable => "No available queue control blocks",
            Self::NotFound => "Queue not found",
            Self::PendInLock => "Cannot wait on queue while task is locked",
            Self::Timeout => "Queue operation timed out",
            Self::InTaskUse => "Queue is being used by tasks and cannot be deleted",
            Self::WriteInInterrupt => "Cannot write to queue with timeout in interrupt context",
            Self::NotCreate => "Queue has not been created",
            Self::InTaskWrite => "Queue reading and writing are not synchronized",
            Self::CreatePtrNull => "Null pointer passed during queue creation",
            Self::ParaIsZero => "Queue length or message size is zero",
            Self::Invalid => "Invalid queue handle",
            Self::ReadPtrNull => "Null pointer passed during queue reading",
            Self::ReadSizeInvalid => "Invalid buffer size for queue reading",
            Self::WritePtrNull => "Null pointer passed during queue writing",
            Self::WriteSizeIsZero => "Buffer size for queue writing is zero",
            Self::WriteSizeTooBig => "Buffer size exceeds queue message size",
            Self::IsFull => "Queue is full",
            Self::PtrNull => "Null pointer passed when getting queue information",
            Self::ReadInInterrupt => "Cannot read from queue with timeout in interrupt context",
            Self::IsEmpty => "Queue is empty",
            Self::ReadSizeTooSmall => "Buffer size is too small for queue reading",
        };
        write!(f, "{}", desc)
    }
}
