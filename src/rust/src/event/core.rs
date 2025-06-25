//! 事件核心操作实现

use core::ptr::addr_of;

use crate::ffi::bindings::{arch_int_locked, get_current_task};
use crate::interrupt::{disable_interrupts, restore_interrupt_state};
use crate::percpu::can_preempt_in_scheduler;
use crate::result::{SystemError, SystemResult};
use crate::task::sched::{schedule, schedule_reschedule};
use crate::task::sync::wait::{task_wait, task_wake};
use crate::task::types::{TaskCB, TaskStatus};
use crate::utils::list::LinkedList;

use super::error::EventError;
use super::types::{EventCB, EventWaitMode};

use crate::interrupt::is_interrupt_active;
use crate::println_debug;

const EVENT_RESERVED_BIT_MASK: u32 = 0x0200_0000;

/// 事件参数检查
#[inline]
fn validate_event_params(event_mask: u32, mode: u32) -> SystemResult<()> {
    // 检查事件掩码
    validate_event_mask(event_mask)?;

    // 检查模式有效性
    if !EventWaitMode::validate(mode) {
        return Err(SystemError::Event(EventError::ModeInvalid));
    }

    Ok(())
}

/// 检查事件读取的上下文有效性
#[inline]
fn validate_event_read_context() -> SystemResult<()> {
    // 检查是否在中断上下文中
    if is_interrupt_active() {
        return Err(SystemError::Event(EventError::ReadInInterrupt));
    }

    // 检查是否在系统任务中（给出警告但不阻止）
    let task = get_current_task();
    if task.is_system_task() {
        println_debug!("DO NOT recommend to use event_read in system tasks.");
    }

    Ok(())
}

/// 检查事件掩码是否有效
#[inline]
fn validate_event_mask(event_mask: u32) -> SystemResult<()> {
    if event_mask == 0 {
        return Err(SystemError::Event(EventError::MaskInvalid));
    }

    if (event_mask & EVENT_RESERVED_BIT_MASK) != 0 {
        return Err(SystemError::Event(EventError::SetBitInvalid));
    }

    Ok(())
}

/// 检查事件设置是否有效
#[inline]
pub fn validate_event_set(events: u32) -> SystemResult<()> {
    if (events & EVENT_RESERVED_BIT_MASK) != 0 {
        Err(SystemError::Event(EventError::SetBitInvalid))
    } else {
        Ok(())
    }
}

/// 事件轮询操作
#[inline]
fn poll(event_id: &mut u32, event_mask: u32, mode: u32) -> u32 {
    debug_assert!(arch_int_locked());

    let result = {
        if EventWaitMode::is_or(mode) {
            // OR模式：任意一个事件满足即可
            *event_id & event_mask
        } else {
            // AND模式：所有事件都必须满足
            if event_mask == (*event_id & event_mask) {
                *event_id & event_mask
            } else {
                0
            }
        }
    };

    // 如果需要清除事件且有匹配的事件
    if result != 0 && EventWaitMode::is_clear(mode) {
        *event_id = *event_id & !result;
    }

    result
}

/// 事件读取实现
#[inline]
fn read(event_cb: &mut EventCB, event_mask: u32, mode: u32, timeout: u32) -> SystemResult<u32> {
    let mut int_save = disable_interrupts();
    let current_task = get_current_task();

    let mut result = poll(&mut event_cb.event_id, event_mask, mode);

    // 如果没有匹配的事件
    if result == 0 {
        if timeout == 0 {
            return Ok(result);
        }

        if !can_preempt_in_scheduler() {
            return Err(SystemError::Event(EventError::ReadInLock));
        }

        // 设置任务的事件等待信息
        current_task.event_mask = event_mask;
        current_task.event_mode = mode;

        // 将任务加入等待队列
        task_wait(&mut event_cb.wait_list, timeout);

        // 立即调度
        schedule_reschedule();

        // 解锁并重新加锁（任务被重新调度时会持有锁）
        restore_interrupt_state(int_save);
        int_save = disable_interrupts();

        // 检查是否超时
        if current_task.task_status.contains(TaskStatus::TIMEOUT) {
            current_task.task_status.remove(TaskStatus::TIMEOUT);
            return Err(SystemError::Event(EventError::ReadTimeout));
        }

        // 重新轮询事件
        result = poll(&mut event_cb.event_id, event_mask, mode);
    }

    restore_interrupt_state(int_save);
    Ok(result)
}

/// 事件写入实现
#[inline]
fn write(event_cb: &mut EventCB, events: u32) -> SystemResult<()> {
    let int_save = disable_interrupts();

    // 设置事件位
    event_cb.set_events(events);

    // 检查等待队列
    let need_schedule = wake_waiting_tasks(event_cb, events);

    restore_interrupt_state(int_save);

    // 如果有任务被唤醒，触发调度
    if need_schedule {
        schedule();
    }

    Ok(())
}

/// 唤醒等待的任务
#[inline]
fn wake_waiting_tasks(event_cb: &mut EventCB, events: u32) -> bool {
    let mut need_schedule = false;
    let event_id = event_cb.event_id;

    let mut cur_task = TaskCB::from_pend_list(event_cb.wait_list.next);

    while addr_of!(cur_task.pend_list) != addr_of!(event_cb.wait_list) {
        let next_task = TaskCB::from_pend_list(cur_task.pend_list.next);

        let task_mask = cur_task.event_mask;
        let task_mode = cur_task.event_mode;
        let should_wake = if EventWaitMode::is_or(task_mode) {
            // OR模式：任意事件匹配即唤醒
            (task_mask & events) != 0
        } else {
            // AND模式：所有事件都匹配才唤醒
            (task_mask & event_id) == task_mask
        };
        if should_wake {
            task_wake(cur_task);
            need_schedule = true;
        }
        cur_task = next_task;
    }

    need_schedule
}

/// 事件初始化
pub fn event_init(event_cb: &mut EventCB) {
    let int_save = disable_interrupts();

    event_cb.event_id = 0; // 初始化事件ID为0
    LinkedList::init(&raw mut event_cb.wait_list); // 初始化等待列表

    restore_interrupt_state(int_save);
}

/// 事件销毁
pub fn event_destroy(event_cb: &mut EventCB) -> SystemResult<()> {
    let int_save = disable_interrupts();

    let result = if !event_cb.is_wait_list_empty() {
        Err(SystemError::Event(EventError::ShouldNotDestroy))
    } else {
        event_cb.event_id = 0; // 清除事件ID
        Ok(())
    };

    restore_interrupt_state(int_save);
    result
}

/// 事件读取
pub fn event_read(
    event_cb: &mut EventCB,
    event_mask: u32,
    mode: u32,
    timeout: u32,
) -> SystemResult<u32> {
    validate_event_params(event_mask, mode)?;
    validate_event_read_context()?;

    read(event_cb, event_mask, mode, timeout)
}

/// 事件写入
pub fn event_write(event_cb: &mut EventCB, events: u32) -> SystemResult<()> {
    validate_event_set(events)?;
    write(event_cb, events)
}

/// 事件轮询
pub fn event_poll(event_id: &mut u32, event_mask: u32, mode: u32) -> SystemResult<u32> {
    validate_event_params(event_mask, mode)?;

    let int_save = disable_interrupts();
    let result = poll(event_id, event_mask, mode);
    restore_interrupt_state(int_save);

    Ok(result)
}

/// 事件清除
pub fn event_clear(event_cb: &mut EventCB, events: u32) {
    let int_save = disable_interrupts();
    event_cb.clear_events(!events); // 清除指定位
    restore_interrupt_state(int_save);
}
