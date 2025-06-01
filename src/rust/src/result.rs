use crate::{
    event::error::EventError, interrupt::error::InterruptError, stack::error::StackError,
    task::error::TaskError,
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

impl From<SystemError> for u32 {
    fn from(error: SystemError) -> Self {
        match error {
            SystemError::Task(err) => u32::from(err),
            SystemError::Interrupt(err) => u32::from(err),
            SystemError::Stack(err) => u32::from(err),
            SystemError::Event(err) => u32::from(err),
            SystemError::Unknown(errno) => errno,
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
            } else {
                Err(SystemError::Unknown(errno))
            }
        }
    }
}
