use super::memory::LosMemDynNode;
use super::mempool::LosMemPoolInfo;
use super::multiple_dlink_head::LosMultipleDlinkHead;
use crate::spinlock;
use crate::utils::list::LinkedList;

pub const OS_MAX_MULTI_DLNK_LOG2: u32 = 29;
pub const OS_MIN_MULTI_DLNK_LOG2: u32 = 4;
pub const OS_MULTI_DLNK_NUM: usize = (OS_MAX_MULTI_DLNK_LOG2 - OS_MIN_MULTI_DLNK_LOG2 + 1) as usize;
pub const OS_MULTI_DLNK_HEAD_SIZE: usize = core::mem::size_of::<LosMultipleDlinkHead>();
pub const OS_MEM_NODE_HEAD_SIZE: usize = core::mem::size_of::<LosMemDynNode>();
pub const OS_MEM_POOL_INFO_SIZE: usize = core::mem::size_of::<LosMemPoolInfo>();
pub const OS_MEM_NODE_USED_FLAG: u32 = 0x80000000;
pub const OS_MEM_NODE_ALIGNED_FLAG: u32 = 0x40000000;
pub const OS_MEM_NODE_ALIGNED_AND_USED_FLAG: u32 = OS_MEM_NODE_USED_FLAG | OS_MEM_NODE_ALIGNED_FLAG;
pub const OS_MEM_ALIGN_SIZE: usize = core::mem::size_of::<usize>();
pub const OS_MEM_MIN_POOL_SIZE: usize =
    OS_MULTI_DLNK_HEAD_SIZE + (2 * OS_MEM_NODE_HEAD_SIZE) + OS_MEM_POOL_INFO_SIZE;

unsafe extern "C" {
    pub static mut g_memSpin: spinlock::Spinlock;

    pub static mut m_aucSysMem0: *mut u8;

    pub static mut m_aucSysMem1: *mut u8;

    pub static mut g_sys_mem_addr_end: usize;

    pub static __heap_start: u8;
}

#[inline]
pub fn os_sys_mem_size() -> usize {
    unsafe {
        let sys_mem_end = g_sys_mem_addr_end;
        let aligned_heap_start = ((&__heap_start as *const _ as usize) + (63)) & !(63);
        sys_mem_end - aligned_heap_start
    }
}

#[inline]
pub fn os_mem_head_addr(pool: *mut LosMemPoolInfo) -> *mut LosMultipleDlinkHead {
    (pool as usize + OS_MEM_POOL_INFO_SIZE) as *mut LosMultipleDlinkHead
}

#[inline]
pub fn os_mem_first_node(pool: *mut LosMemPoolInfo) -> *mut LosMemDynNode {
    let head_addr = os_mem_head_addr(pool) as usize;
    (head_addr + OS_MULTI_DLNK_HEAD_SIZE) as *mut LosMemDynNode
}

#[inline]
pub fn os_mem_end_node(pool: *mut LosMemPoolInfo) -> *mut LosMemDynNode {
    unsafe {
        (pool as usize + (*pool).pool_size as usize - OS_MEM_NODE_HEAD_SIZE) as *mut LosMemDynNode
    }
}

#[inline]
pub fn os_mem_head(pool: *mut LosMemPoolInfo, size: u32) -> *mut LinkedList {
    let head_addr = os_mem_head_addr(pool);
    unsafe { (*head_addr).get_list_head_by_size(size) as *mut LinkedList }
}

#[inline]
pub fn os_mem_align(p: usize, align_size: usize) -> usize {
    (p + align_size - 1) & !(align_size - 1)
}

/// 检查是否对齐
#[inline]
pub fn is_aligned(value: usize, align: usize) -> bool {
    (value & (align - 1)) == 0
}

/// 获取节点的大小
#[inline]
pub fn os_mem_node_get_size(size_and_flag: u32) -> u32 {
    size_and_flag & !OS_MEM_NODE_ALIGNED_AND_USED_FLAG
}

#[inline]
pub fn os_mem_next_node(node: *mut LosMemDynNode) -> *mut LosMemDynNode {
    unsafe {
        let size = os_mem_node_get_size((*node).self_node.size_and_flag);
        (node as usize + size as usize) as *mut LosMemDynNode
    }
}

#[inline]
pub fn os_mem_node_get_used_flag(size_and_flag: u32) -> bool {
    size_and_flag & OS_MEM_NODE_USED_FLAG != 0
}

#[inline]
pub fn os_mem_node_set_used_flag(size_and_flag: &mut u32) {
    *size_and_flag |= OS_MEM_NODE_USED_FLAG;
}

#[inline]
pub fn os_mem_node_get_aligned_flag(size_and_flag: u32) -> bool {
    size_and_flag & OS_MEM_NODE_ALIGNED_FLAG != 0
}

#[inline]
pub fn os_mem_node_set_aligned_flag(size_and_flag: &mut u32) {
    *size_and_flag |= OS_MEM_NODE_ALIGNED_FLAG;
}

#[inline]
pub fn os_mem_node_get_aligned_gap_size(size_and_flag: u32) -> u32 {
    size_and_flag & !OS_MEM_NODE_ALIGNED_FLAG
}

#[inline]
pub fn os_mem_magic_valid(node: *mut LosMemDynNode) -> bool {
    unsafe {
        let magic = (*node).self_node.node_info.used_node_info.magic;
        let magic_ptr = &(*node).self_node.node_info.used_node_info.magic as *const _ as u32;
        (magic ^ magic_ptr) == u32::MAX
    }
}

#[inline]
pub fn mem_lock(state: &mut u32) {
    spinlock::los_spin_lock_save(&raw mut g_memSpin, state);
}

#[inline]
pub fn mem_unlock(state: u32) {
    spinlock::los_spin_unlock_restore(&raw mut g_memSpin, state);
}
