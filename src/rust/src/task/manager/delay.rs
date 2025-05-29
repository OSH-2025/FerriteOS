use crate::{
    ffi::bindings::get_current_task,
    hwi::{int_lock, int_restore, is_int_active},
    percpu::can_preempt,
    task::{
        sched::{priority_queue_get_size, priority_queue_insert_at_back, schedule_reschedule},
        timer::add_to_timer_list,
        types::{TaskError, TaskFlags, TaskStatus},
    },
};

/// 使当前任务延时指定的tick数
pub fn task_delay(tick: u32) -> Result<(), TaskError> {
    // 检查是否在中断上下文
    if is_int_active() {
        return Err(TaskError::DelayInInterrupt);
    }

    // 获取当前运行的任务
    let run_task = get_current_task();

    // 检查是否是系统任务
    if run_task.task_flags.contains(TaskFlags::SYSTEM) {
        return Err(TaskError::OperateSystemTask);
    }

    // 检查是否可以抢占
    if !can_preempt() {
        return Err(TaskError::DelayInLock);
    }

    // 如果tick为0，则调用task_yield函数让出CPU
    if tick == 0 {
        return task_yield();
    }

    // 延时处理
    // 锁定调度器
    let int_save = int_lock();

    // 将任务添加到定时器列表
    add_to_timer_list(run_task, tick);

    // 设置任务状态为延时
    run_task.task_status.insert(TaskStatus::DELAY);

    // 触发调度
    schedule_reschedule();

    // 解锁调度器
    int_restore(int_save);

    Ok(())
}

/// 让当前任务让出CPU，允许同优先级的其他任务运行
pub fn task_yield() -> Result<(), TaskError> {
    // 检查是否在中断上下文
    if is_int_active() {
        return Err(TaskError::YieldInInterrupt);
    }

    // 检查是否可以抢占
    if !can_preempt() {
        return Err(TaskError::YieldInLock);
    }

    // 获取当前运行的任务
    let run_task = get_current_task();

    // 锁定调度器
    let int_save = int_lock();

    // 重置时间片
    #[cfg(feature = "timeslice")]
    {
        run_task.time_slice = 0;
    }

    // 获取同优先级任务数量
    let tsk_count = priority_queue_get_size(run_task.priority);

    // 如果有其他同优先级任务，将当前任务加入就绪队列
    if tsk_count > 0 {
        run_task.task_status.insert(TaskStatus::READY);
        priority_queue_insert_at_back(&mut run_task.pend_list, run_task.priority as u32);

        // 触发重新调度
        schedule_reschedule();

        // 解锁调度器
        int_restore(int_save);
        Ok(())
    } else {
        // 没有其他同优先级任务，解锁并返回错误
        int_restore(int_save);
        Err(TaskError::YieldNotEnoughTask)
    }
}
