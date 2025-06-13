use core::ptr::addr_of;

use crate::{
    percpu::os_percpu_get,
    timer::{internal::timer_update_internal, types::TimerControlBlock},
    utils::{list::LinkedList, sortlink::SortLinkList},
};

/// 定时器扫描函数
pub fn timer_scan() {
    // 获取当前CPU的软件定时器排序链表
    let swtmr_sort_link = &mut os_percpu_get().swtmr_sort_link;

    // 更新游标并获取当前链表对象
    swtmr_sort_link.advance_cursor();
    let list_object = swtmr_sort_link.list_at_cursor();

    // 如果链表为空，返回
    if LinkedList::is_empty(list_object) {
        return;
    }

    unsafe {
        // 获取第一个节点并减少轮数
        let mut sort_list = SortLinkList::from_list((*list_object).next);
        sort_list.roll_num_dec();

        // 处理所有轮数为0的节点
        while sort_list.get_roll_num() == 0 {
            // 获取链表的第一个节点
            LinkedList::remove(&mut sort_list.sort_link_node);

            // 获取对应的定时器控制块
            let timer = TimerControlBlock::from_list(addr_of!(sort_list.sort_link_node));

            #[cfg(feature = "timer-in-isr")]
            {
                // 更新定时器
                os_swtmr_update(timer);

                // 如果处理函数非空
                if let Some(handler_fn) = timer.handler {
                    // 执行回调
                    handler_fn();
                }
            }

            // 根据编译选项选择不同的处理方式
            #[cfg(not(feature = "timer-in-isr"))]
            {
                use core::ffi::c_void;

                use crate::{
                    queue::operation::queue_write,
                    timer::types::{TIMER_HANDLE_ITEM_SIZE, TimerHandlerItem},
                };

                let timer_handler_item = TimerHandlerItem::new(timer.get_handler());
                // 写入队列
                let _ = queue_write(
                    os_percpu_get().swtmr_handler_queue.into(),
                    addr_of!(timer_handler_item) as *const c_void,
                    TIMER_HANDLE_ITEM_SIZE as u32,
                    0,
                );
            }

            // 更新定时器
            timer_update_internal(timer);

            // 检查链表是否为空
            if LinkedList::is_empty(list_object) {
                break;
            }

            // 获取下一个节点
            sort_list = SortLinkList::from_list((*list_object).next);
        }
    }
}
