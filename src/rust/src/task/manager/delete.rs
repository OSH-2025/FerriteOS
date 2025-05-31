use crate::{
    config::TASK_LIMIT,
    error::{SystemError, SystemResult, TaskError},
    interrupt::{int_lock, int_restore, is_int_active},
    mem::{defs::m_aucSysMem1, memory::los_mem_free, memstat::os_memstat_task_clear},
    percpu::can_preempt_in_scheduler,
    task::{
        global::{FREE_TASK_LIST, TASK_RECYCLE_LIST, get_tcb_from_id},
        sched::{priority_queue_remove, schedule_reschedule},
        timer::delete_from_timer_list,
        types::{TaskCB, TaskFlags, TaskSignal, TaskStatus},
    },
    utils::list::LinkedList,
};
use core::ptr::null_mut;

/// 处理正在运行任务的删除操作
fn handle_running_task_deletion(task_cb: &mut TaskCB) {
    // 获取特殊任务控制块
    let run_task = get_tcb_from_id(TASK_LIMIT);
    // 保存任务信息
    run_task.task_id = task_cb.task_id;
    run_task.task_status = task_cb.task_status;
    run_task.top_of_stack = task_cb.top_of_stack;
    run_task.task_name = task_cb.task_name;
    // 标记为未使用
    task_cb.task_status = TaskStatus::UNUSED;
}

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
        // 处理运行中任务的删除
        handle_running_task_deletion(task_cb);
        return true;
    } else {
        // 处理非运行状态的任务删除
        task_cb.task_status = TaskStatus::UNUSED;
        LinkedList::insert(&raw mut FREE_TASK_LIST, &mut task_cb.pend_list);

        // 释放任务栈内存
        if !use_usr_stack {
            let task_stack = (*task_cb).top_of_stack;
            unsafe {
                los_mem_free(
                    m_aucSysMem1 as *mut core::ffi::c_void,
                    task_stack as *mut core::ffi::c_void,
                );
            }
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
    if is_int_active() {
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

    // 获取任务控制块
    let task_cb = get_tcb_from_id(task_id);

    if task_cb.task_flags.contains(TaskFlags::SYSTEM) {
        return Err(SystemError::Task(TaskError::OperateSystemTask));
    }

    // 锁定调度器
    let int_save = int_lock();

    // 获取任务状态
    let temp_status = task_cb.task_status;

    // 检查任务是否未创建
    if temp_status.contains(TaskStatus::UNUSED) {
        int_restore(int_save);
        return Err(SystemError::Task(TaskError::NotCreated));
    }

    // 如果任务正在运行
    if temp_status.contains(TaskStatus::RUNNING) {
        // 检查是否可以删除
        match can_delete_running_task(task_cb) {
            Ok(true) => {}
            Ok(false) => {
                int_restore(int_save);
                return Ok(());
            }
            Err(err) => {
                int_restore(int_save);
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

    // 清除内存相关信息
    #[cfg(feature = "memory_task_statistics")]
    os_memstat_task_clear(task_id);

    // 执行任务删除操作，如果需要重新调度则执行
    if perform_task_deletion(task_cb, task_cb.usr_stack != 0) {
        schedule_reschedule();
    }

    // 解锁调度器
    int_restore(int_save);
    Ok(())
}
