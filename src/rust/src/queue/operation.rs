//! 消息队列操作功能实现

use core::ffi::c_void;
use core::ptr::{addr_of, copy};

use crate::config::QUEUE_LIMIT;
use crate::ffi::bindings::get_current_task;
use crate::interrupt::{disable_interrupts, is_interrupt_active, restore_interrupt_state};
use crate::percpu::can_preempt_in_scheduler;
use crate::queue::error::QueueError;
use crate::queue::global::QueueManager;
use crate::queue::types::{QueueControlBlock, QueueId, QueueOperationType};
use crate::result::SystemResult;
use crate::task::sched::{schedule, schedule_reschedule};
use crate::task::sync::wait::{task_wait, task_wake};
use crate::task::types::{TaskCB, TaskStatus};
use crate::utils::list::LinkedList;

/// 从队列读取数据
pub fn queue_read(
    queue_id: QueueId,
    buffer: *mut c_void,
    buffer_size: &mut u32,
    timeout: u32,
) -> SystemResult<()> {
    // 检查参数
    check_queue_read_parameters(queue_id, buffer, buffer_size, timeout)?;

    // 创建读操作类型
    let operate_type = QueueOperationType::ReadHead;

    // 执行队列操作
    queue_operate(queue_id, operate_type, buffer, buffer_size, timeout)
}

/// 从队列头部写入数据
pub fn queue_write_head(
    queue_id: QueueId,
    buffer: *const c_void,
    buffer_size: u32,
    timeout: u32,
) -> SystemResult<()> {
    // 检查参数
    let mut size = buffer_size;
    check_queue_write_parameters(queue_id, buffer, &size, timeout)?;

    // 创建写操作类型
    let operate_type = QueueOperationType::WriteHead;

    // 执行队列操作
    queue_operate(
        queue_id,
        operate_type,
        buffer as *mut c_void,
        &mut size,
        timeout,
    )
}

/// 从队列尾部写入数据
pub fn queue_write(
    queue_id: QueueId,
    buffer: *const c_void,
    buffer_size: u32,
    timeout: u32,
) -> SystemResult<()> {
    // 检查参数
    let mut size = buffer_size;
    check_queue_write_parameters(queue_id, buffer, &size, timeout)?;

    // 创建写操作类型
    let operate_type = QueueOperationType::WriteTail;

    // 执行队列操作
    queue_operate(
        queue_id,
        operate_type,
        buffer as *mut c_void,
        &mut size,
        timeout,
    )
}

/// 检查队列读取参数
fn check_queue_read_parameters(
    queue_id: QueueId,
    buffer: *const c_void,
    buffer_size: &u32,
    timeout: u32,
) -> SystemResult<()> {
    // 检查队列ID是否有效
    if queue_id.get_index() as u32 >= QUEUE_LIMIT {
        return Err(QueueError::Invalid.into());
    }

    // 检查缓冲区指针是否为空
    if buffer.is_null() {
        return Err(QueueError::ReadPtrNull.into());
    }

    // 检查缓冲区大小是否有效
    if *buffer_size == 0 || *buffer_size > (u16::MAX - 4) as u32 {
        return Err(QueueError::ReadSizeInvalid.into());
    }

    // 检查在中断中是否尝试非零超时等待
    if timeout != 0 && is_interrupt_active() {
        return Err(QueueError::ReadInInterrupt.into());
    }

    Ok(())
}

/// 检查队列写入参数
fn check_queue_write_parameters(
    queue_id: QueueId,
    buffer: *const c_void,
    buffer_size: &u32,
    timeout: u32,
) -> SystemResult<()> {
    // 检查队列ID是否有效
    if queue_id.get_index() as u32 >= QUEUE_LIMIT {
        return Err(QueueError::Invalid.into());
    }

    // 检查缓冲区指针是否为空
    if buffer.is_null() {
        return Err(QueueError::WritePtrNull.into());
    }

    // 检查缓冲区大小是否为零
    if *buffer_size == 0 {
        return Err(QueueError::WriteSizeIsZero.into());
    }

    // 检查在中断中是否尝试非零超时等待
    if timeout != 0 && is_interrupt_active() {
        return Err(QueueError::WriteInInterrupt.into());
    }

    Ok(())
}

/// 队列缓冲区操作
fn queue_buffer_operate(
    queue_cb: &mut QueueControlBlock,
    operate_type: QueueOperationType,
    buffer: *mut c_void,
    buffer_size: &mut u32,
) {
    let queue_position;

    // 根据操作类型获取队列位置并更新队列头/尾指针
    match operate_type {
        QueueOperationType::ReadHead => {
            queue_position = queue_cb.get_head();
            queue_cb.advance_head();
        }
        QueueOperationType::WriteHead => {
            queue_cb.retreat_head();
            queue_position = queue_cb.get_head();
        }
        QueueOperationType::WriteTail => {
            queue_position = queue_cb.get_tail();
            queue_cb.advance_tail();
        }
    }

    // 计算队列节点地址
    let queue_node = unsafe {
        queue_cb
            .queue_mem
            .add((queue_position as usize) * (queue_cb.queue_size as usize))
    };

    // 根据操作类型执行读取或写入
    if operate_type.is_read() {
        let msg_data_size: u32 = 0;
        unsafe {
            // 读取消息大小
            copy(
                queue_node.add(queue_cb.queue_size as usize - 4) as *const u8,
                addr_of!(msg_data_size) as *mut u8,
                4,
            );

            // 复制消息到用户缓冲区
            copy(
                queue_node as *const u8,
                buffer as *mut u8,
                msg_data_size as usize,
            );
        }
        // 返回实际读取的大小
        *buffer_size = msg_data_size;
    } else {
        // 写入消息到队列
        unsafe {
            copy(
                buffer as *const u8,
                queue_node as *mut u8,
                *buffer_size as usize,
            );
            copy(
                &raw const *buffer_size as *const u8,
                queue_node.add(queue_cb.queue_size as usize - 4) as *mut u8,
                4,
            );
        }
    }
}

/// 检查队列操作参数
fn check_queue_operate_params(
    queue_cb: &QueueControlBlock,
    queue_id: QueueId,
    operate_type: QueueOperationType,
    buffer_size: &u32,
) -> SystemResult<()> {
    // 检查队列是否存在且有效
    if !queue_cb.matches_id(queue_id) || queue_cb.is_unused() {
        return Err(QueueError::NotCreate.into());
    }

    // 检查缓冲区大小是否适合操作类型
    if operate_type.is_read() {
        if *buffer_size < (queue_cb.queue_size as u32 - 4) {
            return Err(QueueError::ReadSizeTooSmall.into());
        }
    } else if *buffer_size > (queue_cb.queue_size as u32 - 4) {
        return Err(QueueError::WriteSizeTooBig.into());
    }

    Ok(())
}

/// 队列操作核心函数
fn queue_operate(
    queue_id: QueueId,
    operate_type: QueueOperationType,
    buffer: *mut c_void,
    buffer_size: &mut u32,
    timeout: u32,
) -> SystemResult<()> {
    // 获取队列控制块
    let queue_cb = QueueManager::get_queue_by_index(queue_id.get_index() as usize);

    // 保存中断状态并禁用中断
    let int_save = disable_interrupts();

    // 检查队列操作参数
    match check_queue_operate_params(queue_cb, queue_id, operate_type, buffer_size) {
        Ok(_) => {}
        Err(e) => {
            restore_interrupt_state(int_save);
            return Err(e);
        }
    }

    // 检查是否有可读/可写资源
    if !queue_cb.has_available_resources(operate_type) {
        // 如果没有等待时间，直接返回队列空或满错误
        if timeout == 0 {
            restore_interrupt_state(int_save);
            return Err(if operate_type.is_read() {
                QueueError::IsEmpty.into()
            } else {
                QueueError::IsFull.into()
            });
        }

        // 检查是否可以在调度锁中等待
        if !can_preempt_in_scheduler() {
            restore_interrupt_state(int_save);
            return Err(QueueError::PendInLock.into());
        }

        // 让当前任务等待队列
        let wait_list = queue_cb.get_wait_list(operate_type);
        task_wait(wait_list, timeout);

        // 重新调度
        schedule_reschedule();
        restore_interrupt_state(int_save);
        let int_save = disable_interrupts();

        // 检查是否超时
        let task = get_current_task();
        if task.task_status.contains(TaskStatus::TIMEOUT) {
            task.task_status.remove(TaskStatus::TIMEOUT);
            restore_interrupt_state(int_save);
            return Err(QueueError::Timeout.into());
        }
    } else {
        // 减少可读/可写计数
        queue_cb.decrement_resource_count(operate_type);
    }

    // 执行队列缓冲区操作
    queue_buffer_operate(queue_cb, operate_type, buffer, buffer_size);

    // 检查是否有等待的任务需要唤醒
    if !queue_cb.is_opposite_wait_list_empty(operate_type) {
        // 唤醒等待的任务
        let resumed_task = TaskCB::from_pend_list(LinkedList::first(
            queue_cb.get_opposite_wait_list(operate_type),
        ));
        task_wake(resumed_task);

        // 恢复中断状态
        restore_interrupt_state(int_save);

        // 重新调度
        schedule();
    } else {
        // 增加对应的可读/可写计数
        queue_cb.increment_opposite_resource_count(operate_type);

        // 恢复中断状态
        restore_interrupt_state(int_save);
    }
    Ok(())
}
