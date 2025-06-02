//! 互斥锁相关类型定义

use core::sync::atomic::{AtomicU16, Ordering};

use crate::container_of;
use crate::task::types::TaskCB;
use crate::utils::list::LinkedList;

/// 互斥锁状态
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u8)]
pub enum MutexState {
    Unused = 0,
    Used = 1,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(transparent)]
pub struct MutexId(pub u32);

/// 句柄分割位数

/// 为MutexHandle实现扩展trait
impl MutexId {
    /// 设置互斥锁ID
    const MUX_SPLIT_BIT: u32 = 16;

    pub fn new(id: u32) -> Self {
        Self(id)
    }

    /// 从计数和索引创建互斥锁ID
    pub fn set_mux_id(count: u16, index: u16) -> Self {
        let id = ((count as u32) << Self::MUX_SPLIT_BIT) | (index as u32);
        Self(id)
    }

    /// 获取互斥锁索引部分
    pub fn get_index(self) -> u16 {
        (self.0 & ((1 << Self::MUX_SPLIT_BIT) - 1)) as u16
    }

    /// 获取互斥锁计数部分
    pub fn get_count(self) -> u16 {
        (self.0 >> Self::MUX_SPLIT_BIT) as u16
    }

    /// 增加计数值生成新ID，保持索引不变
    pub fn increment_count(&self) {
        Self::set_mux_id(self.get_count().wrapping_add(1), self.get_index());
    }
}

// 实现转换trait，便于使用
impl From<u32> for MutexId {
    fn from(id: u32) -> Self {
        Self(id)
    }
}

impl From<MutexId> for u32 {
    fn from(id: MutexId) -> Self {
        id.0
    }
}

/// 互斥锁控制块
#[repr(C)]
#[derive(Debug)]
pub struct MutexControlBlock {
    /// 互斥锁链表 - 等待此互斥锁的任务列表
    /// 对应C结构体中的muxList字段
    pub mux_list: LinkedList,

    /// 当前持有互斥锁的任务
    /// 对应C结构体中的owner字段（4字节指针）
    /// 使用Option<&'static mut TaskCB>来表示可能为空的任务引用
    pub owner: *mut TaskCB,

    /// 互斥锁计数 - 递归锁定次数
    /// 对应C结构体中的muxCount字段
    pub mux_count: AtomicU16,

    /// 互斥锁状态
    /// 对应C结构体中的muxStat字段
    pub mux_stat: MutexState,

    /// 互斥锁ID
    /// 对应C结构体中的muxId字段
    pub mux_id: MutexId,
}

impl MutexControlBlock {
    pub const UNINIT: Self = Self {
        mux_list: LinkedList::new(),
        owner: core::ptr::null_mut(),
        mux_count: AtomicU16::new(0),
        mux_stat: MutexState::Unused,
        mux_id: MutexId(0),
    };

    /// 获取锁定计数
    pub fn get_count(&self) -> u16 {
        self.mux_count.load(Ordering::Acquire)
    }

    /// 设置锁定计数
    pub fn set_count(&self, count: u16) {
        self.mux_count.store(count, Ordering::Release);
    }

    /// 递增锁定计数
    pub fn increment_count(&self) -> u16 {
        self.mux_count.fetch_add(1, Ordering::AcqRel) + 1
    }

    /// 递减锁定计数
    pub fn decrement_count(&self) -> u16 {
        self.mux_count.fetch_sub(1, Ordering::AcqRel) - 1
    }

    /// 检查是否有等待的任务
    pub fn has_waiting_tasks(&self) -> bool {
        !LinkedList::is_empty(&raw const self.mux_list)
    }

    /// 检查是否被锁定
    pub fn is_locked(&self) -> bool {
        self.get_count() > 0
    }

    /// 检查指定任务是否为所有者
    pub fn is_owner(&self, task_cb: *mut TaskCB) -> bool {
        task_cb == self.owner
    }

    /// 设置所有者
    pub fn set_owner(&mut self, task: *mut TaskCB) {
        self.owner = task;
    }

    /// 获取所有者
    pub fn get_owner(&mut self) -> &mut TaskCB {
        unsafe { self.owner.as_mut().expect("Mutex owner is not set") }
    }

    /// 清除所有者
    pub fn clear_owner(&mut self) {
        self.owner = core::ptr::null_mut();
    }

    /// 获取互斥锁状态
    #[allow(dead_code)]
    pub fn is_used(&self) -> bool {
        self.mux_stat == MutexState::Used
    }

    pub fn is_unused(&self) -> bool {
        self.mux_stat == MutexState::Unused
    }

    /// 设置互斥锁状态
    pub fn set_state(&mut self, state: MutexState) {
        self.mux_stat = state;
    }

    /// 获取互斥锁ID
    pub fn get_id(&self) -> MutexId {
        self.mux_id
    }

    pub fn increment_id_counter(&mut self) {
        self.mux_id.increment_count();
    }

    /// 设置互斥锁ID
    pub fn set_id(&mut self, id: MutexId) {
        self.mux_id = id;
    }

    /// 检查是否为指定的句柄
    pub fn matches_id(&self, id: MutexId) -> bool {
        self.get_id() == id
    }

    /// 初始化互斥锁
    pub fn initialize(&mut self) {
        self.set_count(0);
        self.clear_owner();
        LinkedList::init(&raw mut self.mux_list);
        self.set_state(MutexState::Used);
    }

    /// 重置互斥锁
    pub fn reset(&mut self) {
        self.set_count(0);
        self.clear_owner();
        self.set_state(MutexState::Unused);
        self.increment_id_counter();
    }

    pub fn from_mux_list(ptr: *mut LinkedList) -> &'static mut MutexControlBlock {
        let mutex_ptr = container_of!(ptr, MutexControlBlock, mux_list);
        unsafe { &mut *mutex_ptr }
    }
}

impl Default for MutexControlBlock {
    fn default() -> Self {
        Self::UNINIT
    }
}
