//! 栈回溯相关功能

use semihosting::println;

use crate::{
    config::TASK_LIMIT,
    ffi::bindings::{arch_back_trace, arch_back_trace_with_sp, get_current_task},
    task::{global::get_tcb_from_id, types::TaskStatus},
};

/// 获取当前任务的栈回溯
pub fn back_trace() {
    let current_task = get_current_task();
    println!("{}", current_task);
    arch_back_trace();
}

/// 获取指定任务的栈回溯
pub fn task_back_trace(task_id: u32) {
    if task_id >= TASK_LIMIT {
        println!("Task ID is out of range!");
        return;
    }
    let task_cb = get_tcb_from_id(task_id);
    if task_cb.task_status.contains(TaskStatus::UNUSED) {
        println!("The task is not created!");
        return;
    }
    println!("{}", task_cb);
    arch_back_trace_with_sp(task_cb.stack_pointer);
}
