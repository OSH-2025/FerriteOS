//! 信号量类型定义

use crate::{container_of, utils::list::LinkedList};
use core::sync::atomic::{AtomicU16, Ordering};

/// 信号量类型
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u8)]
pub enum SemaphoreType {
    /// 计数信号量 - 最大计数为LOS_SEM_COUNT_MAX
    Counting = 0,
    /// 二进制信号量 - 最大计数为OS_SEM_BINARY_COUNT_MAX
    Binary = 1,
}

impl SemaphoreType {
    /// 计数信号量的最大计数值
    const COUNT_MAX: u16 = 0xFFFE;

    /// 二进制信号量的最大计数值
    const BINARY_COUNT_MAX: u16 = 0x0001;

    /// 获取当前信号量类型的最大计数值
    pub fn max_count(&self) -> u16 {
        match self {
            Self::Counting => Self::COUNT_MAX,
            Self::Binary => Self::BINARY_COUNT_MAX,
        }
    }
}

/// 信号量状态
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u8)]
pub enum SemaphoreState {
    /// 未使用
    Unused = 0,
    /// 已使用
    Used = 1,
}

/// 信号量ID封装
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(transparent)]
pub struct SemaphoreId(pub u32);

impl SemaphoreId {
    /// 句柄分割位数
    const SEM_SPLIT_BIT: u32 = 16;

    /// 从计数和索引创建信号量ID
    pub fn new(count: u16, index: u16) -> Self {
        Self(((count as u32) << Self::SEM_SPLIT_BIT) | (index as u32))
    }

    /// 获取索引部分
    pub fn get_index(&self) -> u16 {
        (self.0 & ((1 << Self::SEM_SPLIT_BIT) - 1)) as u16
    }

    /// 获取计数部分
    pub fn get_count(&self) -> u16 {
        (self.0 >> Self::SEM_SPLIT_BIT) as u16
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
#[derive(Debug)]
pub struct SemaphoreControlBlock {
    /// 信号量状态，对应C代码中的semStat
    pub sem_stat: SemaphoreState,

    /// 信号量类型，对应C代码中的semType
    pub sem_type: SemaphoreType,

    /// 可用信号量数量，对应C代码中的semCount
    pub sem_count: AtomicU16,

    /// 信号量控制结构ID，对应C代码中的semId
    pub sem_id: SemaphoreId,

    /// 等待信号量的任务列表，对应C代码中的semList
    pub sem_list: LinkedList,
}

impl SemaphoreControlBlock {
    /// 创建一个新的信号量控制块
    pub const UNINT: Self = Self {
        sem_stat: SemaphoreState::Unused,
        sem_type: SemaphoreType::Counting,
        sem_count: AtomicU16::new(0),
        sem_id: SemaphoreId(0),
        sem_list: LinkedList::new(),
    };

    /// 创建一个新的信号量控制块
    #[allow(dead_code)]
    pub const fn new() -> Self {
        Self {
            sem_stat: SemaphoreState::Unused,
            sem_type: SemaphoreType::Counting,
            sem_count: AtomicU16::new(0),
            sem_id: SemaphoreId(0),
            sem_list: LinkedList::new(),
        }
    }

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

    /// 检查是否为指定的句柄
    #[inline]
    pub fn matches_id(&self, id: SemaphoreId) -> bool {
        self.get_id() == id
    }

    #[inline]
    pub fn increment_id_counter(&mut self) {
        self.sem_id = self.sem_id.increment_count();
    }

    /// 检查是否有等待任务
    #[inline]
    pub fn has_waiting_tasks(&self) -> bool {
        !LinkedList::is_empty(&raw const self.sem_list)
    }

    #[inline]
    pub fn from_list(list: *const LinkedList) -> &'static mut Self {
        let semaphore_ptr = container_of!(list, SemaphoreControlBlock, sem_list);
        unsafe { &mut *semaphore_ptr }
    }

    /// 初始化信号量
    #[inline]
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
        self.set_count(0);
        self.increment_id_counter();
    }
}
