use crate::{config::TIMER_LIMIT, utils::list::LinkedList};

use crate::timer::types::{TimerControlBlock, TimerHandler, TimerMode, TimerState};

pub static mut UNUSED_TIMER_LIST: LinkedList = LinkedList::new();

#[unsafe(export_name = "g_swtmrCBArray")]
pub static mut TIMER_POOL: [TimerControlBlock; TIMER_LIMIT as usize] =
    [TimerControlBlock::UNINIT; TIMER_LIMIT as usize];

/// 定时器池管理器
pub struct TimerPool;

impl TimerPool {
    #[inline]
    pub fn init() {
        LinkedList::init(&raw mut UNUSED_TIMER_LIST);
        for id in 0..TIMER_LIMIT {
            let timer = Self::get_timer_by_index(id as usize);
            timer.set_id(id.into());
            LinkedList::tail_insert(
                &raw mut UNUSED_TIMER_LIST,
                &raw mut timer.sort_list.sort_link_node,
            );
        }
    }

    #[inline]
    pub fn has_available() -> bool {
        !LinkedList::is_empty(&raw const UNUSED_TIMER_LIST)
    }

    /// 根据索引获取定时器控制块
    #[inline]
    pub fn get_timer_by_index(index: usize) -> &'static mut TimerControlBlock {
        unsafe { &mut TIMER_POOL[index] }
    }

    /// 分配一个定时器控制块
    pub fn allocate(
        mode: TimerMode,
        timeout: u32,
        handler: TimerHandler,
    ) -> &'static mut TimerControlBlock {
        // 从空闲链表头部取出一个节点
        let node = LinkedList::first(&raw const UNUSED_TIMER_LIST);
        LinkedList::remove(node);
        // 获取包含该节点的TimerControlBlock
        let timer = TimerControlBlock::from_list(node);
        // 初始化定时器状态
        timer.initialize(mode, timeout, handler);
        timer
    }

    /// 回收定时器控制块
    pub fn deallocate(timer: &mut TimerControlBlock) {
        LinkedList::tail_insert(
            &raw mut UNUSED_TIMER_LIST,
            &mut timer.sort_list.sort_link_node,
        );
        timer.set_state(TimerState::Unused);
    }
}
