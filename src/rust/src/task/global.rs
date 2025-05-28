use crate::{config::TASK_LIMIT, task::types::TaskCB, utils::list::LinkedList};
use core::sync::atomic::{AtomicU32, Ordering};

/// 任务控制块数组
#[unsafe(export_name = "g_taskCBArray")]
pub static mut TASK_CB_ARRAY: [TaskCB; TASK_LIMIT + 1] = [TaskCB::UNINIT; TASK_LIMIT + 1];

/// 空闲任务列表
#[unsafe(export_name = "g_losFreeTask")]
pub static mut FREE_TASK_LIST: LinkedList = LinkedList::UNINIT;

/// 回收任务列表
#[unsafe(export_name = "g_taskRecycleList")]
pub static mut TASK_RECYCLE_LIST: LinkedList = LinkedList::UNINIT;

pub fn get_tcb_mut(task_id: u32) -> &'static mut TaskCB {
    unsafe { &mut TASK_CB_ARRAY[task_id as usize] }
}

#[unsafe(export_name = "g_taskScheduled")]
pub static TASK_SCHEDULED: AtomicU32 = AtomicU32::new(0);

#[inline]
pub fn scheduler_active() -> bool {
    let current_state = TASK_SCHEDULED.load(Ordering::Acquire);
    current_state & 1 != 0
}
