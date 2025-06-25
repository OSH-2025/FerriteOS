//! 等待队列管理

use crate::mutex::types::MutexControlBlock;
use crate::task::types::TaskCB;
use crate::utils::list::LinkedList;

/// 等待管理器
pub struct WaitManager;

impl WaitManager {
    /// 根据优先级查找等待位置
    #[cfg(feature = "mutex-waitmode-prio")]
    pub fn find_wait_position<'a>(
        run_task: &mut TaskCB,
        mutex: &'a mut MutexControlBlock,
    ) -> &'a mut LinkedList {
        use core::ptr::addr_of;

        if LinkedList::is_empty(&raw const mutex.mux_list) {
            return &mut mutex.mux_list;
        }
        let first_node = LinkedList::first(&raw const mutex.mux_list);
        let first_task = TaskCB::from_pend_list(first_node);
        let last_node = LinkedList::last(&raw const mutex.mux_list);
        let last_task = TaskCB::from_pend_list(last_node);

        // 如果当前任务优先级最高，插入到头部
        if first_task.priority > run_task.priority {
            return unsafe { &mut *mutex.mux_list.next };
        }

        // 如果当前任务优先级最低，插入到尾部
        if last_task.priority <= run_task.priority {
            return &mut mutex.mux_list;
        }
        // 在中间查找合适位置
        let mut cur_task = TaskCB::from_pend_list(mutex.mux_list.next);

        while addr_of!(cur_task.pend_list) != addr_of!(mutex.mux_list) {
            if cur_task.priority < run_task.priority {
                cur_task = TaskCB::from_pend_list(cur_task.pend_list.next);
            } else if cur_task.priority > run_task.priority {
                return &mut cur_task.pend_list;
            } else {
                return unsafe { &mut *cur_task.pend_list.next };
            }
        }
        &mut mutex.mux_list
    }

    /// 简单的FIFO等待（不考虑优先级）
    #[cfg(not(feature = "mutex-waitmode-prio"))]
    pub fn find_wait_position(_task: *mut TaskCB, mutex: &MutexCB) -> *mut LinkedList {
        &mutex.mux_list as *const _ as *mut _
    }
}
