use crate::config::TIMER_LIMIT;
use crate::interrupt::disable_interrupts;
use crate::interrupt::restore_interrupt_state;
use crate::result::SystemResult;
use crate::timer::TimerError;
use crate::timer::global::TimerPool;
use crate::timer::internal::timer_delete_internal;
use crate::timer::internal::timer_get_time_internal;
use crate::timer::internal::timer_start_internal;
use crate::timer::internal::timer_stop_internal;
use crate::timer::types::TimerHandler;
use crate::timer::types::TimerId;
use crate::timer::types::TimerMode;
use crate::timer::types::TimerState;

/// 创建定时器
pub fn timer_create(timeout: u32, mode: TimerMode, handler: TimerHandler) -> SystemResult<TimerId> {
    if timeout == 0 {
        return Err(TimerError::IntervalNotSuited.into());
    }

    if handler.is_none() {
        return Err(TimerError::PtrNull.into());
    }

    let int_save = disable_interrupts();

    if !TimerPool::has_available() {
        restore_interrupt_state(int_save);
        Err(TimerError::MaxSize.into())
    } else {
        let timer = TimerPool::allocate(mode, timeout, handler);
        restore_interrupt_state(int_save);
        Ok(timer.get_id())
    }
}

/// 启动定时器
pub fn timer_start(timer_id: TimerId) -> SystemResult<()> {
    let index = timer_id.get_index();
    if index as u32 >= TIMER_LIMIT {
        return Err(TimerError::IdInvalid.into());
    }

    let int_save = disable_interrupts();

    let timer = TimerPool::get_timer_by_index(index as usize);

    if !timer.matches_id(timer_id) {
        restore_interrupt_state(int_save);
        return Err(TimerError::IdInvalid.into());
    }

    // 根据定时器状态执行不同操作
    match timer.get_state() {
        TimerState::Unused => {
            restore_interrupt_state(int_save);
            Err(TimerError::NotCreated.into())
        }
        TimerState::Created => {
            timer_start_internal(timer);
            restore_interrupt_state(int_save);
            Ok(())
        }
        TimerState::Running => {
            timer_stop_internal(timer);
            timer_start_internal(timer);
            restore_interrupt_state(int_save);
            Ok(())
        }
    }
}

/// 停止定时器
pub fn timer_stop(timer_id: TimerId) -> SystemResult<()> {
    let index = timer_id.get_index();
    if index as u32 >= TIMER_LIMIT {
        return Err(TimerError::IdInvalid.into());
    }

    let int_save = disable_interrupts();
    let timer = TimerPool::get_timer_by_index(index as usize);

    if !timer.matches_id(timer_id) {
        restore_interrupt_state(int_save);
        return Err(TimerError::IdInvalid.into());
    }

    // 根据定时器状态执行不同操作
    match timer.get_state() {
        TimerState::Unused => {
            restore_interrupt_state(int_save);
            Err(TimerError::NotCreated.into())
        }
        TimerState::Created => {
            restore_interrupt_state(int_save);
            Err(TimerError::NotStarted.into())
        }
        TimerState::Running => {
            timer_stop_internal(timer);
            restore_interrupt_state(int_save);
            Ok(())
        }
    }
}

/// 删除定时器
pub fn timer_delete(timer_id: TimerId) -> SystemResult<()> {
    let index = timer_id.get_index();
    if index as u32 >= TIMER_LIMIT {
        return Err(TimerError::IdInvalid.into());
    }

    let int_save = disable_interrupts();
    let timer = TimerPool::get_timer_by_index(index as usize);

    if !timer.matches_id(timer_id) {
        restore_interrupt_state(int_save);
        return Err(TimerError::IdInvalid.into());
    }

    // 根据定时器状态执行不同操作
    match timer.get_state() {
        TimerState::Unused => {
            restore_interrupt_state(int_save);
            Err(TimerError::NotCreated.into())
        }
        TimerState::Created => {
            timer_delete_internal(timer);
            restore_interrupt_state(int_save);
            Ok(())
        }
        TimerState::Running => {
            timer_stop_internal(timer);
            timer_delete_internal(timer);
            restore_interrupt_state(int_save);
            Ok(())
        }
    }
}

/// 获取定时器剩余时间
pub fn timer_time_get(timer_id: TimerId) -> SystemResult<u32> {
    let index = timer_id.get_index();
    if index as u32 >= TIMER_LIMIT {
        return Err(TimerError::IdInvalid.into());
    }
    let int_save = disable_interrupts();
    let timer = TimerPool::get_timer_by_index(index as usize);

    if !timer.matches_id(timer_id) {
        restore_interrupt_state(int_save);
        return Err(TimerError::IdInvalid.into());
    }

    match timer.state {
        TimerState::Unused => {
            restore_interrupt_state(int_save);
            Err(TimerError::NotCreated.into())
        }
        TimerState::Running => {
            restore_interrupt_state(int_save);
            Err(TimerError::NotStarted.into())
        }
        TimerState::Created => {
            let time = timer_get_time_internal(timer);
            restore_interrupt_state(int_save);
            Ok(time)
        }
    }
}
