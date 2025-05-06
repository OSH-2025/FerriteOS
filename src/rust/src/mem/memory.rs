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

#[inline]
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

fn os_mem_init(pool: *mut core::ffi::c_void, size: u32) -> Result<(), ()> {
    unsafe {
        let pool_info = pool as *mut LosMemPoolInfo;
        let pool_size = size;
        // 初始化内存池信息
        (*pool_info).pool = pool;
        (*pool_info).pool_size = pool_size;
        // 初始化多链表头
        multiple_dlink_head::os_dlnk_init_multi_head(os_mem_head_addr(pool_info));
        // 初始化第一个节点
        let new_node = os_mem_first_node(pool_info);
        (*new_node).self_node.size_and_flag =
            pool_size - (new_node as usize - pool as usize) as u32 - OS_MEM_NODE_HEAD_SIZE as u32;
        (*new_node).self_node.pre_node = os_mem_end_node(pool_info);
        // 获取对应链表头
        let list_node_head = os_mem_head(pool_info, (*new_node).self_node.size_and_flag);
        if list_node_head.is_null() {
            return Err(());
        }
        // 将新节点添加到链表尾部
        LinkedList::tail_insert(
            list_node_head,
            &mut (*new_node).self_node.node_info.free_node_info as *mut LinkedList,
        );
        // 初始化结束节点
        let end_node = os_mem_end_node(pool_info);
        os_mem_clear_node(end_node);
        (*end_node).self_node.pre_node = new_node;
        (*end_node).self_node.size_and_flag = OS_MEM_NODE_HEAD_SIZE as u32;
        os_mem_node_set_used_flag(&mut (*end_node).self_node.size_and_flag);
        os_mem_set_magic_num_and_task_id(end_node);
        // 初始化内存统计信息
        let zeroed_struct = core::mem::zeroed();
        (*pool_info).stat = zeroed_struct;
        (*pool_info).stat.mem_total_used = core::mem::size_of::<LosMemPoolInfo>() as u32
            + OS_MEM_NODE_HEAD_SIZE as u32
            + os_mem_node_get_size((*end_node).self_node.size_and_flag);
        (*pool_info).stat.mem_total_peak = (*pool_info).stat.mem_total_used;
        Ok(())
    }
}

#[unsafe(export_name = "LOS_MemInit")]
pub fn los_mem_init(pool: *mut core::ffi::c_void, mut size: u32) -> u32 {
    if pool.is_null() || size < OS_MEM_MIN_POOL_SIZE as u32 {
        return LOS_NOK;
    }
    if !is_aligned(size as usize, OS_MEM_ALIGN_SIZE)
        || !is_aligned(pool as usize, OS_MEM_ALIGN_SIZE)
    {
        // 打印警告信息
        unsafe {
            dprintf(
                b"pool [%p, %p) size 0x%x should be aligned with OS_MEM_ALIGN_SIZE\n\0"
                    as *const u8,
                pool as usize,
                pool as usize + size as usize,
                size,
            )
        };
        size = os_mem_align(size as usize, OS_MEM_ALIGN_SIZE) as u32 - OS_MEM_ALIGN_SIZE as u32;
    }
    // 加锁
    let mut int_save: u32 = 0;
    mem_lock(&mut int_save);
    // 初始化内存池
    match os_mem_init(pool, size) {
        Ok(_) => {
            mem_unlock(int_save);
            return LOS_OK;
        }
        Err(_) => {
            mem_unlock(int_save);
            return LOS_NOK;
        }
    }
}

#[unsafe(export_name = "LOS_MemAlloc")]
pub fn los_mem_alloc(pool: *mut core::ffi::c_void, size: u32) -> *mut core::ffi::c_void {
    let mut ptr: *mut core::ffi::c_void = core::ptr::null_mut();
    let mut int_save: u32 = 0;
    if pool.is_null() || size == 0 {
        return core::ptr::null_mut();
    }
    // TODO: g_MALLOC_HOOK
    mem_lock(&mut int_save);
    // 分配内存
    if os_mem_node_get_used_flag(size) || os_mem_node_get_aligned_flag(size) {
        return ptr;
    }
    ptr = os_mem_alloc_with_check(pool as *mut LosMemPoolInfo, size);
    mem_unlock(int_save);
    ptr
}

#[unsafe(export_name = "LOS_MemAllocAlign")]
pub fn los_mem_alloc_align(
    pool: *mut core::ffi::c_void,
    size: u32,
    boundary: u32,
) -> *mut core::ffi::c_void {
    let mut ptr: *mut core::ffi::c_void = core::ptr::null_mut();
    let mut int_save: u32 = 0;
    // 参数检查
    if pool.is_null()
        || size == 0
        || !boundary.is_power_of_two()
        || !is_aligned(
            boundary as usize,
            core::mem::size_of::<*mut core::ffi::c_void>(),
        )
    {
        return core::ptr::null_mut();
    }
    mem_lock(&mut int_save);
    // 分配内存
    loop {
        if (boundary - core::mem::size_of::<u32>() as u32) > (u32::MAX - size) {
            break;
        }
        let use_size = size + boundary - core::mem::size_of::<u32>() as u32;
        if os_mem_node_get_used_flag(use_size) || os_mem_node_get_aligned_flag(use_size) {
            break;
        }
        ptr = os_mem_alloc_with_check(pool as *mut LosMemPoolInfo, use_size);
        let aligned_ptr = os_mem_align(ptr as usize, boundary as usize) as *mut core::ffi::c_void;
        if ptr == aligned_ptr {
            break;
        }
        // 计算 gapSize 并存储
        let mut gap_size = (aligned_ptr as usize - ptr as usize) as u32;
        let alloc_node = unsafe { (ptr as *mut LosMemDynNode).offset(-1) };
        unsafe {
            os_mem_node_set_aligned_flag(&mut (*alloc_node).self_node.size_and_flag);
        }
        os_mem_node_set_aligned_flag(&mut gap_size);
        unsafe {
            *((aligned_ptr as usize - core::mem::size_of::<u32>() as usize) as *mut u32) = gap_size
        };
        ptr = aligned_ptr;
        break;
    }
    mem_unlock(int_save);
    ptr
}

fn os_do_mem_free(pool: *mut core::ffi::c_void, node: *mut LosMemDynNode) {
    os_mem_free_node(node, pool as *mut LosMemPoolInfo);
}

fn os_mem_free(pool: *mut core::ffi::c_void, ptr: *const core::ffi::c_void) -> u32 {
    let mut gap_size: u32;
    loop {
        unsafe {
            gap_size = *((ptr as usize - core::mem::size_of::<u32>()) as *const u32);
        }
        // 检查 gapSize 的标志位
        if os_mem_node_get_aligned_flag(gap_size) && os_mem_node_get_used_flag(gap_size) {
            unsafe {
                dprintf(
                    b"[%s:%d]gapSize:0x%x error\n\0" as *const u8,
                    b"OsMemFree\0" as *const u8,
                    line!(),
                    gap_size,
                )
            };
            break;
        }
        // 获取节点指针
        let mut node = (ptr as usize - OS_MEM_NODE_HEAD_SIZE as usize) as *mut LosMemDynNode;
        // 如果节点有对齐标志
        if os_mem_node_get_aligned_flag(gap_size) {
            gap_size = os_mem_node_get_aligned_gap_size(gap_size);
            if (gap_size & (OS_MEM_ALIGN_SIZE - 1) as u32) != 0
                || gap_size > (ptr as usize - OS_MEM_NODE_HEAD_SIZE as usize) as u32
            {
                unsafe { dprintf(b"illegal gapSize: 0x%x\n\0" as *const u8, gap_size) };
                break;
            }
            node = (ptr as usize - gap_size as usize - OS_MEM_NODE_HEAD_SIZE as usize)
                as *mut LosMemDynNode;
        }
        os_do_mem_free(pool, node);
        break;
    }
    LOS_OK
}

unsafe extern "C" {
    #[link_name = "OsMemSetMagicNumAndTaskID"]
    unsafe fn os_mem_set_magic_num_and_task_id(node: *mut LosMemDynNode);
}
