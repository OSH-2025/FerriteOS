use crate::{
    ffi::bindings::get_current_task,
    task::{
        sched::priority_queue_insert_at_back,
        timer::{add_to_timer_list, delete_from_timer_list},
        types::{TaskCB, TaskStatus},
    },
    utils::list::LinkedList,
};

/// 将当前任务放入等待列表
pub fn task_wait(list: &mut LinkedList, timeout: u32) {
    // 获取当前运行的任务
    let run_task = get_current_task();

    // 清除就绪状态
    run_task.task_status.remove(TaskStatus::READY);

    // 设置等待状态
    run_task.task_status.insert(TaskStatus::PEND);

    // 添加到等待队列尾部
    LinkedList::tail_insert(list, &mut run_task.pend_list);

    // 如果设置了超时时间，添加到定时器列表
    if timeout != u32::MAX {
        run_task.task_status.insert(TaskStatus::PEND_TIME);
        add_to_timer_list(run_task, timeout);
    }
}

/// 唤醒等待中的任务
pub fn task_wake(resumed_task: &mut TaskCB) {
    // 从等待列表中移除
    LinkedList::remove(&mut resumed_task.pend_list);
    resumed_task.task_status.remove(TaskStatus::PEND);

    // 如果任务在定时器列表中，将其移除
    if resumed_task.task_status.contains(TaskStatus::PEND_TIME) {
        delete_from_timer_list(resumed_task);
        resumed_task.task_status.remove(TaskStatus::PEND_TIME);
    }

    if !resumed_task.task_status.intersects(TaskStatus::BLOCKED) {
        resumed_task.task_status.insert(TaskStatus::READY);
        priority_queue_insert_at_back(&mut resumed_task.pend_list, resumed_task.priority as u32);
    }
}
