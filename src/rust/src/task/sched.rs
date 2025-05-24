use crate::utils::list::LinkedList;
use core::sync::atomic::{AtomicU32, Ordering};

use super::types::TaskCB;

const OS_PRIORITY_QUEUE_NUM: usize = 32;
const PRIQUEUE_PRIOR0_BIT: u32 = 0x8000_0000;

#[unsafe(no_mangle)]
pub static mut PRI_QUEUE_LIST: [LinkedList; OS_PRIORITY_QUEUE_NUM] =
    [LinkedList::UNINIT; OS_PRIORITY_QUEUE_NUM];

static PRI_QUEUE_BITMAP: AtomicU32 = AtomicU32::new(0);

/// 初始化优先级队列
#[unsafe(export_name = "OsPriQueueInit")]
pub extern "C" fn priority_queue_init() {
    for priority in 0..OS_PRIORITY_QUEUE_NUM {
        LinkedList::init(&mut unsafe { PRI_QUEUE_LIST }[priority]);
    }
}

/// 将任务节点插入优先级队列头部
#[unsafe(export_name = "OsPriQueueEnqueueHead")]
pub extern "C" fn priority_queue_insert_at_front(priqueue_item: &mut LinkedList, priority: u32) {
    assert!(priqueue_item.next.is_null(), "节点next指针必须为null");

    // 如果该优先级队列为空，则在位图中设置对应位
    if LinkedList::is_empty(&mut unsafe { PRI_QUEUE_LIST }[priority as usize]) {
        PRI_QUEUE_BITMAP.fetch_or(PRIQUEUE_PRIOR0_BIT >> priority, Ordering::Release);
    }

    // 将节点插入到优先级队列的头部
    LinkedList::head_insert(
        &mut unsafe { PRI_QUEUE_LIST }[priority as usize],
        priqueue_item,
    );
}

/// 将任务节点插入优先级队列尾部
#[unsafe(export_name = "OsPriQueueEnqueue")]
pub extern "C" fn priority_queue_insert_at_back(priqueue_item: &mut LinkedList, priority: u32) {
    assert!(priqueue_item.next.is_null(), "节点next指针必须为null");

    // 如果该优先级队列为空，则在位图中设置对应位
    if LinkedList::is_empty(&mut unsafe { PRI_QUEUE_LIST }[priority as usize]) {
        PRI_QUEUE_BITMAP.fetch_or(PRIQUEUE_PRIOR0_BIT >> priority, Ordering::Release);
    }

    // 将节点插入到优先级队列的尾部
    LinkedList::tail_insert(
        &mut unsafe { PRI_QUEUE_LIST }[priority as usize],
        priqueue_item,
    );
}

/// 从优先级队列中移除任务节点
#[unsafe(export_name = "OsPriQueueDequeue")]
pub extern "C" fn priority_queue_remove(priqueue_item: &mut LinkedList) {
    // 从链表中删除节点
    LinkedList::remove(priqueue_item);

    // 获取包含此节点的任务控制块
    let run_task = TaskCB::from_pend_list(priqueue_item);

    // 如果该优先级队列为空，原子更新位图
    if LinkedList::is_empty(&mut unsafe { PRI_QUEUE_LIST }[run_task.priority as usize]) {
        PRI_QUEUE_BITMAP.fetch_and(
            !(PRIQUEUE_PRIOR0_BIT >> run_task.priority),
            Ordering::Release,
        );
    }
}
