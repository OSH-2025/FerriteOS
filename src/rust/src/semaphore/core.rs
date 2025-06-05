use crate::{
    ffi::bindings::get_current_task,
    interrupt::{disable_interrupts, is_interrupt_active, restore_interrupt_state},
    percpu::can_preempt,
    println_debug,
    result::SystemResult,
    semaphore::{
        error::SemaphoreError,
        global::SemaphoreManager,
        types::{SemaphoreId, SemaphoreType},
    },
    task::{
        sched::{schedule, schedule_reschedule},
        sync::wait::{task_wait, task_wake},
        types::{TaskCB, TaskStatus},
    },
    utils::list::LinkedList,
};

/// 初始化信号量系统
pub fn init_semaphore_system() {
    SemaphoreManager::initialize();
    #[cfg(feature = "debug-semaphore")]
    {
        todo!("Debug semaphore initialization");
    }
}

/// 创建计数信号量
pub fn create_semaphore(count: u16) -> SystemResult<SemaphoreId> {
    if count > SemaphoreType::Counting.max_count() {
        return Err(SemaphoreError::Overflow.into());
    }
    let int_save = disable_interrupts();
    match SemaphoreManager::allocate(SemaphoreType::Counting, count) {
        Ok(semaphore_id) => {
            #[cfg(feature = "debug-semaphore")]
            {
                todo!("Semaphore created with ID: {:?}", semaphore_id);
            }
            restore_interrupt_state(int_save);
            Ok(semaphore_id)
        }
        Err(e) => {
            #[cfg(feature = "debug-semaphore")]
            {
                todo!("No available semaphore to allocate");
            }
            restore_interrupt_state(int_save);
            Err(e)
        }
    }
}

/// 创建二进制信号量
pub fn create_binary_semaphore(count: u16) -> SystemResult<SemaphoreId> {
    if count > SemaphoreType::Binary.max_count() {
        return Err(SemaphoreError::Overflow.into());
    }
    let int_save = disable_interrupts();
    match SemaphoreManager::allocate(SemaphoreType::Counting, count) {
        Ok(semaphore_id) => {
            #[cfg(feature = "debug-semaphore")]
            {
                todo!("Semaphore created with ID: {:?}", semaphore_id);
            }
            restore_interrupt_state(int_save);
            Ok(semaphore_id)
        }
        Err(e) => {
            #[cfg(feature = "debug-semaphore")]
            {
                todo!("No available semaphore to allocate");
            }
            restore_interrupt_state(int_save);
            Err(e)
        }
    }
}

/// 删除信号量
pub fn delete_semaphore(id: SemaphoreId) -> SystemResult<()> {
    let int_save = disable_interrupts();
    match SemaphoreManager::deallocate(id) {
        Ok(_) => {
            #[cfg(feature = "debug-semaphore")]
            {
                todo!("Semaphore deleted with ID: {:?}", handle);
            }
            restore_interrupt_state(int_save);
            Ok(())
        }
        Err(e) => {
            restore_interrupt_state(int_save);
            Err(e)
        }
    }
}

/// 等待信号量
pub fn semaphore_pend(handle: SemaphoreId, timeout: u32) -> SystemResult<()> {
    let semaphore = SemaphoreManager::get_semaphore(handle)?;

    if is_interrupt_active() {
        return Err(SemaphoreError::PendInInterrupt.into());
    }

    if !can_preempt() {
        return Err(SemaphoreError::PendInLock.into());
    }

    let run_task = get_current_task();

    if run_task.is_system_task() {
        println_debug!("DO NOT recommend to use semaphore in system tasks.");
    }

    let int_save = disable_interrupts();

    if semaphore.is_unused() || !semaphore.matches_id(handle) {
        restore_interrupt_state(int_save);
        return Err(SemaphoreError::Invalid.into());
    }

    #[cfg(feature = "debug-semaphore")]
    {
        todo!("Semaphore pend called for ID: {:?}", handle);
    }

    if semaphore.get_count() > 0 {
        semaphore.decrement_count();
        restore_interrupt_state(int_save);
        return Ok(());
    }
    if timeout == 0 {
        restore_interrupt_state(int_save);
        return Err(SemaphoreError::Unavailable.into());
    }
    task_wait(&mut semaphore.sem_list, timeout);
    schedule_reschedule();

    restore_interrupt_state(int_save);

    let int_save = disable_interrupts();

    if run_task.task_status.contains(TaskStatus::TIMEOUT) {
        run_task.task_status.remove(TaskStatus::TIMEOUT);
        restore_interrupt_state(int_save);
        Err(SemaphoreError::Timeout.into())
    } else {
        restore_interrupt_state(int_save);
        Ok(())
    }
}

/// 释放信号量
pub fn semaphore_post(handle: SemaphoreId) -> SystemResult<()> {
    let semaphore = SemaphoreManager::get_semaphore(handle)?;

    let int_save = disable_interrupts();

    if semaphore.is_unused() || !semaphore.matches_id(handle) {
        restore_interrupt_state(int_save);
        return Err(SemaphoreError::Invalid.into());
    }

    #[cfg(feature = "debug-semaphore")]
    {
        todo!("Semaphore post called for ID: {:?}", handle);
    }

    let max_count = semaphore.max_count();

    if semaphore.get_count() >= max_count {
        restore_interrupt_state(int_save);
        return Err(SemaphoreError::Overflow.into());
    }

    if semaphore.has_waiting_tasks() {
        let first_node = LinkedList::first(&raw const semaphore.sem_list);
        let resumed_task = TaskCB::from_pend_list(first_node);
        task_wake(resumed_task);
        restore_interrupt_state(int_save);
        schedule();
    } else {
        semaphore.increment_count();
    }

    Ok(())
}
