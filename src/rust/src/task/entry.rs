use crate::{
    config::TASK_LIMIT,
    hwi::{int_lock, int_restore, int_unlock},
    percpu::os_percpu_get,
    task::{global::get_tcb_from_id, manager::delete::task_delete},
};

/// 任务入口函数
#[unsafe(export_name = "OsTaskEntry")]
pub extern "C" fn task_entry(task_id: u32) {
    debug_assert!(task_id < TASK_LIMIT);

    int_unlock();

    let task_cb = get_tcb_from_id(task_id);

    task_cb.task_entry.unwrap()(task_cb.args);

    // 禁用中断
    let int_save = int_lock();

    // 清除任务锁定计数
    os_percpu_get().task_lock_cnt = 0;

    // 恢复中断
    int_restore(int_save);

    // 删除任务
    let _ = task_delete(task_cb.task_id);
}
