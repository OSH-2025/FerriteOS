use crate::{
    container_of, offset_of,
    percpu::os_percpu_get,
    task::{
        sched::{priority_queue_insert_at_back, schedule},
        types::{TaskCB, TaskStatus},
    },
    utils::{
        list::LinkedList,
        sortlink::{SortLinkList, add_to_sort_link, delete_from_sort_link},
    },
};

#[unsafe(export_name = "OsTaskAdd2TimerList")]
pub extern "C" fn add_to_timer_list(task_cb: &mut TaskCB, timeout: u32) {
    // 设置排序链表值
    task_cb.sort_list.set_timeout(timeout);
    let sort_link_header = &mut os_percpu_get().task_sort_link;
    add_to_sort_link(sort_link_header, &mut task_cb.sort_list);
}

#[unsafe(export_name = "OsTimerListDelete")]
pub extern "C" fn delete_from_timer_list(task_cb: &mut TaskCB) {
    let sort_link_header = &mut os_percpu_get().task_sort_link;
    delete_from_sort_link(sort_link_header, &mut (*task_cb).sort_list);
}

#[unsafe(export_name = "OsTaskScan")]
pub extern "C" fn task_scan() {
    let mut need_schedule = false;
    // 获取当前CPU的任务排序链表
    let sort_link_header = &mut os_percpu_get().task_sort_link;
    sort_link_header.advance_cursor();
    let list_object = sort_link_header.list_at_cursor();
    if LinkedList::is_empty(list_object) {
        return;
    }
    unsafe {
        // 获取链表中的第一个元素
        let mut sort_list = container_of!((*list_object).next, SortLinkList, sort_link_node);
        (*sort_list).roll_num_dec();
        // 处理所有超时的任务
        while (*sort_list).get_roll_num() == 0 {
            // 从链表中删除节点
            LinkedList::remove(&mut (*sort_list).sort_link_node);

            // 获取任务控制块
            let task_cb = container_of!(sort_list, TaskCB, sort_list);
            // 清除任务的定时状态
            (*task_cb).task_status.remove(TaskStatus::PEND_TIME);

            // 保存任务当前状态
            let temp_status = (*task_cb).task_status;

            // 处理阻塞任务
            if temp_status.contains(TaskStatus::PEND) {
                (*task_cb).task_status.remove(TaskStatus::PEND);
                (*task_cb).task_status.insert(TaskStatus::TIMEOUT);
                LinkedList::remove(&mut (*task_cb).pend_list);
                (*task_cb).task_sem = core::ptr::null_mut();
                (*task_cb).task_mux = core::ptr::null_mut();
            } else {
                (*task_cb).task_status.remove(TaskStatus::DELAY);
            }
            if !temp_status.contains(TaskStatus::SUSPEND) {
                (*task_cb).task_status.insert(TaskStatus::READY);
                priority_queue_insert_at_back(
                    &mut (*task_cb).pend_list,
                    (*task_cb).priority as u32,
                );
                need_schedule = true;
            }
            // 如果列表为空，退出循环
            if LinkedList::is_empty(list_object) {
                break;
            }

            // 获取下一个元素
            sort_list = container_of!((*list_object).next, SortLinkList, sort_link_node);
        }
        // 如果有任务超时并就绪，触发调度
        if need_schedule {
            schedule();
        }
    }
}
