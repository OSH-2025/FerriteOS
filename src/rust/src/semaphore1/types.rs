use crate::utils::list::LinkedList;
use core::sync::atomic::{AtomicU16, Ordering};

/// 信号量类型
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u8)]
pub enum SemaphoreType {
    /// 计数信号量
    Counting = 0,
    /// 二进制信号量
    Binary = 1,
}

impl SemaphoreType {
    /// 获取信号量类型的最大计数值
    pub fn max_count(&self) -> u16 {
        match self {
            Self::Counting => crate::semaphore1::configs::COUNTING_SEMAPHORE_COUNT_MAX,
            Self::Binary => crate::semaphore1::configs::BINARY_SEMAPHORE_COUNT_MAX,
        }
    }
}

/// 信号量状态
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u8)]
pub enum SemaphoreState {
    /// 未使用
    Unused = 0,
    /// 使用中
    Used = 1,
}

/// 信号量ID封装
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(transparent)]
pub struct SemaphoreId(pub u32);

impl SemaphoreId {
    /// 信号量ID分割位数
    const SEMAPHORE_SPLIT_BIT: u32 = 16;

    /// 从计数和索引创建信号量ID
    pub fn new(count: u16, index: u16) -> Self {
        Self(((count as u32) << Self::SEMAPHORE_SPLIT_BIT) | (index as u32))
    }

    /// 获取索引部分
    pub fn get_index(&self) -> u16 {
        (self.0 & ((1 << Self::SEMAPHORE_SPLIT_BIT) - 1)) as u16
    }

    /// 获取计数部分
    pub fn get_count(&self) -> u16 {
        (self.0 >> Self::SEMAPHORE_SPLIT_BIT) as u16
    }

    /// 创建下一个版本的ID（计数+1）
    pub fn increment_count(&self) -> Self {
        Self::new(self.get_count().wrapping_add(1), self.get_index())
    }
}

impl From<u32> for SemaphoreId {
    fn from(id: u32) -> Self {
        Self(id)
    }
}

impl From<SemaphoreId> for u32 {
    fn from(id: SemaphoreId) -> Self {
        id.0
    }
}

/// 信号量控制块 - 保持与LosSemCB结构体内存布局兼容
#[repr(C)]
pub struct SemaphoreControlBlock {
    /// 信号量状态
    pub sem_stat: SemaphoreState,
    
    /// 信号量类型
    pub sem_type: SemaphoreType,
    
    /// 信号量计数
    pub sem_count: AtomicU16,
    
    /// 信号量ID
    pub sem_id: SemaphoreId,
    
    /// 等待任务链表
    pub sem_list: LinkedList,
}

impl SemaphoreControlBlock {
    /// 未初始化的控制块
    pub const UNINT: Self = Self {
        sem_stat: SemaphoreState::Unused,
        sem_type: SemaphoreType::Counting,
        sem_count: AtomicU16::new(0),
        sem_id: SemaphoreId(0),
        sem_list: LinkedList::new(),
    };
    
    /// 设置信号量状态
    #[inline]
    pub fn set_state(&mut self, state: SemaphoreState) {
        self.sem_stat = state;
    }
    
    /// 获取信号量状态
    #[inline]
    pub fn get_state(&self) -> SemaphoreState {
        self.sem_stat
    }
    
    /// 检查信号量是否未使用
    #[inline]
    pub fn is_unused(&self) -> bool {
        self.get_state() == SemaphoreState::Unused
    }
    
    /// 设置信号量类型
    #[inline]
    pub fn set_type(&mut self, sem_type: SemaphoreType) {
        self.sem_type = sem_type;
    }
    
    /// 获取信号量类型
    #[inline]
    pub fn get_type(&self) -> SemaphoreType {
        self.sem_type
    }
    
    /// 设置信号量计数
    #[inline]
    pub fn set_count(&self, count: u16) {
        self.sem_count.store(count, Ordering::Release);
    }
    
    /// 获取信号量计数
    #[inline]
    pub fn get_count(&self) -> u16 {
        self.sem_count.load(Ordering::Acquire)
    }
    
    /// 递增信号量计数
    #[inline]
    pub fn increment_count(&self) -> u16 {
        self.sem_count.fetch_add(1, Ordering::AcqRel) + 1
    }
    
    /// 递减信号量计数
    #[inline]
    pub fn decrement_count(&self) -> u16 {
        self.sem_count.fetch_sub(1, Ordering::AcqRel) - 1
    }
    
    /// 获取信号量最大计数值
    #[inline]
    pub fn max_count(&self) -> u16 {
        self.get_type().max_count()
    }
    
    /// 设置信号量ID
    #[inline]
    pub fn set_id(&mut self, id: SemaphoreId) {
        self.sem_id = id;
    }
    
    /// 获取信号量ID
    #[inline]
    pub fn get_id(&self) -> SemaphoreId {
        self.sem_id
    }
    
    /// 检查是否匹配指定ID
    #[inline]
    pub fn matches_id(&self, id: SemaphoreId) -> bool {
        self.get_id() == id
    }
    
    /// 增加ID计数器
    #[inline]
    pub fn increment_id_counter(&mut self) {
        self.sem_id = self.sem_id.increment_count();
    }
    
    /// 检查是否有任务等待
    #[inline]
    pub fn has_waiting_tasks(&self) -> bool {
        !LinkedList::is_empty(&raw const self.sem_list)
    }
    
    /// 从链表节点获取信号量控制块
    #[inline]
    pub fn from_list(list: *const LinkedList) -> &'static mut Self {
        let ptr = crate::container_of!(list, Self, sem_list);
        unsafe { &mut *ptr }
    }
    
    /// 初始化信号量
    pub fn initialize(&mut self, sem_type: SemaphoreType, count: u16) {
        self.set_state(SemaphoreState::Used);
        self.set_type(sem_type);
        self.set_count(count);
        LinkedList::init(&raw mut self.sem_list);
    }
    
    /// 重置信号量
    #[inline]
    pub fn reset(&mut self) {
        self.set_state(SemaphoreState::Unused);
        self.increment_id_counter();
    }
}