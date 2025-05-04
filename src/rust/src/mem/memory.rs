use crate::bindings::config::get_os_sys_mem_size;
use crate::mem::mempool;
use crate::mem::multiple_dlink_head;
use crate::utils::list::LinkedList;
use crate::utils::printf::dprintf;
use crate::{container_of, list_for_each_entry, offset_of};

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

fn os_mem_clear_node(node: *mut LosMemDynNode) {
    unsafe { core::ptr::write_bytes(node, 0, 1) };
}

pub fn os_mem_merge_node(node: *mut LosMemDynNode) {
    unsafe {
        // 获取前一个节点并合并当前节点的大小到前一个节点
        (*(*node).self_node.pre_node).self_node.size_and_flag += (*node).self_node.size_and_flag;

        // 计算下一个节点的地址
        let next_node = (node as usize + (*node).self_node.size_and_flag as usize) as *mut LosMemDynNode;

        // 更新下一个节点的前置节点指针
        (*next_node).self_node.pre_node = (*node).self_node.pre_node;

        #[cfg(feature = "loscfg_mem_head_backup")]
        {
            // 如果启用了头部备份功能，保存节点信息
            mempool::os_mem_node_save((*node).self_node.pre_node);
            mempool::os_mem_node_save(next_node);
        }

        // 清除当前节点
        os_mem_clear_node(node);
    }
}

unsafe extern "C" {
    #[link_name = "LOS_MemInit"]
    unsafe fn los_mem_init_wrapper(pool: *mut core::ffi::c_void, size: u32) -> u32;
}

pub fn los_mem_init(pool: *mut core::ffi::c_void, size: u32) -> u32 {
    unsafe { los_mem_init_wrapper(pool, size) }
}
