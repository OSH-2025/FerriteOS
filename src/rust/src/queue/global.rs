use crate::{
    config::QUEUE_LIMIT,
    queue::types::{QueueControlBlock, QueueId, QueueMemoryType},
    utils::list::LinkedList,
};

/// 全部信号量控制块数组
#[unsafe(export_name = "g_allQueue")]
pub static mut QUEUE_POOL: [QueueControlBlock; QUEUE_LIMIT as usize] =
    [QueueControlBlock::UNINT; QUEUE_LIMIT as usize];

#[unsafe(export_name = "g_freeQueueList")]
pub static mut UNUSED_QUEUE_LIST: LinkedList = LinkedList::new();

pub struct QueueManager;

impl QueueManager {
    /// 初始化消息队列池
    #[inline]
    pub fn initialize() {
        LinkedList::init(&raw mut UNUSED_QUEUE_LIST);
        for id in 0..QUEUE_LIMIT {
            let queue = Self::get_queue_by_index(id as usize);
            queue.set_id(id.into());
            LinkedList::tail_insert(
                &raw mut UNUSED_QUEUE_LIST,
                &raw mut queue.write_waiting_list,
            );
        }
    }

    /// 检查是否有可用的消息队列
    #[inline]
    pub fn has_available_queue() -> bool {
        !LinkedList::is_empty(&raw const UNUSED_QUEUE_LIST)
    }

    // 通过索引获取消息队列
    #[inline]
    pub fn get_queue_by_index(index: usize) -> &'static mut QueueControlBlock {
        unsafe { &mut QUEUE_POOL[index] }
    }

    /// 分配一个新的消息队列
    #[inline]
    pub fn allocate(
        queue_mem: *mut u8,
        mem_type: QueueMemoryType,
        queue_len: u16,
        queue_size: u16,
    ) -> QueueId {
        let node = LinkedList::first(&raw const UNUSED_QUEUE_LIST);
        LinkedList::remove(node);
        let queue = QueueControlBlock::from_list(node);
        queue.initialize(queue_mem, mem_type, queue_len, queue_size);
        queue.get_id()
    }

    /// 释放互斥锁
    #[inline]
    pub fn deallocate(index: u16) {
        let queue = Self::get_queue_by_index(index as usize);
        queue.reset();
        LinkedList::tail_insert(
            &raw mut UNUSED_QUEUE_LIST,
            &raw mut queue.write_waiting_list,
        );
    }
}
