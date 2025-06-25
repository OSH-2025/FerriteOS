/// 定时器操作错误类型
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum TimerError {
    /// 定时器指针为空
    PtrNull,
    /// 定时器间隔时间不合适
    IntervalNotSuited,
    /// 定时器模式无效
    ModeInvalid,
    /// ID返回指针为空
    RetPtrNull,
    /// 超过最大定时器数量
    MaxSize,
    /// 定时器ID无效
    IdInvalid,
    /// 定时器未创建
    NotCreated,
    /// 队列创建失败
    QueueCreateFailed,
    /// 任务创建失败
    TaskCreateFailed,
    /// 定时器未启动
    NotStarted,
    /// 定时器状态无效
    StatusInvalid,
    /// Tick指针为空
    TickPtrNull,
}

impl From<TimerError> for u32 {
    fn from(err: TimerError) -> u32 {
        match err {
            TimerError::PtrNull => ERRNO_TIMER_PTR_NULL,
            TimerError::IntervalNotSuited => ERRNO_TIMER_INTERVAL_NOT_SUITED,
            TimerError::ModeInvalid => ERRNO_TIMER_MODE_INVALID,
            TimerError::RetPtrNull => ERRNO_SWTMR_RET_PTR_NULL,
            TimerError::MaxSize => ERRNO_TIMER_MAXSIZE,
            TimerError::IdInvalid => ERRNO_TIMER_ID_INVALID,
            TimerError::NotCreated => ERRNO_TIMER_NOT_CREATED,
            TimerError::QueueCreateFailed => ERRNO_TIMER_QUEUE_CREATE_FAILED,
            TimerError::TaskCreateFailed => ERRNO_TIMER_TASK_CREATE_FAILED,
            TimerError::NotStarted => ERRNO_TIMER_NOT_STARTED,
            TimerError::StatusInvalid => ERRNO_TIMER_STATUS_INVALID,
            TimerError::TickPtrNull => ERRNO_SWTMR_TICK_PTR_NULL,
        }
    }
}

/// 从u32错误码转换为TimerError
impl TryFrom<u32> for TimerError {
    type Error = ();

    fn try_from(errno: u32) -> Result<Self, Self::Error> {
        match errno {
            ERRNO_TIMER_PTR_NULL => Ok(TimerError::PtrNull),
            ERRNO_TIMER_INTERVAL_NOT_SUITED => Ok(TimerError::IntervalNotSuited),
            ERRNO_TIMER_MODE_INVALID => Ok(TimerError::ModeInvalid),
            ERRNO_SWTMR_RET_PTR_NULL => Ok(TimerError::RetPtrNull),
            ERRNO_TIMER_MAXSIZE => Ok(TimerError::MaxSize),
            ERRNO_TIMER_ID_INVALID => Ok(TimerError::IdInvalid),
            ERRNO_TIMER_NOT_CREATED => Ok(TimerError::NotCreated),
            ERRNO_TIMER_QUEUE_CREATE_FAILED => Ok(TimerError::QueueCreateFailed),
            ERRNO_TIMER_TASK_CREATE_FAILED => Ok(TimerError::TaskCreateFailed),
            ERRNO_TIMER_NOT_STARTED => Ok(TimerError::NotStarted),
            ERRNO_TIMER_STATUS_INVALID => Ok(TimerError::StatusInvalid),
            ERRNO_SWTMR_TICK_PTR_NULL => Ok(TimerError::TickPtrNull),
            _ => Err(()),
        }
    }
}

impl core::fmt::Display for TimerError {
    fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
        let desc = match self {
            Self::PtrNull => "Timer pointer is null",
            Self::IntervalNotSuited => "Timer interval is not suitable",
            Self::ModeInvalid => "Timer mode is invalid",
            Self::RetPtrNull => "Return pointer for timer ID is null",
            Self::MaxSize => "Exceeded maximum number of timers",
            Self::IdInvalid => "Timer ID is invalid",
            Self::NotCreated => "Timer is not created",
            Self::QueueCreateFailed => "Failed to create timer queue",
            Self::TaskCreateFailed => "Failed to create timer task",
            Self::NotStarted => "Timer is not started",
            Self::StatusInvalid => "Timer status is invalid",
            Self::TickPtrNull => "Tick pointer is null",
        };
        write!(f, "{}", desc)
    }
}

// 定时器错误码常量定义
const ERRNO_TIMER_PTR_NULL: u32 = 0x02000300;
const ERRNO_TIMER_INTERVAL_NOT_SUITED: u32 = 0x02000301;
const ERRNO_TIMER_MODE_INVALID: u32 = 0x02000302;
const ERRNO_SWTMR_RET_PTR_NULL: u32 = 0x02000303;
const ERRNO_TIMER_MAXSIZE: u32 = 0x02000304;
const ERRNO_TIMER_ID_INVALID: u32 = 0x02000305;
const ERRNO_TIMER_NOT_CREATED: u32 = 0x02000306;
const ERRNO_TIMER_QUEUE_CREATE_FAILED: u32 = 0x0200030b;
const ERRNO_TIMER_TASK_CREATE_FAILED: u32 = 0x0200030c;
const ERRNO_TIMER_NOT_STARTED: u32 = 0x0200030d;
const ERRNO_TIMER_STATUS_INVALID: u32 = 0x0200030e;
const ERRNO_SWTMR_TICK_PTR_NULL : u32 = 0x02000310;
