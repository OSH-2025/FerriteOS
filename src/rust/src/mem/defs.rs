use super::memory::LosMemDynNode;
use super::mempool::LosMemPoolInfo;
use super::multiple_dlink_head::{LosMultipleDlinkHead, OS_MULTI_DLNK_HEAD_SIZE};
use crate::utils::list::LinkedList;

pub const OS_MEM_NODE_HEAD_SIZE: usize = core::mem::size_of::<LosMemDynNode>();
pub const OS_MEM_ALIGN_SIZE: usize = core::mem::size_of::<usize>();
pub const OS_MEM_NODE_USED_FLAG: u32 = 0x80000000;
pub const OS_MEM_NODE_ALIGNED_FLAG: u32 = 0x40000000;
pub const OS_MEM_NODE_ALIGNED_AND_USED_FLAG: u32 = OS_MEM_NODE_USED_FLAG | OS_MEM_NODE_ALIGNED_FLAG;

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
pub fn os_mem_magic_valid(node: *mut LosMemDynNode) -> bool {
    unsafe {
        let magic = (*node).self_node.node_info.used_node_info.magic;
        let magic_ptr = &(*node).self_node.node_info.used_node_info.magic as *const _ as u32;
        (magic ^ magic_ptr) == u32::MAX
    }
}
