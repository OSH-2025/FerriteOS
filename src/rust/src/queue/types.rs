//! 消息队列类型定义
use crate::utils::list::LinkedList;
use alloc::{boxed::Box, vec::Vec};
use semihosting::println;

/// 队列操作类型
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u8)]
pub enum QueueOperationType {
    /// 从队头读取
    ReadHead,
    /// 从队头写入
    WriteHead,
    /// 从队尾写入
    WriteTail,
}

impl QueueOperationType {
    /// 检查是否为读操作
    #[inline]
    pub fn is_read(&self) -> bool {
        *self == Self::ReadHead
    }

    /// 检查是否为写操作
    #[allow(dead_code)]
    #[inline]
    pub fn is_write(&self) -> bool {
        !self.is_read()
    }
}

/// 队列状态
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u8)]
pub enum QueueState {
    /// 未使用
    Unused = 0,
    /// 已使用
    Used = 1,
}

/// 队列ID封装
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(transparent)]
pub struct QueueId(pub u32);

impl QueueId {
    /// 队列ID分割位数
    const QUEUE_SPLIT_BIT: u32 = 16;

    /// 从计数和索引创建队列ID
    pub fn new(count: u16, index: u16) -> Self {
        Self(((count as u32) << Self::QUEUE_SPLIT_BIT) | (index as u32))
    }

    /// 获取索引部分
    pub fn get_index(&self) -> u16 {
        (self.0 & ((1 << Self::QUEUE_SPLIT_BIT) - 1)) as u16
    }

    /// 获取计数部分
    pub fn get_count(&self) -> u16 {
        (self.0 >> Self::QUEUE_SPLIT_BIT) as u16
    }

    /// 创建下一个版本的ID（计数+1）
    pub fn increment_count(&self) -> Self {
        Self::new(self.get_count().wrapping_add(1), self.get_index())
    }
}

impl From<u32> for QueueId {
    fn from(id: u32) -> Self {
        Self(id)
    }
}

impl From<QueueId> for u32 {
    fn from(id: QueueId) -> Self {
        id.0
    }
}

/// 队列控制块
#[derive(Debug)]
pub struct QueueControlBlock {
    /// 队列数据存储区域
    pub queue_mem: Option<Box<[u8]>>,

    /// 队列长度（最大消息数量）
    pub capacity: usize,

    /// 每个消息的大小（字节）
    pub slot_size: usize,

    /// 队列状态
    pub queue_state: QueueState,

    /// 队列ID
    pub queue_id: QueueId,

    /// 队列头指针
    pub queue_head: usize,

    /// 队列尾指针
    pub queue_tail: usize,

    /// 可读计数
    pub readable_count: usize,

    /// 可写计数
    pub writable_count: usize,

    /// 读等待链表
    pub read_waiting_list: LinkedList,

    /// 写等待链表
    pub write_waiting_list: LinkedList,
}

impl QueueControlBlock {
    pub const MESSAGE_LEN_BYTES: usize = 4; // 消息长度字段的字节数

    /// 创建一个新的未初始化队列控制块
    pub const UNINIT: Self = Self {
        queue_mem: None,
        queue_state: QueueState::Unused,
        capacity: 0,
        slot_size: 0,
        queue_id: QueueId(0),
        queue_head: 0,
        queue_tail: 0,
        readable_count: 0,
        writable_count: 0,
        read_waiting_list: LinkedList::new(),
        write_waiting_list: LinkedList::new(),
    };

    /// 创建一个新的未初始化队列控制块
    #[allow(dead_code)]
    pub const fn new() -> Self {
        Self {
            queue_mem: None,
            queue_state: QueueState::Unused,
            capacity: 0,
            slot_size: 0,
            queue_id: QueueId(0),
            queue_head: 0,
            queue_tail: 0,
            readable_count: 0,
            writable_count: 0,
            read_waiting_list: LinkedList::new(),
            write_waiting_list: LinkedList::new(),
        }
    }

    /// 设置队列状态
    #[inline]
    pub fn set_state(&mut self, state: QueueState) {
        self.queue_state = state;
    }

    /// 获取队列状态
    #[inline]
    pub fn get_state(&self) -> QueueState {
        self.queue_state
    }

    #[inline]
    pub fn is_unused(&self) -> bool {
        self.get_state() == QueueState::Unused
    }

    /// 队列ID
    #[inline]
    pub fn get_id(&self) -> QueueId {
        self.queue_id
    }

    /// 设置队列ID
    #[inline]
    pub fn set_id(&mut self, id: QueueId) {
        self.queue_id = id;
    }

    /// 检查是否为指定的句柄
    #[inline]
    pub fn matches_id(&self, id: QueueId) -> bool {
        self.get_id() == id
    }

    #[inline]
    pub fn increment_id_counter(&mut self) {
        self.queue_id = self.queue_id.increment_count();
    }

    #[inline]
    pub fn get_slot_size(&self) -> usize {
        self.slot_size
    }

    /// 检查是否有任务等待读取
    #[inline]
    pub fn has_read_waiting_tasks(&self) -> bool {
        !LinkedList::is_empty(&raw const self.read_waiting_list)
    }

    /// 检查是否有任务等待写入
    #[inline]
    pub fn has_write_waiting_tasks(&self) -> bool {
        !LinkedList::is_empty(&raw const self.write_waiting_list)
    }

    /// 检查是否有任务等待
    #[inline]
    pub fn has_waiting_tasks(&self) -> bool {
        self.has_read_waiting_tasks() || self.has_write_waiting_tasks()
    }

    /// 检查队列是否为空
    #[inline]
    pub fn is_empty(&self) -> bool {
        self.readable_count == 0
    }

    /// 检查队列是否已满
    #[inline]
    pub fn is_full(&self) -> bool {
        self.writable_count == 0
    }

    #[inline]
    pub fn is_read_write_inconsistent(&self) -> bool {
        (self.readable_count + self.writable_count) != self.capacity
    }

    /// 将队列头指针向前移动一个位置（考虑循环）
    #[inline]
    pub fn advance_head(&mut self) {
        if self.queue_head + 1 == self.capacity {
            self.queue_head = 0;
        } else {
            self.queue_head += 1;
        }
    }

    /// 将队列头指针向后移动一个位置（考虑循环）
    #[inline]
    pub fn retreat_head(&mut self) {
        if self.queue_head == 0 {
            self.queue_head = self.capacity - 1;
        } else {
            self.queue_head -= 1;
        }
    }

    /// 将队列尾指针向前移动一个位置（考虑循环）
    #[inline]
    pub fn advance_tail(&mut self) {
        if self.queue_tail + 1 == self.capacity {
            self.queue_tail = 0;
        } else {
            self.queue_tail += 1;
        }
    }

    /// 将队列尾指针向后移动一个位置（考虑循环）
    #[inline]
    #[allow(dead_code)]
    pub fn retreat_tail(&mut self) {
        if self.queue_tail == 0 {
            self.queue_tail = self.capacity - 1;
        } else {
            self.queue_tail -= 1;
        }
    }

    /// 检查指定操作类型是否有可用资源
    #[inline]
    pub fn has_available_resources(&self, op_type: QueueOperationType) -> bool {
        if op_type.is_read() {
            !self.is_empty()
        } else {
            !self.is_full()
        }
    }

    /// 递减指定操作类型的资源计数
    #[inline]
    pub fn decrement_resource_count(&mut self, op_type: QueueOperationType) {
        if op_type.is_read() {
            self.readable_count -= 1;
        } else {
            self.writable_count -= 1;
        }
    }

    /// 递增指定操作类型的资源计数
    #[inline]
    #[allow(dead_code)]
    pub fn increment_resource_count(&mut self, op_type: QueueOperationType) {
        if op_type.is_read() {
            self.readable_count += 1;
        } else {
            self.writable_count += 1;
        }
    }

    /// 递增相反操作类型的资源计数
    #[inline]
    pub fn increment_opposite_resource_count(&mut self, op_type: QueueOperationType) {
        if op_type.is_read() {
            self.writable_count += 1;
        } else {
            self.readable_count += 1;
        }
    }

    /// 获取指定操作类型的等待链表
    #[inline]
    pub fn get_wait_list(&mut self, op_type: QueueOperationType) -> &mut LinkedList {
        if op_type.is_read() {
            &mut self.read_waiting_list
        } else {
            &mut self.write_waiting_list
        }
    }

    /// 获取相反操作类型的等待链表
    #[inline]
    pub fn get_opposite_wait_list(&mut self, op_type: QueueOperationType) -> &mut LinkedList {
        if op_type.is_read() {
            &mut self.write_waiting_list
        } else {
            &mut self.read_waiting_list
        }
    }

    /// 检查相反操作类型的等待链表是否为空
    #[inline]
    pub fn is_opposite_wait_list_empty(&self, op_type: QueueOperationType) -> bool {
        if op_type.is_read() {
            LinkedList::is_empty(&raw const self.write_waiting_list)
        } else {
            LinkedList::is_empty(&raw const self.read_waiting_list)
        }
    }

    /// 初始化队列
    pub fn initialize(&mut self, capacity: usize, slot_size: usize) {
        // 为队列分配内存
        let total_size = capacity * slot_size;
        let mut queue_data: Vec<u8> = Vec::with_capacity(total_size);
        queue_data.resize(total_size, 0);
        let queue_mem = queue_data.into_boxed_slice();

        self.queue_mem = Some(queue_mem);
        self.set_state(QueueState::Used);
        self.capacity = capacity;
        self.slot_size = slot_size;
        self.queue_head = 0;
        self.queue_tail = 0;
        self.readable_count = 0;
        self.writable_count = capacity;
        LinkedList::init(&raw mut self.read_waiting_list);
        LinkedList::init(&raw mut self.write_waiting_list);
    }

    /// 重置信号量
    #[inline]
    pub fn reset(&mut self) {
        self.set_state(QueueState::Unused);
        self.queue_mem = None;
        self.increment_id_counter();
    }

    #[inline]
    pub fn enqueue_back(&mut self, message_data: &[u8]) {
        let queue_mem_slice = self.queue_mem.as_mut().unwrap();
        // 计算当前槽位在底层 Box<[u8]> 中的起始和结束索引
        let slot_start_idx = self.queue_tail * self.slot_size;
        let slot_end_idx = slot_start_idx + self.slot_size;

        // 获取当前槽位的可变切片，准备写入数据
        let current_slot = &mut queue_mem_slice[slot_start_idx..slot_end_idx];

        // 1. 将消息数据拷贝到槽位的前部
        current_slot[0..message_data.len()].copy_from_slice(message_data);

        // 2. 将消息长度（u16）编码并存储到槽位的末尾
        let message_len = message_data.len();
        let len_bytes = message_len.to_le_bytes(); // 或 to_be_bytes()，根据你的字节序需求选择

        let len_start_idx = self.slot_size - Self::MESSAGE_LEN_BYTES;
        current_slot[len_start_idx..].copy_from_slice(&len_bytes);

        // 更新队列指针和计数
        self.advance_tail();
    }

    #[inline]
    pub fn enqueue_front(&mut self, message_data: &[u8]) {
        // 更新队列指针和计数
        self.retreat_head();
        let queue_mem_slice = self.queue_mem.as_mut().unwrap();
        // 计算当前槽位在底层 Box<[u8]> 中的起始和结束索引
        let slot_start_idx = self.queue_head * self.slot_size;
        let slot_end_idx = slot_start_idx + self.slot_size;

        // 获取当前槽位的可变切片，准备写入数据
        let current_slot = &mut queue_mem_slice[slot_start_idx..slot_end_idx];

        // 1. 将消息数据拷贝到槽位的前部
        current_slot[0..message_data.len()].copy_from_slice(message_data);

        // 2. 将消息长度（u16）编码并存储到槽位的末尾
        let message_len = message_data.len();
        let len_bytes = message_len.to_le_bytes(); // 或 to_be_bytes()，根据你的字节序需求选择

        let len_start_idx = self.slot_size - Self::MESSAGE_LEN_BYTES;
        current_slot[len_start_idx..].copy_from_slice(&len_bytes);
    }

    #[inline]
    pub fn dequeue_front(&mut self, buffer: &mut [u8], buffer_size: &mut usize) {
        // 获取当前槽位的切片
        let queue_mem_slice = self.queue_mem.as_ref().unwrap();
        let slot_start_idx = self.queue_head * self.slot_size;
        let slot_end_idx = slot_start_idx + self.slot_size;

        let current_slot = &queue_mem_slice[slot_start_idx..slot_end_idx];

        let len_start_idx = self.slot_size - Self::MESSAGE_LEN_BYTES;
        let len_bytes_slice = &current_slot[len_start_idx..];
        let message_len = usize::from_le_bytes(len_bytes_slice.try_into().unwrap());
        // 将数据从队列槽位复制到调用者提供的缓冲区
        buffer[0..message_len].copy_from_slice(&current_slot[0..message_len]);
        *buffer_size = message_len;
        self.advance_head();
    }
    /// 获取队列信息
    #[inline]
    pub fn get_info(&self) -> QueueInfo {
        QueueInfo {
            queue_id: self.queue_id.0,
            queue_len: self.capacity as u16,
            queue_size: self.slot_size as u16,
            queue_head: self.queue_head as u16,
            queue_tail: self.queue_tail as u16,
            writable_count: self.writable_count as u16,
            readable_count: self.readable_count as u16,
        }
    }

    /// 打印队列信息
    #[inline]
    pub fn print_info(&self) {
        let info = self.get_info();
        println!(
            "Queue Info: ID: {}, Length: {}, Size: {}, Head: {}, Tail: {}, Writable: {}, Readable: {}",
            info.queue_id,
            info.queue_len,
            info.queue_size,
            info.queue_head,
            info.queue_tail,
            info.writable_count,
            info.readable_count
        );
    }
}

/// 队列信息结构体
#[repr(C)]
#[derive(Default, Debug, Clone, Copy)]
pub struct QueueInfo {
    /// 队列ID
    pub queue_id: u32,
    /// 队列长度，即队列中的节点数
    pub queue_len: u16,
    /// 队列节点大小
    pub queue_size: u16,
    /// 队列头节点位置（数组下标）
    pub queue_head: u16,
    /// 队列尾节点位置（数组下标）
    pub queue_tail: u16,
    /// 可写资源计数
    pub writable_count: u16,
    /// 可读资源计数
    pub readable_count: u16,
}
