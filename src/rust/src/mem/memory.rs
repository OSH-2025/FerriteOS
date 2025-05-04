use crate::bindings::config::get_os_sys_mem_size;
use crate::mem::mempool;
use crate::mem::multiple_dlink_head;
use crate::utils::list::LinkedList;
use crate::utils::printf::dprintf;
use crate::{container_of, list_for_each_entry, offset_of, os_check_null_return};

/// The start address of the exception interaction dynamic memory pool.
/// When the exception interaction feature is not supported, `m_aucSysMem0` equals `m_aucSysMem1`.
#[unsafe(no_mangle)]
#[allow(non_upper_case_globals)]
pub static mut m_aucSysMem0: *mut u8 = core::ptr::null_mut();

/// The start address of the system dynamic memory pool.
#[unsafe(no_mangle)]
#[allow(non_upper_case_globals)]
pub static mut m_aucSysMem1: *mut u8 = core::ptr::null_mut();

#[unsafe(link_section = ".data.init")]
#[unsafe(no_mangle)]
#[allow(non_upper_case_globals)]
pub static mut g_sys_mem_addr_end: usize = 0;

#[repr(C)]
pub union NodeInfo {
    pub free_node_info: LinkedList,
    pub used_node_info: UsedNodeInfo,
}

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct UsedNodeInfo {
    pub magic: u32,
    pub task_id: u16,
}

#[repr(C)]
pub struct LosMemCtlNode {
    pub node_info: NodeInfo,
    pub pre_node: *mut LosMemDynNode,
    pub size_and_flag: u32,
}

#[repr(C)]
pub struct LosMemDynNode {
    pub self_node: LosMemCtlNode,
}

impl LosMemDynNode {
    // fn set_task_id(&mut self, task_id: u32) {
    //     self.self_node.node_info.used_node_info.task_id = task_id as u16;
    // }

    // fn get_task_id(&self) -> u32 {
    //     unsafe { self.self_node.node_info.used_node_info.task_id as u32 }
    // }
}

const OS_MEM_NODE_USED_FLAG: u32 = 0x80000000;
const OS_MEM_NODE_ALIGNED_FLAG: u32 = 0x40000000;
const OS_MEM_NODE_ALIGNED_AND_USED_FLAG: u32 = OS_MEM_NODE_USED_FLAG | OS_MEM_NODE_ALIGNED_FLAG;

/// 获取节点的大小（去掉对齐和使用标志位）
#[inline]
fn os_mem_node_get_size(size_and_flag: u32) -> u32 {
    size_and_flag & !OS_MEM_NODE_ALIGNED_AND_USED_FLAG
}

#[inline]
fn os_mem_next_node(node: *mut LosMemDynNode) -> *mut LosMemDynNode {
    unsafe {
        let size = os_mem_node_get_size((*node).self_node.size_and_flag);
        (node as usize + size as usize) as *mut LosMemDynNode
    }
}

#[inline]
fn os_mem_node_get_used_flag(size_and_flag: u32) -> bool {
    size_and_flag & OS_MEM_NODE_USED_FLAG != 0
}

#[inline]
fn os_mem_list_delete(node: *mut LinkedList, _first_node: *const core::ffi::c_void) {
    unsafe {
        (*(*node).next).prev = (*node).prev;
        (*(*node).prev).next = (*node).next;
        (*node).next = core::ptr::null_mut();
        (*node).prev = core::ptr::null_mut();
    }
}

#[inline]
pub fn os_mem_list_add(
    list_node: *mut LinkedList,
    node: *mut LinkedList,
    _first_node: *const core::ffi::c_void,
) {
    unsafe {
        (*node).next = (*list_node).next;
        (*node).prev = list_node;
        (*(*list_node).next).prev = node;
        (*list_node).next = node;
    }
}

#[unsafe(export_name = "OsMemSystemInit")]
pub unsafe extern "C" fn os_mem_system_init(mem_start: usize) -> u32 {
    unsafe { m_aucSysMem1 = mem_start as *mut u8 };
    let pool_size = get_os_sys_mem_size();
    let ret = los_mem_init(unsafe { m_aucSysMem1 } as *mut core::ffi::c_void, pool_size);
    unsafe {
        dprintf(
            b"LiteOS system heap memory address:%p,size:0x%x\n\0" as *const u8,
            m_aucSysMem1,
            pool_size,
        )
    };
    unsafe { m_aucSysMem0 = m_aucSysMem1 };
    ret
}

#[inline]
fn os_mem_find_suitable_free_block(
    pool: *mut core::ffi::c_void,
    alloc_size: u32,
) -> Option<*mut LosMemDynNode> {
    let pool = pool as *mut mempool::LosMemPoolInfo;
    let head =
        mempool::os_mem_head(pool, alloc_size) as *mut multiple_dlink_head::LosMultipleDlinkHead;
    let mut list_node_head = mempool::os_mem_head(pool, alloc_size);
    while !list_node_head.is_null() {
        list_for_each_entry!(
            tmp_node,
            list_node_head,
            LosMemDynNode,
            self_node.node_info.free_node_info,
            {
                let size = (*tmp_node).self_node.size_and_flag;
                if size >= alloc_size {
                    return Some(tmp_node);
                }
            }
        );
        list_node_head = multiple_dlink_head::os_dlnk_next_multi_head(head, list_node_head);
    }
    Option::None
}

#[inline]
fn os_mem_clear_node(node: *mut LosMemDynNode) {
    unsafe { core::ptr::write_bytes(node, 0, 1) };
}

#[inline]
fn os_mem_merge_node(node: *mut LosMemDynNode) {
    unsafe {
        let merge_node = &mut *node;
        let prev_node = &mut *merge_node.self_node.pre_node;
        prev_node.self_node.size_and_flag += merge_node.self_node.size_and_flag;
        let next_node = &mut *((node as usize + merge_node.self_node.size_and_flag as usize)
            as *mut LosMemDynNode);
        next_node.self_node.pre_node = prev_node;
        os_mem_clear_node(node);
    }
}

#[inline]
fn os_mem_split_node(
    pool: *mut core::ffi::c_void,
    alloc_node: *mut LosMemDynNode,
    alloc_size: u32,
) {
    unsafe {
        let pool = pool as *mut mempool::LosMemPoolInfo;
        let first_node = mempool::os_mem_first_node(pool);
        let first_node = first_node as *const core::ffi::c_void;
        // 计算新空闲节点的地址
        let new_free_node = (alloc_node as usize + alloc_size as usize) as *mut LosMemDynNode;
        // 初始化新空闲节点
        (*new_free_node).self_node.pre_node = alloc_node;
        (*new_free_node).self_node.size_and_flag =
            (*alloc_node).self_node.size_and_flag - alloc_size;
        // 更新分配节点的大小
        (*alloc_node).self_node.size_and_flag = alloc_size;
        // 获取下一个节点
        let next_node = os_mem_next_node(alloc_node);
        // 更新下一个节点的前置节点指针
        (*next_node).self_node.pre_node = new_free_node;
        // 如果下一个节点未被使用，合并节点
        if !os_mem_node_get_used_flag((*next_node).self_node.size_and_flag) {
            os_mem_list_delete(
                &mut (*next_node).self_node.node_info.free_node_info as *mut LinkedList,
                first_node,
            );
            os_mem_merge_node(next_node);
        }
        // 获取新空闲节点对应的链表头
        let list_node_head = mempool::os_mem_head(pool, (*new_free_node).self_node.size_and_flag);
        os_check_null_return!(list_node_head);

        // 将新空闲节点添加到链表中
        os_mem_list_add(
            list_node_head,
            &mut (*new_free_node).self_node.node_info.free_node_info,
            first_node,
        );
    }
}

unsafe extern "C" {
    #[link_name = "LOS_MemInit"]
    unsafe fn los_mem_init_wrapper(pool: *mut core::ffi::c_void, size: u32) -> u32;
}

pub fn los_mem_init(pool: *mut core::ffi::c_void, size: u32) -> u32 {
    unsafe { los_mem_init_wrapper(pool, size) }
}
