/// 信号量操作错误
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u32)]
pub enum SemaphoreError {
    /// 互斥锁句柄无效
    Invalid,
    /// 互斥锁指针为空
    PtrNull,
    /// 所有互斥锁都在使用中
    AllBusy,
    /// 互斥锁不可用
    Unavailable,
    /// 在中断中等待互斥锁
    PendInInterrupt,
    /// 在调度锁定状态下等待互斥锁
    PendInLock,
    /// 等待互斥锁超时
    Timeout,
    /// 信号量溢出
    Overflow,
    /// 互斥锁被挂起，无法删除
    Pended,
}

const ERRNO_SEM_INVALID: u32 = 0x02000701;
const ERRNO_SEM_PTR_NULL: u32 = 0x02000702;
const ERRNO_SEM_ALL_BUSY: u32 = 0x02000703;
const ERRNO_SEM_UNAVAILABLE: u32 = 0x02000704;
const ERRNO_SEM_PEND_INTERR: u32 = 0x02000705;
const ERRNO_SEM_PEND_IN_LOCK: u32 = 0x02000706;
const ERRNO_SEM_TIMEOUT: u32 = 0x02000707;
const ERRNO_SEM_OVERFLOW: u32 = 0x02000708;
const ERRNO_SEM_PENDED: u32 = 0x02000709;

impl From<SemaphoreError> for u32 {
    fn from(err: SemaphoreError) -> u32 {
        match err {
            SemaphoreError::Invalid => ERRNO_SEM_INVALID,
            SemaphoreError::PtrNull => ERRNO_SEM_PTR_NULL,
            SemaphoreError::AllBusy => ERRNO_SEM_ALL_BUSY,
            SemaphoreError::Unavailable => ERRNO_SEM_UNAVAILABLE,
            SemaphoreError::PendInInterrupt => ERRNO_SEM_PEND_INTERR,
            SemaphoreError::PendInLock => ERRNO_SEM_PEND_IN_LOCK,
            SemaphoreError::Timeout => ERRNO_SEM_TIMEOUT,
            SemaphoreError::Overflow => ERRNO_SEM_OVERFLOW,
            SemaphoreError::Pended => ERRNO_SEM_PENDED,
        }
    }
}

impl TryFrom<u32> for SemaphoreError {
    type Error = ();

    fn try_from(errno: u32) -> Result<Self, Self::Error> {
        match errno {
            ERRNO_SEM_INVALID => Ok(SemaphoreError::Invalid),
            ERRNO_SEM_PTR_NULL => Ok(SemaphoreError::PtrNull),
            ERRNO_SEM_ALL_BUSY => Ok(SemaphoreError::AllBusy),
            ERRNO_SEM_UNAVAILABLE => Ok(SemaphoreError::Unavailable),
            ERRNO_SEM_PEND_INTERR => Ok(SemaphoreError::PendInInterrupt),
            ERRNO_SEM_PEND_IN_LOCK => Ok(SemaphoreError::PendInLock),
            ERRNO_SEM_TIMEOUT => Ok(SemaphoreError::Timeout),
            ERRNO_SEM_OVERFLOW => Ok(SemaphoreError::Overflow),
            ERRNO_SEM_PENDED => Ok(SemaphoreError::Pended),
            _ => Err(()),
        }
    }
}

impl core::fmt::Display for SemaphoreError {
    fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
        let desc = match self {
            Self::Invalid => "Invalid mutex handle",
            Self::PtrNull => "Semaphore pointer is null",
            Self::AllBusy => "All semaphores are busy",
            Self::Unavailable => "Semaphore is unavailable",
            Self::PendInInterrupt => "Waiting for semaphore in interrupt context",
            Self::PendInLock => "Waiting for semaphore while in lock state",
            Self::Timeout => "Semaphore wait timed out",
            Self::Overflow => "Semaphore overflow occurred",
            Self::Pended => "Semaphore is pending and cannot be deleted",
        };
        write!(f, "{}", desc)
    }
}
