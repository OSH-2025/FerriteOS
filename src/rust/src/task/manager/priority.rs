use crate::{
    config::TASK_PRIORITY_LOWEST,
    ffi::bindings::get_current_task,
    hwi::{int_lock, int_restore},
    task::{
        global::get_tcb_from_id,
        sched::{priority_queue_insert_at_back, priority_queue_remove, schedule},
        types::{TaskError, TaskFlags, TaskStatus},
    },
};

/// 获取指定任务的优先级
pub fn get_task_priority(task_id: u32) -> Result<u16, TaskError> {
    // 检查任务ID是否有效
    if task_id >= crate::config::TASK_LIMIT {
        return Err(TaskError::InvalidId);
    }

    // 获取任务控制块
    let task_cb = get_tcb_from_id(task_id);

    // 锁定调度器
    let int_save = int_lock();

    // 检查任务是否已创建
    let result = {
        if task_cb.task_status.contains(TaskStatus::UNUSED) {
            Err(TaskError::NotCreated)
        } else {
            // 获取优先级
            let priority = task_cb.priority;
            Ok(priority)
        }
    };

    // 解锁调度器
    int_restore(int_save);

    result
}

/// 设置指定任务的优先级
pub fn set_task_priority(task_id: u32, priority: u16) -> Result<(), TaskError> {
    // 检查优先级是否有效
    if priority > TASK_PRIORITY_LOWEST {
        return Err(TaskError::PriorityError);
    }

    // 检查任务ID是否有效
    if task_id >= crate::config::TASK_LIMIT {
        return Err(TaskError::InvalidId);
    }

    // 获取任务控制块
    let task_cb = get_tcb_from_id(task_id);

    // 检查是否为系统任务
    if task_cb.task_flags.contains(TaskFlags::SYSTEM) {
        return Err(TaskError::OperateSystemTask);
    }

    // 锁定调度器
    let int_save = int_lock();

    // 检查任务是否已创建
    let temp_status = task_cb.task_status;
    if temp_status.contains(TaskStatus::UNUSED) {
        int_restore(int_save);
        return Err(TaskError::NotCreated);
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
    int_restore(int_save);

    // 如果需要重新调度，触发调度
    if needs_reschedule {
        schedule();
    }

    Ok(())
}

pub fn set_current_task_priority(priority: u16) -> Result<(), TaskError> {
    let current_task = get_current_task();
    let task_id = current_task.task_id;
    set_task_priority(task_id, priority)
}
