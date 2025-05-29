use crate::{
    config::{OK, LOS_ERRNO_TSK_DELAY_IN_INT, LOS_ERRNO_TSK_DELAY_IN_LOCK, 
             LOS_ERRNO_TSK_OPERATE_SYSTEM_TASK},
    debug::back_trace,
    hwi::is_interrupt_active,
    interrupt::{int_lock, int_restore},
    percpu::can_preempt,
    scheduler::os_sched_resched,
    task::{
        types::{TaskCB, TaskStatus, TaskFlags, TaskError},
        manager::get_current_task,
        timer::add_to_timer_list,
        scheduling::yield_task::task_yield,
    },
};

/// 使当前任务延时指定的tick数
/// 
/// # Arguments
/// * `tick` - 延时的tick数
/// 
/// # Returns
/// * `Ok(())` - 成功
/// * `Err(TaskError)` - 失败，包含具体错误原因
pub fn task_delay(tick: u32) -> Result<(), TaskError> {
    // 检查是否在中断上下文
    if is_interrupt_active() {
        return Err(TaskError::DelayInInterrupt);
    }
    
    // 获取当前运行的任务
    let run_task = get_current_task();
    
    // 检查是否是系统任务
    unsafe {
        if (*run_task).task_flags.contains(TaskFlags::SYSTEM) {
            // 记录回溯信息
            back_trace();
            return Err(TaskError::OperateSystemTask);
        }
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
    
    unsafe {
        // 将任务添加到定时器列表
        add_to_timer_list(run_task, tick);
        
        // 设置任务状态为延时
        (*run_task).task_status.insert(TaskStatus::DELAY);
        
        // 触发调度
        os_sched_resched();
    }
    
    // 解锁调度器
    int_restore(int_save);
    
    Ok(())
}

/// C兼容的任务延时函数
#[no_mangle]
pub extern "C" fn LOS_TaskDelay(tick: u32) -> u32 {
    match task_delay(tick) {
        Ok(()) => OK,
        Err(err) => err.into()
    }
}