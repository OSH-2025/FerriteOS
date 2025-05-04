use crate::bindings::config::{LOS_NOK, LOS_OK, get_os_sys_mem_size};
use crate::utils::list::LinkedList;
use crate::utils::printf::dprintf;
use crate::{container_of, list_for_each_entry, offset_of, os_check_null_return};

use super::defs::*;
use super::mempool::{LosMemPoolInfo, LosMemPoolStatus};
use super::memstat;
use super::multiple_dlink_head;

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
    fn set_task_id(&mut self, task_id: u32) {
        self.self_node.node_info.used_node_info.task_id = task_id as u16;
    }

    fn get_task_id(&self) -> u32 {
        unsafe { self.self_node.node_info.used_node_info.task_id as u32 }
    }
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
    let pool = pool as *mut LosMemPoolInfo;
    let head = os_mem_head(pool, alloc_size) as *mut multiple_dlink_head::LosMultipleDlinkHead;
    let mut list_node_head = os_mem_head(pool, alloc_size);
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
        let pool = pool as *mut LosMemPoolInfo;
        let first_node = os_mem_first_node(pool) as *const core::ffi::c_void;
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
        let list_node_head = os_mem_head(pool, (*new_free_node).self_node.size_and_flag);
        os_check_null_return!(list_node_head);

        // 将新空闲节点添加到链表中
        os_mem_list_add(
            list_node_head,
            &mut (*new_free_node).self_node.node_info.free_node_info,
            first_node,
        );
    }
}

fn os_mem_free_node(node: *mut LosMemDynNode, pool: *mut LosMemPoolInfo) {
    unsafe {
        let first_node = os_mem_first_node(pool) as *const core::ffi::c_void;
        // 更新内存统计信息
        memstat::os_memstat_task_used_dec(
            &mut (*pool).stat,
            os_mem_node_get_size((*node).self_node.size_and_flag),
            (*node).get_task_id(),
        );
        // 更新节点的大小，去掉标志位
        (*node).self_node.size_and_flag = os_mem_node_get_size((*node).self_node.size_and_flag);

        if !(*node).self_node.pre_node.is_null()
            && !os_mem_node_get_used_flag((*(*node).self_node.pre_node).self_node.size_and_flag)
        {
            let pre_node = (*node).self_node.pre_node;
            os_mem_merge_node(node);

            let next_node = os_mem_next_node(pre_node);
            if !os_mem_node_get_used_flag((*next_node).self_node.size_and_flag) {
                os_mem_list_delete(
                    &mut (*next_node).self_node.node_info.free_node_info as *mut LinkedList,
                    first_node,
                );
                os_mem_merge_node(next_node);
            }

            os_mem_list_delete(
                &mut (*pre_node).self_node.node_info.free_node_info as *mut LinkedList,
                first_node,
            );

            let list_node_head = os_mem_head(pool, (*pre_node).self_node.size_and_flag);
            os_check_null_return!(list_node_head);

            os_mem_list_add(
                list_node_head,
                &mut (*pre_node).self_node.node_info.free_node_info,
                first_node,
            );
        } else {
            let next_node = os_mem_next_node(node);
            if !os_mem_node_get_used_flag((*next_node).self_node.size_and_flag) {
                os_mem_list_delete(
                    &mut (*next_node).self_node.node_info.free_node_info as *mut LinkedList,
                    first_node,
                );
                os_mem_merge_node(next_node);
            }

            let list_node_head = os_mem_head(pool, (*node).self_node.size_and_flag);
            os_check_null_return!(list_node_head);

            os_mem_list_add(
                list_node_head,
                &mut (*node).self_node.node_info.free_node_info,
                first_node,
            );
        }
    }
}

fn os_mem_info_get(pool_info: *mut LosMemPoolInfo, pool_status: &mut LosMemPoolStatus) -> u32 {
    unsafe {
        let tmp_node = os_mem_end_node(pool_info);
        let tmp_node = os_mem_align(tmp_node as usize, OS_MEM_ALIGN_SIZE) as *mut LosMemDynNode;

        if !os_mem_magic_valid(tmp_node) {
            dprintf(b"Wrong memory pool address: {%p}\n" as *const u8, pool_info);
            return LOS_NOK;
        }

        let mut total_used_size = 0;
        let mut total_free_size = 0;
        let mut max_free_node_size = 0;
        let mut used_node_num = 0;
        let mut free_node_num = 0;

        let mut tmp_node = os_mem_first_node(pool_info);
        while tmp_node <= os_mem_end_node(pool_info) {
            if !os_mem_node_get_used_flag((*tmp_node).self_node.size_and_flag) {
                free_node_num += 1;
                total_free_size += os_mem_node_get_size((*tmp_node).self_node.size_and_flag);
                max_free_node_size = u32::max(
                    max_free_node_size,
                    os_mem_node_get_size((*tmp_node).self_node.size_and_flag),
                );
            } else {
                used_node_num += 1;
                total_used_size += os_mem_node_get_size((*tmp_node).self_node.size_and_flag);
            }
            tmp_node = os_mem_next_node(tmp_node);
        }

        pool_status.total_used_size = total_used_size;
        pool_status.total_free_size = total_free_size;
        pool_status.max_free_node_size = max_free_node_size;
        pool_status.used_node_num = used_node_num;
        pool_status.free_node_num = free_node_num;
        pool_status.usage_water_line = (*pool_info).stat.mem_total_peak;
        LOS_OK
    }
}

#[unsafe(export_name = "OsMemInfoPrint")]
pub fn os_mem_info_print(pool_info: *mut LosMemPoolInfo) {
    let mut status: LosMemPoolStatus = LosMemPoolStatus {
        total_used_size: 0,
        total_free_size: 0,
        max_free_node_size: 0,
        used_node_num: 0,
        free_node_num: 0,
        usage_water_line: 0,
    };
    if os_mem_info_get(pool_info, &mut status) == LOS_NOK {
        return;
    }
    unsafe {
        dprintf(
            b"pool addr          pool size    used size     free size    \
             max free node size   used node num     free node num      \
             UsageWaterLine\n\0" as *const u8,
        );
        dprintf(
            b"---------------    --------     -------       --------     \
             --------------       -------------      ------------      \
             ------------\n\0" as *const u8,
        );
        dprintf(
            b"%-16p   0x%-8x   0x%-8x    0x%-8x   0x%-16x   0x%-13x    0x%-13x    \
             0x%-13x\n\0" as *const u8,
            (*pool_info).pool,
            (*pool_info).pool_size,
            status.total_used_size,
            status.total_free_size,
            status.max_free_node_size,
            status.used_node_num,
            status.free_node_num,
            (*pool_info).stat.mem_total_peak,
        );
    }
}

fn os_mem_alloc_with_check(pool: *mut LosMemPoolInfo, size: u32) -> *mut core::ffi::c_void {
    let first_node = os_mem_first_node(pool) as *const core::ffi::c_void;

    let alloc_size = os_mem_align(size as usize + OS_MEM_NODE_HEAD_SIZE, OS_MEM_ALIGN_SIZE) as u32;

    let alloc_node =
        match os_mem_find_suitable_free_block(pool as *mut core::ffi::c_void, alloc_size) {
            Some(node) => node,
            None => {
                // TODO os_mem_info_alert
                return core::ptr::null_mut();
            }
        };
    unsafe {
        if (alloc_size + OS_MEM_NODE_HEAD_SIZE as u32 + OS_MEM_ALIGN_SIZE as u32)
            <= (*alloc_node).self_node.size_and_flag
        {
            os_mem_split_node(pool as *mut core::ffi::c_void, alloc_node, alloc_size);
        }
        os_mem_list_delete(
            &mut (*alloc_node).self_node.node_info.free_node_info as *mut LinkedList,
            first_node,
        );
        os_mem_set_magic_num_and_task_id(alloc_node);
        os_mem_node_set_used_flag(&mut (*alloc_node).self_node.size_and_flag);
        memstat::os_memstat_task_used_dec(
            &mut (*pool).stat,
            os_mem_node_get_size((*alloc_node).self_node.size_and_flag),
            (*alloc_node).get_task_id(),
        );
        alloc_node.add(1) as *mut core::ffi::c_void
    }
}

#[inline]
fn os_mem_realloc_smaller(
    pool: *mut LosMemPoolInfo,
    alloc_size: u32,
    node: *mut LosMemDynNode,
    node_size: u32,
) {
    unsafe {
        if (alloc_size + OS_MEM_NODE_HEAD_SIZE as u32 + OS_MEM_ALIGN_SIZE as u32) <= node_size {
            (*node).self_node.size_and_flag = node_size;
            os_mem_split_node(pool as *mut core::ffi::c_void, node, alloc_size);
            os_mem_node_set_used_flag(&mut (*node).self_node.size_and_flag);
            memstat::os_memstat_task_used_dec(
                &mut (*pool).stat,
                node_size - alloc_size,
                (*node).get_task_id(),
            );
        }
    }
}

#[inline]
fn os_mem_merge_node_for_realloc_bigger(
    pool: *mut LosMemPoolInfo,
    alloc_size: u32,
    node: *mut LosMemDynNode,
    node_size: u32,
    next_node: *mut LosMemDynNode,
) {
    unsafe {
        let first_node = os_mem_first_node(pool) as *const core::ffi::c_void;
        (*node).self_node.size_and_flag = node_size;
        os_mem_list_delete(
            &mut (*next_node).self_node.node_info.free_node_info as *mut LinkedList,
            first_node,
        );
        os_mem_merge_node(next_node);
        if (alloc_size + OS_MEM_NODE_HEAD_SIZE as u32 + OS_MEM_ALIGN_SIZE as u32)
            <= (*node).self_node.size_and_flag
        {
            os_mem_split_node(pool as *mut core::ffi::c_void, node, alloc_size);
        }
        memstat::os_memstat_task_used_inc(
            &mut (*pool).stat,
            (*node).self_node.size_and_flag - node_size,
            (*node).get_task_id(),
        );
        os_mem_node_set_used_flag(&mut (*node).self_node.size_and_flag);
    }
}

unsafe extern "C" {
    #[link_name = "LOS_MemInit"]
    unsafe fn los_mem_init_wrapper(pool: *mut core::ffi::c_void, size: u32) -> u32;

    #[link_name = "OsMemSetMagicNumAndTaskID"]
    unsafe fn os_mem_set_magic_num_and_task_id(node: *mut LosMemDynNode);
}

pub fn los_mem_init(pool: *mut core::ffi::c_void, size: u32) -> u32 {
    unsafe { los_mem_init_wrapper(pool, size) }
}
