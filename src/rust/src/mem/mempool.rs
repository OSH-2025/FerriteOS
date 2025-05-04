use super::memory::LosMemDynNode;
use super::memstat::Memstat;
use super::multiple_dlink_head::{LosMultipleDlinkHead, OS_MULTI_DLNK_HEAD_SIZE};
use crate::utils::list::LinkedList;

#[repr(C)]
pub struct LosMemPoolInfo {
    /// Starting address of a memory pool
    pub pool: *mut core::ffi::c_void,
    /// Memory pool size
    pub pool_size: u32,
    /// Memory statistics (enabled with LOSCFG_MEM_TASK_STAT)
    pub stat: Memstat,
}

#[inline]
pub fn os_mem_head_addr(pool: *mut LosMemPoolInfo) -> *mut LosMultipleDlinkHead {
    (pool as usize + core::mem::size_of::<LosMemPoolInfo>()) as *mut LosMultipleDlinkHead
}

#[inline]
pub fn os_mem_first_node(pool: *mut LosMemPoolInfo) -> *mut LosMemDynNode {
    let head_addr = os_mem_head_addr(pool) as usize;
    (head_addr + OS_MULTI_DLNK_HEAD_SIZE) as *mut LosMemDynNode
}

#[inline]
pub fn os_mem_head(pool: *mut LosMemPoolInfo, size: u32) -> *mut LinkedList {
    let head_addr = os_mem_head_addr(pool);
    unsafe { (*head_addr).get_list_head_by_size(size) as *mut LinkedList }
}
