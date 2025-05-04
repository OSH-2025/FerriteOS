use crate::mem::memstat::Memstat;
use crate::mem::multiple_dlink_head::LosMultipleDlinkHead;
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

pub fn os_mem_head_addr(pool: *mut LosMemPoolInfo) -> *mut LosMultipleDlinkHead {
    (pool as usize + core::mem::size_of::<LosMemPoolInfo>()) as *mut LosMultipleDlinkHead
}

pub fn os_mem_head(pool: *mut LosMemPoolInfo, size: u32) -> *mut LinkedList {
    let head_addr = unsafe { &*os_mem_head_addr(pool) };
    head_addr.get_list_head_by_size(size) as *mut LinkedList
}
