// 双向链表结构体定义，对应C中的LOS_DL_LIST
#[repr(C)]
pub struct LosDlList {
    pub pst_prev: *mut LosDlList,  // Current node's pointer to the previous node
    pub pst_next: *mut LosDlList,  // Current node's pointer to the next node
}

// 队列控制块结构体，对应C中的LosQueueCB
#[repr(C)]
pub struct LosQueueCB {
    pub queue_handle: *mut u8,         // Pointer to a queue handle
    pub queue_state: u8,               // state
    pub queue_mem_type: u8,            // memory type
    pub queue_len: u16,                // length
    pub queue_size: u16,               // Node size
    pub queue_id: u32,                 // queueId
    pub queue_head: u16,               // Node head
    pub queue_tail: u16,               // Node tail
    pub readable_writable_cnt: [u16; 2], // Count of readable or writable resources, 0:readable, 1:writable
    pub read_write_list: [LosDlList; 2], // the linked list to be read or written, 0:readlist, 1:writelist
    pub mem_list: LosDlList,           // Pointer to the memory linked list
}

// 队列读写操作的枚举类型
#[repr(u8)]
#[allow(non_camel_case_types)]
pub enum QueueReadWrite {
    OS_QUEUE_READ = 0,
    OS_QUEUE_WRITE = 1,
    OS_QUEUE_N_RW = 2,
}

// 队列头尾操作的枚举类型
#[repr(u8)]
#[allow(non_camel_case_types)]
pub enum QueueHeadTail {
    OS_QUEUE_HEAD = 0,
    OS_QUEUE_TAIL = 1,
}

#[repr(C)]
pub struct QueueInfoS {
    pub uw_queue_id: u32,      // 队列ID
    pub us_queue_len: u16,     // 队列长度
    pub us_queue_size: u16,    // 队列大小
    pub us_queue_head: u16,    // 队列头指针
    pub us_queue_tail: u16,    // 队列尾指针
    pub us_readable_cnt: u16,  // 可读消息数
    pub us_writable_cnt: u16,  // 可写消息数
    pub uw_wait_read_task: u64, // 等待读取的任务位图
    pub uw_wait_write_task: u64, // 等待写入的任务位图
    pub uw_wait_mem_task: u64,  // 等待内存的任务位图
}
