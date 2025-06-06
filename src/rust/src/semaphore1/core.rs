use crate::{
    semaphore1::{
        bindings::{
            arch_int_lock, arch_int_restore, get_current_task, is_interrupt_context,
            is_preemptable, schedule, task_from_wait_list, task_wait, task_wait_abort,
            OS_TASK_FLAG_SYSTEM, OS_TASK_STATUS_TIMEOUT,
        },
        configs::{BINARY_SEMAPHORE_COUNT_MAX, COUNTING_SEMAPHORE_COUNT_MAX},
        error::{SemaphoreError, SemaphoreResult},
        global::SemaphoreManager,
        types::{SemaphoreId, SemaphoreType},
    },
    task::types::TaskStatus,
    println_debug,
    utils::list::LinkedList,
};

/// 初始化信号量系统
#[inline]
pub fn init_semaphore_system() {
    SemaphoreManager::initialize();
    
    #[cfg(feature = "debug-semaphore")]
    {
        println_debug!("Semaphore system initialized");
    }
}

/// 创建计数信号量
pub fn create_semaphore(count: u16) -> SemaphoreResult<SemaphoreId> {
    if count > COUNTING_SEMAPHORE_COUNT_MAX {
        return Err(SemaphoreError::Overflow.into());
    }
    
    // 保存中断状态并禁用中断
    let int_save = arch_int_lock();
    
    // 分配新的信号量
    let result = match SemaphoreManager::allocate(SemaphoreType::Counting, count) {
        Ok(semaphore_id) => {
            #[cfg(feature = "debug-semaphore")]
            {
                println_debug!("Counting semaphore created with ID: {:?}, count: {}", 
                          semaphore_id, count);
            }
            Ok(semaphore_id)
        }
        Err(e) => {
            #[cfg(feature = "debug-semaphore")]
            {
                println_debug!("Failed to create semaphore: {:?}", e);
            }
            Err(e)
        }
    };
    
    // 恢复中断状态
    arch_int_restore(int_save);
    
    result
}

/// 创建二进制信号量
pub fn create_binary_semaphore(count: u16) -> SemaphoreResult<SemaphoreId> {
    if count > BINARY_SEMAPHORE_COUNT_MAX {
        return Err(SemaphoreError::Overflow.into());
    }
    
    // 保存中断状态并禁用中断
    let int_save = arch_int_lock();
    
    // 分配新的信号量，使用Binary类型
    let result = match SemaphoreManager::allocate(SemaphoreType::Binary, count) {
        Ok(semaphore_id) => {
            #[cfg(feature = "debug-semaphore")]
            {
                println_debug!("Binary semaphore created with ID: {:?}, count: {}", 
                          semaphore_id, count);
            }
            Ok(semaphore_id)
        }
        Err(e) => {
            #[cfg(feature = "debug-semaphore")]
            {
                println_debug!("Failed to create binary semaphore: {:?}", e);
            }
            Err(e)
        }
    };
    
    // 恢复中断状态
    arch_int_restore(int_save);
    
    result
}

/// 删除信号量
pub fn delete_semaphore(semaphore_id: SemaphoreId) -> SemaphoreResult<()> {
    // 保存中断状态并禁用中断
    let int_save = arch_int_lock();
    
    // 删除信号量
    let result = SemaphoreManager::deallocate(semaphore_id);
    
    #[cfg(feature = "debug-semaphore")]
    {
        match &result {
            Ok(_) => println_debug!("Semaphore deleted: {:?}", semaphore_id),
            Err(e) => println_debug!("Failed to delete semaphore: {:?}", e),
        }
    }
    
    // 恢复中断状态
    arch_int_restore(int_save);
    
    result
}

/// 等待信号量(P操作)
pub fn semaphore_pend(semaphore_id: SemaphoreId, timeout: u32) -> SemaphoreResult<()> {
    // 判断是否在中断上下文中
    if is_interrupt_context() {
        return Err(SemaphoreError::PendInterrupt.into());
    }
    
    // 获取当前任务
    let current_task = get_current_task();
    
    // 判断是否是系统任务
    if current_task.is_system_task() {
        #[cfg(feature = "debug-semaphore")]
        {
            println_debug!("Warning: DO NOT recommend to use semaphore_pend in system tasks");
        }
    }
    
    // 判断任务是否可抢占
    if !is_preemptable() {
        return Err(SemaphoreError::PendInLock.into());
    }
    
    // 保存中断状态并禁用中断
    let int_save = arch_int_lock();
    
    // 获取信号量控制块
    let semaphore = match SemaphoreManager::get_semaphore(semaphore_id) {
        Ok(sem) => {
            if sem.is_unused() || !sem.matches_id(semaphore_id) {
                arch_int_restore(int_save);
                return Err(SemaphoreError::Invalid.into());
            }
            sem
        }
        Err(e) => {
            arch_int_restore(int_save);
            return Err(e);
        }
    };
    
    #[cfg(feature = "debug-semaphore")]
    {
        println_debug!("Semaphore pend: {:?}, current count: {}", 
                  semaphore_id, semaphore.get_count());
    }
    
    // 如果信号量计数大于0，直接获取信号量
    if semaphore.get_count() > 0 {
        semaphore.decrement_count();
        arch_int_restore(int_save);
        return Ok(());
    } else if timeout == 0 {
        // 无信号量可用且不等待
        arch_int_restore(int_save);
        return Err(SemaphoreError::Unavailable.into());
    }
    
    // 需要等待信号量
    task_wait(&mut semaphore.sem_list, timeout);
    
    // 进行调度
    schedule();
    
    // 释放调度器锁，再次获取调度器锁
    arch_int_restore(int_save);
    let int_save = arch_int_lock();
    
    // 检查是否超时
    if current_task.task_status.contains(TaskStatus::TIMEOUT) {
        current_task.task_status.remove(TaskStatus::TIMEOUT);
        arch_int_restore(int_save);
        return Err(SemaphoreError::Timeout.into());
    }
    
    arch_int_restore(int_save);
    Ok(())
}

/// 释放信号量(V操作)
pub fn semaphore_post(semaphore_id: SemaphoreId) -> SemaphoreResult<()> {
    // 保存中断状态并禁用中断
    let int_save = arch_int_lock();
    
    // 获取信号量控制块
    let semaphore = match SemaphoreManager::get_semaphore(semaphore_id) {
        Ok(sem) => {
            if sem.is_unused() || !sem.matches_id(semaphore_id) {
                arch_int_restore(int_save);
                return Err(SemaphoreError::Invalid.into());
            }
            sem
        }
        Err(e) => {
            arch_int_restore(int_save);
            return Err(e);
        }
    };
    
    #[cfg(feature = "debug-semaphore")]
    {
        println_debug!("Semaphore post: {:?}, current count: {}", 
                  semaphore_id, semaphore.get_count());
    }
    
    // 检查信号量计数是否达到最大值
    let max_count = semaphore.max_count();
    if semaphore.get_count() >= max_count {
        arch_int_restore(int_save);
        return Err(SemaphoreError::Overflow.into());
    }
    
    // 如果有任务在等待信号量
    if semaphore.has_waiting_tasks() {
        let first_node = LinkedList::first(&raw const semaphore.sem_list);
        let waiting_task = task_from_wait_list(first_node);
        
        // 唤醒等待任务
        task_wait_abort(waiting_task);
        
        // 恢复中断并进行调度
        arch_int_restore(int_save);
        schedule();
        
        return Ok(());
    } else {
        // 没有任务等待，增加信号量计数
        semaphore.increment_count();
        arch_int_restore(int_save);
        
        return Ok(());
    }
}