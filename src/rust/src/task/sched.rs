use crate::utils::list::LinkedList;
use core::sync::atomic::{AtomicU32, Ordering};

const OS_PRIORITY_QUEUE_NUM: usize = 32;
const PRIQUEUE_PRIOR0_BIT: u32 = 0x8000_0000;

#[unsafe(no_mangle)]
pub static mut PRI_QUEUE_LIST: [LinkedList; OS_PRIORITY_QUEUE_NUM] =
    [LinkedList::UNINIT; OS_PRIORITY_QUEUE_NUM];

static PRI_QUEUE_BITMAP: AtomicU32 = AtomicU32::new(0);

/// 初始化优先级队列
#[unsafe(export_name = "OsPriQueueInit")]
pub extern "C" fn pri_queue_init() {
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
