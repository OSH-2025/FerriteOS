use crate::{
    interrupt::{disable_interrupts, restore_interrupt_state},
    percpu::os_percpu_get,
    task::{global::is_scheduler_active, sched::schedule},
};

/// 锁定任务调度
pub fn task_lock() {
    // 保存中断状态并关中断
    let int_save = disable_interrupts();

    // 获取当前CPU的任务锁计数
    let percpu = os_percpu_get();
    percpu.task_lock_cnt += 1;
    // 恢复中断状态
    restore_interrupt_state(int_save);
}

/// 解锁任务调度
pub fn task_unlock() {
    // 保存中断状态并关中断
    let int_save = disable_interrupts();

    // 获取当前CPU的数据
    let percpu = os_percpu_get();

    // 任务锁计数大于0时才减少
    if percpu.task_lock_cnt > 0 {
        percpu.task_lock_cnt -= 1;

        // 如果任务锁计数为0，且有挂起的调度请求，且调度器处于活动状态
        if percpu.task_lock_cnt == 0
            && percpu.needs_reschedule == 1
            && is_scheduler_active()
        {
            // 清除挂起标志
            percpu.needs_reschedule = 0;

            // 恢复中断状态
            restore_interrupt_state(int_save);

            // 触发调度
            schedule();
            return;
        }
    }
    // 恢复中断状态
    restore_interrupt_state(int_save);
}
