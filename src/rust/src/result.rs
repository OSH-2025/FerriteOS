use crate::{
    event::error::EventError, interrupt::error::InterruptError, mutex::error::MutexError,
    queue::error::QueueError, semaphore::error::SemaphoreError, stack::error::StackError,
    task::error::TaskError, timer::TimerError,
};

pub type SystemResult<T> = Result<T, SystemError>;

/// 系统级通用错误类型
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum SystemError {
    /// 任务相关错误
    Task(TaskError),
    /// 中断相关错误
    Interrupt(InterruptError),
    /// 栈相关错误
    Stack(StackError),
    /// 事件操作错误类型
    Event(EventError),
    /// 互斥锁相关错误
    Mutex(MutexError),
    /// 信号量相关错误
    Semaphore(SemaphoreError),
    /// 消息队列相关错误
    Queue(QueueError),
    /// 定时器相关错误
    Timer(TimerError),
    /// 未知错误码
    Unknown(u32),
}

impl From<TaskError> for SystemError {
    fn from(err: TaskError) -> Self {
        SystemError::Task(err)
    }
}

impl From<InterruptError> for SystemError {
    fn from(err: InterruptError) -> Self {
        SystemError::Interrupt(err)
    }
}

impl From<StackError> for SystemError {
    fn from(err: StackError) -> Self {
        SystemError::Stack(err)
    }
}

impl From<EventError> for SystemError {
    fn from(err: EventError) -> Self {
        SystemError::Event(err)
    }
}

impl From<MutexError> for SystemError {
    fn from(err: MutexError) -> Self {
        SystemError::Mutex(err)
    }
}

impl From<SemaphoreError> for SystemError {
    fn from(err: SemaphoreError) -> Self {
        SystemError::Semaphore(err)
    }
}

impl From<QueueError> for SystemError {
    fn from(err: QueueError) -> Self {
        SystemError::Queue(err)
    }
}

impl From<TimerError> for SystemError {
    fn from(err: TimerError) -> Self {
        SystemError::Timer(err)
    }
}

impl From<SystemError> for u32 {
    fn from(error: SystemError) -> Self {
        match error {
            SystemError::Task(err) => u32::from(err),
            SystemError::Interrupt(err) => u32::from(err),
            SystemError::Stack(err) => u32::from(err),
            SystemError::Event(err) => u32::from(err),
            SystemError::Mutex(err) => u32::from(err),
            SystemError::Semaphore(err) => u32::from(err),
            SystemError::Queue(err) => u32::from(err),
            SystemError::Timer(err) => u32::from(err),
            SystemError::Unknown(errno) => errno,
        }
    }
}

impl core::fmt::Display for SystemError {
    fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
        match self {
            SystemError::Task(err) => write!(f, "Task error: {}", err),
            SystemError::Interrupt(err) => write!(f, "Interrupt error: {}", err),
            SystemError::Stack(err) => write!(f, "Stack error: {}", err),
            SystemError::Event(err) => write!(f, "Event error: {}", err),
            SystemError::Mutex(err) => write!(f, "Mutex error: {}", err),
            SystemError::Semaphore(err) => write!(f, "Semaphore error: {}", err),
            SystemError::Queue(err) => write!(f, "Queue error: {}", err),
            SystemError::Timer(err) => write!(f, "Timer error: {}", err),
            SystemError::Unknown(code) => write!(f, "Unknown error: 0x{:08x}", code),
        }
    }
}

pub struct ErrorCode(pub u32);

impl From<ErrorCode> for SystemResult<()> {
    fn from(code: ErrorCode) -> Self {
        let errno = code.0;
        if errno == 0 {
            Ok(())
        } else {
            if let Ok(task_error) = TaskError::try_from(errno) {
                Err(SystemError::Task(task_error))
            } else if let Ok(interrupt_error) = InterruptError::try_from(errno) {
                Err(SystemError::Interrupt(interrupt_error))
            } else if let Ok(stack_error) = StackError::try_from(errno) {
                Err(SystemError::Stack(stack_error))
            } else if let Ok(event_error) = EventError::try_from(errno) {
                Err(SystemError::Event(event_error))
            } else if let Ok(mutex_error) = MutexError::try_from(errno) {
                Err(SystemError::Mutex(mutex_error))
            } else if let Ok(semaphore_error) = SemaphoreError::try_from(errno) {
                Err(SystemError::Semaphore(semaphore_error))
            } else if let Ok(queue_error) = QueueError::try_from(errno) {
                Err(SystemError::Queue(queue_error))
            } else if let Ok(timer_error) = TimerError::try_from(errno) {
                Err(SystemError::Timer(timer_error))
            } else {
                Err(SystemError::Unknown(errno))
            }
        }
    }
}
