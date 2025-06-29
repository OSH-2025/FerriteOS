use crate::{
    config::TASK_LIMIT,
    percpu::os_percpu_get,
    task::{
        global::{FREE_TASK_LIST, TASK_RECYCLE_LIST, get_tcb_from_id},
        sched::init_priority_queue,
    },
    utils::{list::LinkedList, sortlink::os_sort_link_init},
};

/// 初始化任务模块
pub fn init_task_system() {
    // 初始化空闲任务列表和回收列表
    LinkedList::init(&raw mut FREE_TASK_LIST);
    LinkedList::init(&raw mut TASK_RECYCLE_LIST);

    // 初始化每个任务控制块并添加到空闲列表
    for index in 0..TASK_LIMIT {
        let task_cb = get_tcb_from_id(index);
        task_cb.task_id = index;
        LinkedList::tail_insert(&raw mut FREE_TASK_LIST, &mut task_cb.pend_list);
    }

    // 初始化优先级队列
    init_priority_queue();

    // 为每个CPU核心初始化排序链接
    let percpu_array = os_percpu_get();
    os_sort_link_init(&mut percpu_array.task_sort_link);
}