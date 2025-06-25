//! 互斥锁错误码定义

/// 互斥锁操作错误类型
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u32)]
pub enum MutexError {
    /// 互斥锁句柄无效
    Invalid,
    /// 互斥锁指针为空
    PtrNull,
    /// 所有互斥锁都在使用中
    AllBusy,
    /// 互斥锁不可用
    Unavailable,
    /// 在中断中等待互斥锁
    PendInterrupt,
    /// 在调度锁定状态下等待互斥锁
    PendInLock,
    /// 等待互斥锁超时
    Timeout,
    /// 互斥锁被挂起，无法删除
    Pended,
}

impl From<MutexError> for u32 {
    fn from(err: MutexError) -> u32 {
        match err {
            MutexError::Invalid => ERRNO_MUX_INVALID,
            MutexError::PtrNull => ERRNO_MUX_PTR_NULL,
            MutexError::AllBusy => ERRNO_MUX_ALL_BUSY,
            MutexError::Unavailable => ERRNO_MUX_UNAVAILABLE,
            MutexError::PendInterrupt => ERRNO_MUX_PEND_INTERR,
            MutexError::PendInLock => ERRNO_MUX_PEND_IN_LOCK,
            MutexError::Timeout => ERRNO_MUX_TIMEOUT,
            MutexError::Pended => ERRNO_MUX_PENDED,
        }
    }
}

impl TryFrom<u32> for MutexError {
    type Error = ();

    fn try_from(errno: u32) -> Result<Self, Self::Error> {
        match errno {
            ERRNO_MUX_INVALID => Ok(MutexError::Invalid),
            ERRNO_MUX_PTR_NULL => Ok(MutexError::PtrNull),
            ERRNO_MUX_ALL_BUSY => Ok(MutexError::AllBusy),
            ERRNO_MUX_UNAVAILABLE => Ok(MutexError::Unavailable),
            ERRNO_MUX_PEND_INTERR => Ok(MutexError::PendInterrupt),
            ERRNO_MUX_PEND_IN_LOCK => Ok(MutexError::PendInLock),
            ERRNO_MUX_TIMEOUT => Ok(MutexError::Timeout),
            ERRNO_MUX_PENDED => Ok(MutexError::Pended),
            _ => Err(()),
        }
    }
}

const ERRNO_MUX_INVALID: u32 = 0x02001d01;
const ERRNO_MUX_PTR_NULL: u32 = 0x02001d02;
const ERRNO_MUX_ALL_BUSY: u32 = 0x02001d03;
const ERRNO_MUX_UNAVAILABLE: u32 = 0x02001d04;
const ERRNO_MUX_PEND_INTERR: u32 = 0x02001d05;
const ERRNO_MUX_PEND_IN_LOCK: u32 = 0x02001d06;
const ERRNO_MUX_TIMEOUT: u32 = 0x02001d07;
const ERRNO_MUX_PENDED: u32 = 0x02001d09;

impl core::fmt::Display for MutexError {
    fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
        let desc = match self {
            Self::Invalid => "Invalid mutex handle",
            Self::PtrNull => "Mutex pointer is null",
            Self::AllBusy => "All mutexes are busy",
            Self::Unavailable => "Mutex is unavailable",
            Self::PendInterrupt => "Cannot wait for mutex in interrupt context",
            Self::PendInLock => "Cannot wait for mutex in scheduler locked state",
            Self::Timeout => "Mutex wait timeout",
            Self::Pended => "Mutex is pended and cannot be deleted",
        };
        write!(f, "{}", desc)
    }
}
