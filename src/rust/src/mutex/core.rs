use crate::{
    config::WAIT_FOREVER,
    ffi::bindings::get_current_task,
    interrupt::{disable_interrupts, is_interrupt_active, restore_interrupt_state},
    percpu::can_preempt_in_scheduler,
    println_debug,
    result::SystemResult,
    task::{
        sched::{schedule, schedule_reschedule},
        sync::wait::{task_wait, task_wake},
        types::{TaskCB, TaskStatus},
    },
};

use super::{
    error::MutexError,
    global::MutexManager,
    priority::PriorityInheritance,
    types::{MutexControlBlock, MutexId},
    wait::WaitManager,
};

fn pend_operation(
    run_task: &mut TaskCB,
    mutex: &mut MutexControlBlock,
    timeout: u32,
    int_save: &mut u32,
) -> SystemResult<()> {
    // 找到合适的等待位置
    let wait_pos = WaitManager::find_wait_position(run_task, mutex);

    // 将任务加入等待队列
    task_wait(wait_pos, timeout);

    // 立即调度
    schedule_reschedule();

    // 解锁并重新加锁
    restore_interrupt_state(*int_save);

    *int_save = disable_interrupts();

    // 检查是否超时
    if run_task.task_status.contains(TaskStatus::TIMEOUT) {
        run_task.task_status.remove(TaskStatus::TIMEOUT);
        // 如果不是永久等待，需要恢复优先级
        if timeout != WAIT_FOREVER {
            PriorityInheritance::restore_priority_on_timeout(run_task, mutex.get_owner());
        }
        return Err(MutexError::Timeout.into());
    } else {
        if timeout != WAIT_FOREVER {
            PriorityInheritance::restore_priority_on_timeout(run_task, mutex.get_owner());
        }
        return Ok(());
    }
}

/// 互斥锁释放操作实现
fn post_operation(run_task: &mut TaskCB, mutex: &mut MutexControlBlock) -> bool {
    if !mutex.has_waiting_tasks() {
        mutex.clear_owner();
        #[cfg(feature = "debug-mutex-deadlock")]
        {
            todo!("Implement debug deadlock handling");
        }
        return false;
    }
    // // 获取第一个等待的任务

    let resumed_task = TaskCB::from_pend_list(mutex.mux_list.next);

    // 处理优先级继承
    PriorityInheritance::handle_mutex_post(run_task, resumed_task, mutex);

    // 设置新的所有者
    mutex.set_count(1);
    mutex.set_owner(resumed_task);

    #[cfg(feature = "debug-mutex-deadlock")]
    {
        todo!("Implement debug deadlock handling");
    }

    // 唤醒任务
    task_wake(resumed_task);

    true
}

/// 互斥锁系统初始化
pub fn mutex_init() {
    MutexManager::initialize();
    #[cfg(feature = "debug-mutex")]
    {
        crate::mutex::debug::init_mutex_debug();
    }
}

/// 创建互斥锁
pub fn mutex_create() -> SystemResult<MutexId> {
    let int_save = disable_interrupts();

    if !MutexManager::has_available_mutex() {
        restore_interrupt_state(int_save);
        #[cfg(feature = "debug-mutex")]
        {
            crate::mutex::debug::check_mutex_usage();
        }
        return Err(MutexError::AllBusy.into());
    };
    let id = MutexManager::allocate();

    #[cfg(feature = "debug-mutex")]
    {
        if let Some(current) = current_task() {
            crate::mutex::debug::update_mutex_creator(
                handle,
                Some(unsafe { (*current).task_entry }),
            );
        }
    }
    restore_interrupt_state(int_save);
    Ok(id)
}

/// 删除互斥锁
pub fn mutex_delete(id: MutexId) -> SystemResult<()> {
    // 验证句柄
    let delete_mutex = MutexManager::get_mutex_mut(id)?;

    let int_save = disable_interrupts();

    let result = MutexManager::deallocate(delete_mutex, id);

    match result {
        Ok(()) => {
            #[cfg(feature = "debug-mutex")]
            {
                crate::mutex::debug::update_mutex_creator(handle, None);
            }
            restore_interrupt_state(int_save);
            Ok(())
        }
        Err(e) => {
            restore_interrupt_state(int_save);
            return Err(e);
        }
    }
}

/// 获取互斥锁（加锁）
pub fn mutex_pend(id: MutexId, timeout: u32) -> SystemResult<()> {
    let mutex = MutexManager::get_mutex_mut(id)?;

    let mut int_save = disable_interrupts();

    let run_task = get_current_task();

    if run_task.is_system_task() {
        println_debug!("DO NOT recommend to use mutex_lock in system tasks.");
    }

    if mutex.is_unused() || !mutex.matches_id(id) {
        return Err(MutexError::Invalid.into());
    }

    #[cfg(feature = "debug-mutex")]
    {
        todo!("Implement mutex debug logging");
    }

    if is_interrupt_active() {
        return Err(MutexError::PendInterrupt.into());
    }

    // 如果互斥锁未被锁定
    if mutex.get_count() == 0 {
        #[cfg(feature = "debug-mutex-deadlock")]
        {
            crate::mutex::debug::add_deadlock_node(unsafe { (*current).task_id }, mutex);
        }

        mutex.increment_count();
        mutex.set_owner(run_task);
        restore_interrupt_state(int_save);
        return Ok(());
    }

    // 如果当前任务已经是所有者（递归锁定）
    if mutex.is_owner(run_task) {
        mutex.increment_count();
        restore_interrupt_state(int_save);
        return Ok(());
    }

    // 如果不等待
    if timeout == 0 {
        restore_interrupt_state(int_save);
        return Err(MutexError::Unavailable.into());
    }

    // 检查是否可以调度
    if !can_preempt_in_scheduler() {
        restore_interrupt_state(int_save);
        return Err(MutexError::PendInLock.into());
    }

    // 设置优先级继承
    PriorityInheritance::handle_mutex_pend(run_task, mutex.get_owner());

    // 执行等待操作
    let result = pend_operation(run_task, mutex, timeout, &mut int_save);

    restore_interrupt_state(int_save);

    result
}

/// 释放互斥锁（解锁）
pub fn mutex_post(id: MutexId) -> SystemResult<()> {
    let mutex = MutexManager::get_mutex_mut(id)?;

    let int_save = disable_interrupts();

    let run_task = get_current_task();

    // 参数检查
    if mutex.is_unused() || !mutex.matches_id(id) {
        return Err(MutexError::Invalid.into());
    }

    #[cfg(feature = "debug-mutex")]
    {
        todo!("Implement mutex debug logging");
    }

    if is_interrupt_active() {
        return Err(MutexError::PendInterrupt.into());
    }

    // 检查所有者
    if mutex.get_count() == 0 || !mutex.is_owner(run_task) {
        restore_interrupt_state(int_save);
        return Err(MutexError::Invalid.into());
    }

    // 递减计数
    if mutex.decrement_count() != 0 {
        restore_interrupt_state(int_save);
        return Ok(());
    }

    // 执行释放操作
    let need_schedule = post_operation(run_task, mutex);

    restore_interrupt_state(int_save);

    // 如果需要调度
    if need_schedule {
        schedule();
    }

    Ok(())
}
