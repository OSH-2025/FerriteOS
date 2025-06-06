use crate::{
    semaphore1::{
        configs::SEM_LIMIT,
        error::{SemaphoreError, SemaphoreResult},
        types::{SemaphoreControlBlock, SemaphoreId, SemaphoreType},
    },
    utils::list::LinkedList,
};

/// 全部信号量控制块数组
#[unsafe(export_name = "g_allSem")]
pub static mut SEMAPHORE_POOL: [SemaphoreControlBlock; SEM_LIMIT as usize] = 
    [SemaphoreControlBlock::UNINT; SEM_LIMIT as usize];

/// 未使用信号量列表
#[unsafe(export_name = "g_unusedSemList")]
pub static mut UNUSED_SEMAPHORE_LIST: LinkedList = LinkedList::new();

/// 信号量管理器
pub struct SemaphoreManager;

impl SemaphoreManager {
    /// 初始化信号量池
    #[inline]
    pub fn initialize() {
        LinkedList::init(&raw mut UNUSED_SEMAPHORE_LIST);
        for id in 0..SEM_LIMIT {
            let semaphore = Self::get_semaphore_by_index(id as usize);
            semaphore.set_id(SemaphoreId::new(0, id as u16));
            LinkedList::tail_insert(
                &raw mut UNUSED_SEMAPHORE_LIST, 
                &raw mut semaphore.sem_list
            );
        }
    }
    
    /// 检查是否有可用的信号量
    #[inline]
    pub fn has_available_semaphore() -> bool {
        !LinkedList::is_empty(&raw const UNUSED_SEMAPHORE_LIST)
    }
    
    /// 通过索引获取信号量
    #[inline]
    fn get_semaphore_by_index(index: usize) -> &'static mut SemaphoreControlBlock {
        unsafe { &mut SEMAPHORE_POOL[index] }
    }
    
    /// 分配一个新的信号量
    #[inline]
    pub fn allocate(sem_type: SemaphoreType, count: u16) -> SemaphoreResult<SemaphoreId> {
        if !Self::has_available_semaphore() {
            return Err(SemaphoreError::AllBusy.into());
        }
        
        let node = LinkedList::first(&raw const UNUSED_SEMAPHORE_LIST);
        LinkedList::remove(node);
        let semaphore = SemaphoreControlBlock::from_list(node);
        semaphore.initialize(sem_type, count);
        
        Ok(semaphore.get_id())
    }
    
    /// 释放信号量
    #[inline]
    pub fn deallocate(id: SemaphoreId) -> SemaphoreResult<()> {
        let semaphore = Self::get_semaphore(id)?;
        
        if !semaphore.matches_id(id) || semaphore.is_unused() {
            return Err(SemaphoreError::Invalid.into());
        }
        
        if semaphore.has_waiting_tasks() {
            return Err(SemaphoreError::Pended.into());
        }
        
        // 重置信号量状态
        semaphore.reset();
        
        // 加回未使用列表
        LinkedList::tail_insert(
            &raw mut UNUSED_SEMAPHORE_LIST, 
            &raw mut semaphore.sem_list
        );
        
        Ok(())
    }
    
    /// 获取信号量
    #[inline]
    pub fn get_semaphore(semaphore_id: SemaphoreId) -> SemaphoreResult<&'static mut SemaphoreControlBlock> {
        let index = semaphore_id.get_index() as u32;
        if index >= SEM_LIMIT {
            return Err(SemaphoreError::Invalid.into());
        }
        
        Ok(Self::get_semaphore_by_index(index as usize))
    }
}