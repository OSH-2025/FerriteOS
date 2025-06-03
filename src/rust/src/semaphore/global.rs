use crate::{
    config::SEM_LIMIT,
    result::SystemResult,
    semaphore::{
        error::SemaphoreError,
        types::{SemaphoreControlBlock, SemaphoreId, SemaphoreType},
    },
    utils::list::LinkedList,
};

/// 全部信号量控制块数组
#[unsafe(export_name = "g_allSem")]
pub static mut SEMAPHORE_POOL: [SemaphoreControlBlock; SEM_LIMIT as usize] =
    [SemaphoreControlBlock::UNINT; SEM_LIMIT as usize];

#[unsafe(export_name = "g_unusedSemList")]
pub static mut UNUSED_SEMAPHORE_LIST: LinkedList = LinkedList::new();

pub struct SemaphoreManager;

impl SemaphoreManager {
    /// 初始化互斥锁池
    #[inline]
    pub fn initialize() {
        LinkedList::init(&raw mut UNUSED_SEMAPHORE_LIST);
        for id in 0..SEM_LIMIT {
            let semaphore = Self::get_semaphore_by_index(id as usize);
            semaphore.set_id(id.into());
            LinkedList::tail_insert(&raw mut UNUSED_SEMAPHORE_LIST, &raw mut semaphore.sem_list);
        }
    }

    /// 检查是否有可用的互斥锁
    #[inline]
    pub fn has_available_semaphore() -> bool {
        !LinkedList::is_empty(&raw const UNUSED_SEMAPHORE_LIST)
    }

    // 通过索引获取互斥锁
    #[inline]
    fn get_semaphore_by_index(index: usize) -> &'static mut SemaphoreControlBlock {
        unsafe { &mut SEMAPHORE_POOL[index] }
    }

    /// 分配一个新的互斥锁
    #[inline]
    pub fn allocate(sem_type: SemaphoreType, count: u16) -> SystemResult<SemaphoreId> {
        if !Self::has_available_semaphore() {
            return Err(SemaphoreError::AllBusy.into());
        };
        let node = LinkedList::first(&raw const UNUSED_SEMAPHORE_LIST);
        LinkedList::remove(node);
        let semaphore = SemaphoreControlBlock::from_list(node);
        semaphore.initialize(sem_type, count);

        Ok(semaphore.get_id())
    }

    /// 释放互斥锁
    #[inline]
    pub fn deallocate(id: SemaphoreId) -> SystemResult<()> {
        let semaphore = Self::get_semaphore(id)?;
        if !semaphore.matches_id(id) || semaphore.is_unused() {
            return Err(SemaphoreError::Invalid.into());
        }
        if semaphore.has_waiting_tasks() {
            return Err(SemaphoreError::Pended.into());
        }
        // 重置互斥锁状态
        semaphore.reset();
        LinkedList::tail_insert(&raw mut UNUSED_SEMAPHORE_LIST, &raw mut semaphore.sem_list);
        // 加回未使用列表
        Ok(())
    }

    /// 获取互斥锁
    #[inline]
    pub fn get_semaphore(id: SemaphoreId) -> SystemResult<&'static mut SemaphoreControlBlock> {
        let index = id.get_index() as u32;
        if index >= SEM_LIMIT {
            return Err(SemaphoreError::Invalid.into());
        }
        Ok(Self::get_semaphore_by_index(index as usize))
    }

    // /// 获取互斥锁（可变引用）
    // #[inline]
    // pub fn get_mutex_mut(id: SemaphoreId) -> SystemResult<&'static mut SemaphoreControlBlock> {
    //     let index = id.get_index() as u32;
    //     if index >= SEM_LIMIT {
    //         return Err(SemaphoreError::Invalid.into());
    //     }
    //     Ok(Self::get_semaphore_by_index(index as usize))
    // }
}
