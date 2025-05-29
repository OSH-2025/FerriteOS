/// 让当前任务让出CPU，允许同优先级的其他任务运行
pub fn task_yield() -> Result<(), TaskError> {
    // 检查是否在中断上下文
    if is_interrupt_active() {
        return Err(TaskError::YieldInInterrupt);
    }

    // 检查是否可以抢占
    if !can_preempt() {
        return Err(TaskError::YieldInLock);
    }

    // 获取当前运行的任务
    let run_task = get_current_task();

    unsafe {
        // 检查任务ID是否有效
        if (*run_task).task_id >= KERNEL_TSK_LIMIT {
            return Err(TaskError::InvalidId);
        }

        // 锁定调度器
        let int_save = int_lock();

        // 重置时间片
        #[cfg(feature = "base_core_timeslice")]
        {
            (*run_task).time_slice = 0;
        }

        // 获取同优先级任务数量
        let tsk_count = pri_queue_size((*run_task).priority);

        // 如果有其他同优先级任务，将当前任务加入就绪队列
        if tsk_count > 0 {
            (*run_task).task_status.insert(TaskStatus::READY);
            pri_queue_enqueue(&mut (*run_task).pend_list, (*run_task).priority);

            // 触发重新调度
            os_sched_resched();

            // 解锁调度器
            int_restore(int_save);
        } else {
            // 没有其他同优先级任务，解锁并返回错误
            int_restore(int_save);
            return Err(TaskError::YieldNotEnoughTask);
        }
    }

    Ok(())
}

/// C兼容的任务让出函数
#[no_mangle]
pub extern "C" fn LOS_TaskYield() -> u32 {
    match task_yield() {
        Ok(()) => OK,
        Err(err) => err.into(),
    }
}
