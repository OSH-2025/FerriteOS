/// 事件操作错误类型
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum EventError {
    /// 设置位无效
    SetBitInvalid,
    /// 读取事件超时
    ReadTimeout,
    /// 事件掩码无效
    MaskInvalid,
    /// 在中断中读取事件
    ReadInInterrupt,
    /// 读取事件模式无效
    ModeInvalid,
    /// 在锁定状态下读取事件
    ReadInLock,
    /// 事件指针为空
    PtrNull,
    /// 在系统任务中读取事件
    ReadInSystemTask,
    /// 不应该销毁的事件
    ShouldNotDestroy,
}

impl From<EventError> for u32 {
    fn from(err: EventError) -> u32 {
        match err {
            EventError::SetBitInvalid => ERRNO_EVENT_SETBIT_INVALID,
            EventError::ReadTimeout => ERRNO_EVENT_READ_TIMEOUT,
            EventError::MaskInvalid => ERRNO_EVENT_EVENTMASK_INVALID,
            EventError::ReadInInterrupt => ERRNO_EVENT_READ_IN_INTERRUPT,
            EventError::ModeInvalid => ERRNO_EVENT_FLAGS_INVALID,
            EventError::ReadInLock => ERRNO_EVENT_READ_IN_LOCK,
            EventError::PtrNull => ERRNO_EVENT_PTR_NULL,
            EventError::ReadInSystemTask => ERRNO_EVENT_READ_IN_SYSTEM_TASK,
            EventError::ShouldNotDestroy => ERRNO_EVENT_SHOULD_NOT_DESTORY,
        }
    }
}

/// 从u32错误码转换为InterruptError
impl TryFrom<u32> for EventError {
    type Error = ();

    fn try_from(errno: u32) -> Result<Self, Self::Error> {
        match errno {
            ERRNO_EVENT_SETBIT_INVALID => Ok(EventError::SetBitInvalid),
            ERRNO_EVENT_READ_TIMEOUT => Ok(EventError::ReadTimeout),
            ERRNO_EVENT_EVENTMASK_INVALID => Ok(EventError::MaskInvalid),
            ERRNO_EVENT_READ_IN_INTERRUPT => Ok(EventError::ReadInInterrupt),
            ERRNO_EVENT_FLAGS_INVALID => Ok(EventError::ModeInvalid),
            ERRNO_EVENT_READ_IN_LOCK => Ok(EventError::ReadInLock),
            ERRNO_EVENT_PTR_NULL => Ok(EventError::PtrNull),
            ERRNO_EVENT_READ_IN_SYSTEM_TASK => Ok(EventError::ReadInSystemTask),
            ERRNO_EVENT_SHOULD_NOT_DESTORY => Ok(EventError::ShouldNotDestroy),
            _ => Err(()),
        }
    }
}

impl core::fmt::Display for EventError {
    fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
        let desc = match self {
            Self::PtrNull => "Event pointer is null",
            Self::MaskInvalid => "Event mask is invalid",
            Self::SetBitInvalid => "Event set bit is invalid",
            Self::ModeInvalid => "Event mode are invalid",
            Self::ReadInInterrupt => "Cannot read event in interrupt context",
            Self::ReadTimeout => "Event read timeout",
            Self::ReadInLock => "Cannot read event in lock context",
            Self::ReadInSystemTask => "Cannot read event in system task context",
            Self::ShouldNotDestroy => "Event should not be destroyed",
        };
        write!(f, "{}", desc)
    }
}

const ERRNO_EVENT_SETBIT_INVALID: u32 = 0x02001c00;
const ERRNO_EVENT_READ_TIMEOUT: u32 = 0x02001c01;
const ERRNO_EVENT_EVENTMASK_INVALID: u32 = 0x02001c02;
const ERRNO_EVENT_READ_IN_INTERRUPT: u32 = 0x02001c03;
const ERRNO_EVENT_FLAGS_INVALID: u32 = 0x02001c04;
const ERRNO_EVENT_READ_IN_LOCK: u32 = 0x02001c05;
const ERRNO_EVENT_PTR_NULL: u32 = 0x02001c06;
const ERRNO_EVENT_READ_IN_SYSTEM_TASK: u32 = 0x02001c07;
const ERRNO_EVENT_SHOULD_NOT_DESTORY: u32 = 0x02001c08;
