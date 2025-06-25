//! 互斥锁全局变量
use crate::config::MUX_LIMIT;
use crate::mutex::types::MutexControlBlock;
use crate::result::SystemResult;
use crate::utils::list::LinkedList;

use super::error::MutexError;
use super::types::MutexId;

#[unsafe(export_name = "g_allMux")]
pub static mut MUTEX_POOL: [MutexControlBlock; MUX_LIMIT as usize] =
    [MutexControlBlock::UNINIT; MUX_LIMIT as usize];

#[unsafe(export_name = "g_unusedMuxList")]
pub static mut UNUSED_MUTEX_LIST: LinkedList = LinkedList::new();

pub struct MutexManager;

impl MutexManager {
    /// 初始化互斥锁池
    #[inline]
    pub fn initialize() {
        LinkedList::init(&raw mut UNUSED_MUTEX_LIST);
        for id in 0..MUX_LIMIT {
            let mutex = Self::get_mutex_by_id(id);
            mutex.set_id(id.into());
            LinkedList::tail_insert(&raw mut UNUSED_MUTEX_LIST, &raw mut mutex.mux_list);
        }
    }

    /// 检查是否有可用的互斥锁
    #[inline]
    pub fn has_available_mutex() -> bool {
        !LinkedList::is_empty(&raw const UNUSED_MUTEX_LIST)
    }

    // 通过索引获取互斥锁
    #[inline]
    fn get_mutex_by_id(id: u32) -> &'static mut MutexControlBlock {
        unsafe { &mut MUTEX_POOL[id as usize] }
    }

    /// 分配一个新的互斥锁
    #[inline]
    pub fn allocate() -> MutexId {
        let node = LinkedList::first(&raw const UNUSED_MUTEX_LIST);
        LinkedList::remove(node);
        let mutex = MutexControlBlock::from_mux_list(node);
        mutex.initialize();
        mutex.get_id()
    }

    /// 释放互斥锁
    #[inline]
    pub fn deallocate(mutex: &mut MutexControlBlock, id: MutexId) -> SystemResult<()> {
        if !mutex.matches_id(id) || mutex.is_unused() {
            return Err(MutexError::Invalid.into());
        }
        if mutex.has_waiting_tasks() || mutex.is_locked() {
            return Err(MutexError::Pended.into());
        }
        // 重置互斥锁状态
        mutex.reset();
        LinkedList::tail_insert(&raw mut UNUSED_MUTEX_LIST, &raw mut mutex.mux_list);
        // 加回未使用列表
        Ok(())
    }

    /// 获取互斥锁（不可变引用）
    #[allow(dead_code)]
    #[inline]
    pub fn get_mutex(id: MutexId) -> SystemResult<&'static MutexControlBlock> {
        let index = id.get_index() as u32;
        if index >= MUX_LIMIT {
            return Err(MutexError::Invalid.into());
        }
        Ok(Self::get_mutex_by_id(index))
    }

    /// 获取互斥锁（可变引用）
    #[inline]
    pub fn get_mutex_mut(id: MutexId) -> SystemResult<&'static mut MutexControlBlock> {
        let index = id.get_index() as u32;
        if index >= MUX_LIMIT {
            return Err(MutexError::Invalid.into());
        }
        Ok(Self::get_mutex_by_id(index))
    }
}
