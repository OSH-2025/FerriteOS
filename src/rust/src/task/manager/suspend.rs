use crate::{
    config::TASK_LIMIT,
    result::{SystemError, SystemResult, TaskError},
    ffi::bindings::get_current_task,
    interrupt::{disable_interrupts, restore_interrupt_state, is_int_active},
    percpu::can_preempt_in_scheduler,
    task::{
        global::{get_tcb_from_id, is_scheduler_active},
        sched::{
            priority_queue_insert_at_back, priority_queue_remove, schedule, schedule_reschedule,
        },
        types::{TaskCB, TaskFlags, TaskSignal, TaskStatus},
    },
};

/// 恢复一个被挂起的任务
pub fn task_resume(task_id: u32) -> SystemResult<()> {
    // 检查任务ID是否有效
    if task_id >= TASK_LIMIT {
        return Err(SystemError::Task(TaskError::InvalidId));
    }

    // 获取任务控制块
    let task_cb = get_tcb_from_id(task_id);

    // 锁定调度器
    let int_save = disable_interrupts();

    // 清除挂起信号
    task_cb.signal.remove(TaskSignal::SUSPEND);

    // 获取当前任务状态
    let temp_status = task_cb.task_status;

    // 检查任务是否已创建
    if temp_status.contains(TaskStatus::UNUSED) {
        restore_interrupt_state(int_save);
        return Err(SystemError::Task(TaskError::NotCreated));
    }

    // 检查任务是否已挂起
    if !temp_status.contains(TaskStatus::SUSPEND) {
        restore_interrupt_state(int_save);
        return Err(SystemError::Task(TaskError::NotSuspended));
    }

    // 清除挂起状态
    task_cb.task_status.remove(TaskStatus::SUSPEND);

    let mut need_sched = false;

    // 如果任务没有被其他原因阻塞，则将其设置为就绪状态并加入就绪队列
    if !task_cb.task_status.intersects(TaskStatus::BLOCKED) {
        task_cb.task_status.insert(TaskStatus::READY);
        priority_queue_insert_at_back(&mut task_cb.pend_list, task_cb.priority as u32);

        // 检查调度器是否活动
        if is_scheduler_active() {
            need_sched = true;
        }
    }

    // 解锁调度器
    restore_interrupt_state(int_save);

    // 如果需要调度，则触发调度
    if need_sched {
        schedule();
    }

    Ok(())
}

/// 检查是否可以挂起正在运行的任务
fn can_suspend_running_task(task_cb: &mut TaskCB) -> SystemResult<bool> {
    // 检查调度器是否可抢占
    if !can_preempt_in_scheduler() {
        // 当前核心的运行任务无法挂起
        return Err(SystemError::Task(TaskError::SuspendLocked));
    }

    // 检查是否在中断上下文
    if is_int_active() {
        // 在中断中挂起任务，设置挂起信号
        task_cb.signal = TaskSignal::SUSPEND;
        return Ok(false);
    }

    // 可以挂起
    Ok(true)
}

/// 挂起任务
///
/// # Arguments
/// * `task_id` - 要挂起的任务ID
///
/// # Returns
/// * `Ok(())` - 成功挂起任务
/// * `Err(TaskError)` - 挂起任务失败，包含具体错误原因
pub fn task_suspend(task_id: u32) -> SystemResult<()> {
    // 检查任务ID是否有效
    if task_id >= TASK_LIMIT {
        return Err(SystemError::Task(TaskError::InvalidId));
    }

    // 获取任务控制块
    let task_cb = get_tcb_from_id(task_id);

    // 检查是否为系统任务
    if task_cb.task_flags.contains(TaskFlags::SYSTEM) {
        return Err(SystemError::Task(TaskError::OperateSystemTask));
    }

    // 锁定调度器
    let int_save = disable_interrupts();

    // 获取当前任务状态
    let temp_status = task_cb.task_status;

    // 检查任务是否已创建
    if temp_status.contains(TaskStatus::UNUSED) {
        restore_interrupt_state(int_save);
        return Err(SystemError::Task(TaskError::NotCreated));
    }

    // 检查任务是否已挂起
    if temp_status.contains(TaskStatus::SUSPEND) {
        restore_interrupt_state(int_save);
        return Err(SystemError::Task(TaskError::AlreadySuspended));
    }

    // 如果任务正在运行，检查是否可以挂起
    if temp_status.contains(TaskStatus::RUNNING) {
        match can_suspend_running_task(task_cb) {
            Ok(true) => {}
            Ok(false) => {
                restore_interrupt_state(int_save);
                return Ok(());
            }
            Err(err) => {
                restore_interrupt_state(int_save);
                return Err(err);
            }
        }
    }

    // 如果任务处于就绪状态，从就绪队列中移除
    if temp_status.contains(TaskStatus::READY) {
        priority_queue_remove(&mut task_cb.pend_list);
        task_cb.task_status.remove(TaskStatus::READY);
    }

    // 设置任务为挂起状态
    task_cb.task_status.insert(TaskStatus::SUSPEND);

    // 获取当前运行任务
    let run_task = get_current_task();

    // 如果挂起的是当前运行任务，则触发调度
    if task_id == run_task.task_id {
        schedule_reschedule();
    }

    // 解锁调度器
    restore_interrupt_state(int_save);

    Ok(())
}
