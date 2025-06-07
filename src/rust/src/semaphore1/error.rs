use core::fmt;

/// 信号量错误码定义
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum SemaphoreError {
    /// 信号量指针为空
    PtrNull,
    /// 信号量全部被使用
    AllBusy,
    /// 信号量溢出
    Overflow,
    /// 无效信号量
    Invalid,
    /// 信号量已被任务使用
    Pended,
    /// 在中断中等待信号量
    PendInterrupt,
    /// 在锁定状态下等待信号量
    PendInLock,
    /// 信号量不可用
    Unavailable,
    /// 等待信号量超时
    Timeout,
}

// 错误码常量定义
const ERRNO_SEM_PTR_NULL: u32 = 0x02000700;
const ERRNO_SEM_ALL_BUSY: u32 = 0x02000701;
const ERRNO_SEM_OVERFLOW: u32 = 0x02000702;
const ERRNO_SEM_INVALID: u32 = 0x02000703;
const ERRNO_SEM_PENDED: u32 = 0x02000704;
const ERRNO_SEM_PEND_INTERR: u32 = 0x02000705;
const ERRNO_SEM_PEND_IN_LOCK: u32 = 0x02000706;
const ERRNO_SEM_UNAVAILABLE: u32 = 0x02000707;
const ERRNO_SEM_TIMEOUT: u32 = 0x02000708;

impl From<SemaphoreError> for u32 {
    fn from(err: SemaphoreError) -> u32 {
        match err {
            SemaphoreError::PtrNull => ERRNO_SEM_PTR_NULL,
            SemaphoreError::AllBusy => ERRNO_SEM_ALL_BUSY,
            SemaphoreError::Overflow => ERRNO_SEM_OVERFLOW,
            SemaphoreError::Invalid => ERRNO_SEM_INVALID,
            SemaphoreError::Pended => ERRNO_SEM_PENDED,
            SemaphoreError::PendInterrupt => ERRNO_SEM_PEND_INTERR,
            SemaphoreError::PendInLock => ERRNO_SEM_PEND_IN_LOCK,
            SemaphoreError::Unavailable => ERRNO_SEM_UNAVAILABLE,
            SemaphoreError::Timeout => ERRNO_SEM_TIMEOUT,
        }
    }
}

impl TryFrom<u32> for SemaphoreError {
    type Error = ();

    fn try_from(errno: u32) -> Result<Self, Self::Error> {
        match errno {
            ERRNO_SEM_PTR_NULL => Ok(SemaphoreError::PtrNull),
            ERRNO_SEM_ALL_BUSY => Ok(SemaphoreError::AllBusy),
            ERRNO_SEM_OVERFLOW => Ok(SemaphoreError::Overflow),
            ERRNO_SEM_INVALID => Ok(SemaphoreError::Invalid),
            ERRNO_SEM_PENDED => Ok(SemaphoreError::Pended),
            ERRNO_SEM_PEND_INTERR => Ok(SemaphoreError::PendInterrupt),
            ERRNO_SEM_PEND_IN_LOCK => Ok(SemaphoreError::PendInLock),
            ERRNO_SEM_UNAVAILABLE => Ok(SemaphoreError::Unavailable),
            ERRNO_SEM_TIMEOUT => Ok(SemaphoreError::Timeout),
            _ => Err(()),
        }
    }
}

/* impl From<SemaphoreError> for crate::result::SystemError {
    fn from(err: SemaphoreError) -> Self {
        crate::result::SystemError::Semaphore1(err)
    }
} */

impl fmt::Display for SemaphoreError {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        let desc = match self {
            Self::PtrNull => "Semaphore pointer is null",
            Self::AllBusy => "All semaphores are busy",
            Self::Overflow => "Semaphore counter overflow",
            Self::Invalid => "Invalid semaphore",
            Self::Pended => "Semaphore is being pended",
            Self::PendInterrupt => "Cannot pend semaphore in interrupt context",
            Self::PendInLock => "Cannot pend semaphore in lock state",
            Self::Unavailable => "Semaphore is unavailable",
            Self::Timeout => "Pend semaphore timeout",
        };
        write!(f, "{}", desc)
    }
}

// 系统结果类型别名
pub type SemaphoreResult<T> = Result<T, crate::result::SystemError>;