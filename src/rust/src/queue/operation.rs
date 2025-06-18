//! 消息队列操作功能实现
use crate::config::QUEUE_LIMIT;
use crate::ffi::bindings::get_current_task;
use crate::interrupt::is_interrupt_active;
use crate::percpu::can_preempt_in_scheduler;
use crate::queue::error::QueueError;
use crate::queue::global::QUEUE_POOL;
use crate::queue::types::{QueueControlBlock, QueueId, QueueOperationType};
use crate::result::SystemResult;
use crate::task::sched::{schedule, schedule_reschedule};
use crate::task::sync::wait::{task_wait, task_wake};
use crate::task::types::{TaskCB, TaskStatus};
use crate::utils::list::LinkedList;
use critical_section::with;

/// 从队列读取数据
pub fn queue_read(queue_id: QueueId, buffer: &mut [u8], timeout: u32) -> SystemResult<usize> {
    let mut size = buffer.len();
    // 检查参数
    check_queue_read_parameters(queue_id, size, timeout)?;
    // 创建读操作类型
    let operate_type = QueueOperationType::ReadHead;
    // 执行队列操作
    queue_operate(queue_id, operate_type, buffer, &mut size, timeout)?;
    Ok(size)
}

/// 从队列头部写入数据
pub fn queue_write_head(queue_id: QueueId, buffer: &mut [u8], timeout: u32) -> SystemResult<()> {
    // 检查参数
    let mut size = buffer.len();
    check_queue_write_parameters(queue_id, size, timeout)?;

    // 创建写操作类型
    let operate_type = QueueOperationType::WriteHead;

    // 执行队列操作
    queue_operate(queue_id, operate_type, buffer, &mut size, timeout)
}

/// 从队列尾部写入数据
pub fn queue_write(queue_id: QueueId, buffer: &mut [u8], timeout: u32) -> SystemResult<()> {
    // 检查参数
    let mut size = buffer.len();
    check_queue_write_parameters(queue_id, size, timeout)?;

    // 创建写操作类型
    let operate_type = QueueOperationType::WriteTail;

    // 执行队列操作
    queue_operate(queue_id, operate_type, buffer, &mut size, timeout)
}

/// 检查队列读取参数
fn check_queue_read_parameters(
    queue_id: QueueId,
    buffer_size: usize,
    timeout: u32,
) -> SystemResult<()> {
    // 检查队列ID是否有效
    if queue_id.get_index() as u32 >= QUEUE_LIMIT {
        return Err(QueueError::Invalid.into());
    }

    // 检查缓冲区大小是否有效
    if buffer_size == 0 || buffer_size > (usize::MAX - QueueControlBlock::MESSAGE_LEN_BYTES) {
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
    buffer_size: usize,
    timeout: u32,
) -> SystemResult<()> {
    // 检查队列ID是否有效
    if queue_id.get_index() as u32 >= QUEUE_LIMIT {
        return Err(QueueError::Invalid.into());
    }

    // 检查缓冲区大小是否为零
    if buffer_size == 0 {
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
    buffer: &mut [u8],
    buffer_size: &mut usize,
) {
    // 根据操作类型获取队列位置并更新队列头/尾指针
    match operate_type {
        QueueOperationType::ReadHead => {
            queue_cb.dequeue_front(buffer, buffer_size);
        }
        QueueOperationType::WriteHead => {
            queue_cb.enqueue_front(buffer);
        }
        QueueOperationType::WriteTail => {
            queue_cb.enqueue_back(buffer);
        }
    };
}

/// 检查队列操作参数
fn check_queue_operate_params(
    queue_cb: &QueueControlBlock,
    queue_id: QueueId,
    operate_type: QueueOperationType,
    buffer_size: usize,
) -> SystemResult<()> {
    // 检查队列是否存在且有效
    if !queue_cb.matches_id(queue_id) || queue_cb.is_unused() {
        return Err(QueueError::NotCreate.into());
    }

    // 检查缓冲区大小是否适合操作类型
    if operate_type.is_read() {
        if buffer_size < (queue_cb.get_slot_size() - QueueControlBlock::MESSAGE_LEN_BYTES) {
            return Err(QueueError::ReadSizeTooSmall.into());
        }
    } else if buffer_size > (queue_cb.get_slot_size() - QueueControlBlock::MESSAGE_LEN_BYTES) {
        return Err(QueueError::WriteSizeTooBig.into());
    }

    Ok(())
}

/// 队列操作核心函数
fn queue_operate(
    queue_id: QueueId,
    operate_type: QueueOperationType,
    buffer: &mut [u8],
    buffer_size: &mut usize,
    timeout: u32,
) -> SystemResult<()> {
    let index: u16 = queue_id.get_index();
    let res = with(|cs| {
        let mut queue_pool = QUEUE_POOL.borrow_ref_mut(cs);
        let mut queue = queue_pool.get_mut(index as usize).unwrap();
        // 检查队列操作参数
        match check_queue_operate_params(queue, queue_id, operate_type, *buffer_size) {
            Ok(_) => {}
            Err(e) => {
                return Err(e);
            }
        };
        // 检查是否有可读/可写资源
        if queue.has_available_resources(operate_type) {
            // 减少可读/可写计数
            queue.decrement_resource_count(operate_type);
        } else {
            // 如果没有等待时间，直接返回队列空或满错误
            if timeout == 0 {
                return Err(if operate_type.is_read() {
                    QueueError::IsEmpty.into()
                } else {
                    QueueError::IsFull.into()
                });
            }

            // 检查是否可以在调度锁中等待
            if !can_preempt_in_scheduler() {
                return Err(QueueError::PendInLock.into());
            }

            // 让当前任务等待队列
            let wait_list = queue.get_wait_list(operate_type);

            task_wait(wait_list, timeout);
            drop(queue_pool);

            // 重新调度
            schedule_reschedule();

            // 检查是否超时
            let task = get_current_task();
            if task.task_status.contains(TaskStatus::TIMEOUT) {
                task.task_status.remove(TaskStatus::TIMEOUT);
                return Err(QueueError::Timeout.into());
            }
            queue_pool = QUEUE_POOL.borrow_ref_mut(cs);
            queue = queue_pool.get_mut(index as usize).unwrap();
        }
        // 执行队列缓冲区操作
        queue_buffer_operate(queue, operate_type, buffer, buffer_size);

        // 检查是否有等待的任务需要唤醒
        if !queue.is_opposite_wait_list_empty(operate_type) {
            // 唤醒等待的任务
            let resumed_task = TaskCB::from_pend_list(LinkedList::first(
                queue.get_opposite_wait_list(operate_type),
            ));
            task_wake(resumed_task);
            Ok(true)
        } else {
            // 增加对应的可读/可写计数
            queue.increment_opposite_resource_count(operate_type);
            Ok(false)
        }
    });
    match res {
        Ok(need_schedule) => {
            // 如果需要调度，则执行调度
            if need_schedule {
                schedule();
            }
            Ok(())
        }
        Err(e) => Err(e),
    }
}
