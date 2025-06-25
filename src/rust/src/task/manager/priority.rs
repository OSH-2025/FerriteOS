use crate::{
    config::TASK_PRIORITY_LOWEST,
    ffi::bindings::get_current_task,
    interrupt::{disable_interrupts, restore_interrupt_state},
    result::{SystemError, SystemResult},
    task::{
        error::TaskError,
        global::get_tcb_from_id,
        sched::{priority_queue_insert_at_back, priority_queue_remove, schedule},
        types::{TaskCB, TaskStatus},
    },
};

/// 获取指定任务的优先级
pub fn get_task_priority(task_id: u32) -> SystemResult<u16> {
    // 检查任务ID是否有效
    if task_id >= crate::config::TASK_LIMIT {
        return Err(SystemError::Task(TaskError::InvalidId));
    }

    // 获取任务控制块
    let task_cb = get_tcb_from_id(task_id);

    // 锁定调度器
    let int_save = disable_interrupts();

    // 检查任务是否已创建
    let result = {
        if task_cb.task_status.contains(TaskStatus::UNUSED) {
            Err(SystemError::Task(TaskError::NotCreated))
        } else {
            // 获取优先级
            let priority = task_cb.priority;
            Ok(priority)
        }
    };

    // 解锁调度器
    restore_interrupt_state(int_save);

    result
}

/// 设置指定任务的优先级
pub fn set_task_priority(task_id: u32, priority: u16) -> SystemResult<()> {
    // 检查优先级是否有效
    if priority > TASK_PRIORITY_LOWEST {
        return Err(SystemError::Task(TaskError::PriorityError));
    }

    // 检查任务ID是否有效
    if task_id >= crate::config::TASK_LIMIT {
        return Err(SystemError::Task(TaskError::InvalidId));
    }

    // 获取任务控制块
    let task_cb = get_tcb_from_id(task_id);

    // 检查是否为系统任务
    if task_cb.is_system_task() {
        return Err(SystemError::Task(TaskError::OperateSystemTask));
    }

    // 锁定调度器
    let int_save = disable_interrupts();

    // 检查任务是否已创建
    let temp_status = task_cb.task_status;
    if temp_status.contains(TaskStatus::UNUSED) {
        restore_interrupt_state(int_save);
        return Err(SystemError::Task(TaskError::NotCreated));
    }

    // 标记是否需要重新调度
    let needs_reschedule = {
        if temp_status.contains(TaskStatus::READY) {
            // 从就绪队列中移除任务
            priority_queue_remove(&mut task_cb.pend_list);

            // 更新优先级
            task_cb.priority = priority;

            // 将任务重新加入就绪队列
            priority_queue_insert_at_back(&mut task_cb.pend_list, task_cb.priority as u32);

            true
        } else if temp_status.contains(TaskStatus::RUNNING) {
            task_cb.priority = priority;

            true
        } else {
            task_cb.priority = priority;

            false
        }
    };

    // 解锁调度器
    restore_interrupt_state(int_save);

    // 如果需要重新调度，触发调度
    if needs_reschedule {
        schedule();
    }

    Ok(())
}

pub fn set_current_task_priority(priority: u16) -> SystemResult<()> {
    let current_task = get_current_task();
    let task_id = current_task.task_id;
    set_task_priority(task_id, priority)
}

/// 直接修改任务优先级，无需额外检查
///
/// 这是一个低级别函数，供内部使用，不执行ID验证或其他安全检查
///
/// # Arguments
/// * `task_cb` - 任务控制块引用
/// * `priority` - 新的优先级值
///
/// # Safety
/// 调用者必须确保任务控制块有效且优先级值合法
pub fn modify_task_priority_raw(task_cb: &mut TaskCB, priority: u16) {
    if task_cb.task_status.contains(TaskStatus::READY) {
        // 从就绪队列中移除任务
        priority_queue_remove(&mut task_cb.pend_list);

        // 更新优先级
        task_cb.priority = priority;

        // 将任务重新加入就绪队列
        priority_queue_insert_at_back(&mut task_cb.pend_list, task_cb.priority as u32);
    } else {
        // 直接更新优先级
        task_cb.priority = priority;
    }
}
