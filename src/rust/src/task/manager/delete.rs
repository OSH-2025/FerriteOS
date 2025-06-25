use crate::{
    config::TASK_LIMIT,
    interrupt::{disable_interrupts, is_interrupt_active, restore_interrupt_state},
    memory::free,
    percpu::can_preempt_in_scheduler,
    result::{SystemError, SystemResult},
    task::{
        error::TaskError,
        global::{FREE_TASK_LIST, TASK_RECYCLE_LIST, get_tcb_from_id},
        sched::{priority_queue_remove, schedule_reschedule},
        timer::delete_from_timer_list,
        types::{TaskCB, TaskSignal, TaskStatus},
    },
    utils::list::LinkedList,
};
use core::{ffi::c_void, ptr::null_mut};

/// 执行任务删除操作
fn perform_task_deletion(task_cb: &mut TaskCB, use_usr_stack: bool) -> bool {
    // 检查任务是否在运行中
    if task_cb.task_status.contains(TaskStatus::RUNNING) {
        #[cfg(feature = "task_static_allocation")]
        {
            if use_usr_stack {
                LinkedList::insert(&raw mut FREE_TASK_LIST, &mut task_cb.pend_list);
            } else {
                LinkedList::tail_insert(&raw mut TASK_RECYCLE_LIST, &mut task_cb.pend_list);
            }
        }
        #[cfg(not(feature = "task_static_allocation"))]
        {
            LinkedList::tail_insert(&raw mut TASK_RECYCLE_LIST, &mut task_cb.pend_list);
        }
        return true;
    } else {
        // 处理非运行状态的任务删除
        task_cb.task_status = TaskStatus::UNUSED;
        LinkedList::insert(&raw mut FREE_TASK_LIST, &mut task_cb.pend_list);

        // 释放任务栈内存
        if !use_usr_stack {
            let task_stack = (*task_cb).top_of_stack;
            free(task_stack as *mut c_void);
        }

        task_cb.top_of_stack = null_mut();
        return false;
    }
}

/// 检查是否可以删除运行中的任务
fn can_delete_running_task(task_cb: &mut TaskCB) -> SystemResult<bool> {
    // 检查调度器是否可抢占
    if !can_preempt_in_scheduler() {
        // 如果任务正在运行且调度器被锁定，则不能删除
        return Err(SystemError::Task(TaskError::DeleteLocked));
    }
    // 检查是否在中断上下文
    if is_interrupt_active() {
        task_cb.signal = TaskSignal::KILL;
        return Ok(false);
    }
    // 可以删除
    Ok(true)
}

/// 删除任务
pub fn task_delete(task_id: u32) -> SystemResult<()> {
    // 检查任务ID是否有效
    if task_id >= TASK_LIMIT {
        return Err(SystemError::Task(TaskError::InvalidId));
    }

    // 锁定调度器
    let int_save = disable_interrupts();

    // 获取任务控制块
    let task_cb = get_tcb_from_id(task_id);

    if task_cb.is_system_task() {
        return Err(SystemError::Task(TaskError::OperateSystemTask));
    }

    // 获取任务状态
    let temp_status = task_cb.task_status;

    // 检查任务是否未创建
    if temp_status.contains(TaskStatus::UNUSED) {
        restore_interrupt_state(int_save);
        return Err(SystemError::Task(TaskError::NotCreated));
    }

    // 如果任务正在运行
    if temp_status.contains(TaskStatus::RUNNING) {
        // 检查是否可以删除
        match can_delete_running_task(task_cb) {
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

    // 从相应队列中移除任务
    if temp_status.contains(TaskStatus::READY) {
        priority_queue_remove(&mut task_cb.pend_list);
        task_cb.task_status.remove(TaskStatus::READY);
    } else if temp_status.contains(TaskStatus::PEND) {
        LinkedList::remove(&mut task_cb.pend_list);
    }

    // 如果任务在延时列表中，将其移除
    if temp_status.intersects(TaskStatus::DELAY | TaskStatus::PEND_TIME) {
        delete_from_timer_list(task_cb);
    }

    // 清除挂起状态，设置为未使用状态
    task_cb.task_status.remove(TaskStatus::SUSPEND);
    task_cb.task_status.insert(TaskStatus::UNUSED);

    // 清除事件相关信息
    task_cb.event.event_id = u32::MAX;
    task_cb.event_mask = 0;

    // 执行任务删除操作，如果需要重新调度则执行
    if perform_task_deletion(task_cb, task_cb.usr_stack != 0) {
        schedule_reschedule();
    }

    // 解锁调度器
    restore_interrupt_state(int_save);
    Ok(())
}
