use semihosting::eprintln;

use crate::{
    ffi::bindings::get_current_task,
    percpu::{can_preempt, os_percpu_get},
    task::{
        manager::{delete::task_delete, suspend::task_suspend},
        types::TaskSignal,
    },
};

/// 处理当前任务的挂起和删除信号
pub fn process_task_signals() -> u32 {
    // 获取当前运行的任务
    let run_task = get_current_task();

    // 处理KILL信号
    if run_task.signal.contains(TaskSignal::KILL) {
        // 清除所有信号
        run_task.signal = TaskSignal::empty();
        // 执行任务删除
        let task_id = run_task.task_id;
        if let Err(err) = task_delete(task_id) {
            eprintln!(
                "process_task_signals: task delete failed, err: {:x}\n",
                u32::from(err)
            );
        }
    }
    // 处理SUSPEND信号
    else if run_task.signal.contains(TaskSignal::SUSPEND) {
        // 只清除SUSPEND信号
        run_task.signal.remove(TaskSignal::SUSPEND);

        // 执行任务挂起，忽略可能的错误
        let _ = task_suspend(run_task.task_id);
    }

    // 检查是否需要调度
    check_reschedule_needed()
}

/// 检查是否需要重新调度
#[inline]
fn check_reschedule_needed() -> u32 {
    let percpu = os_percpu_get();
    // 如果可抢占且有挂起的调度请求
    if can_preempt() && percpu.needs_reschedule == 1 {
        // 清除调度标志
        percpu.needs_reschedule = 0;
        1
    } else {
        0
    }
}
