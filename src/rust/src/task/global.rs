use super::types::TaskCB;
use crate::utils::list::LinkedList;

/// 任务控制块数组
#[unsafe(export_name = "g_taskCBArray")]
pub static mut TASK_CB_ARRAY: *mut TaskCB = core::ptr::null_mut();

/// 空闲任务列表
#[unsafe(export_name = "g_losFreeTask")]
pub static mut TASK_FREE_LIST: LinkedList = LinkedList::UNINIT;

/// 回收任务列表
#[unsafe(export_name = "g_taskRecycleList")]
pub static mut TASK_RECYCLE_LIST: LinkedList = LinkedList::UNINIT;

/// 最大任务数量
#[unsafe(export_name = "g_taskMaxNum")]
pub static mut TASK_MAX_NUM: u32 = 0;

// unsafe extern "C" {
//     static mut g_percpu: [Percpu; LOSCFG_KERNEL_CORE_NUM];
// }
