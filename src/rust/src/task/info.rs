use crate::ffi::bindings::get_current_task;

/// 获取当前运行任务的ID
pub fn get_current_task_id() -> u32 {
    // 获取当前运行的任务
    let run_task = get_current_task();
    run_task.task_id
}
