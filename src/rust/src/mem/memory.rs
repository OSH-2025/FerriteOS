use crate::config::{NOK, OK, OS_INVALID};
use crate::list_for_each_entry;
use crate::utils::list::LinkedList;

use super::defs::*;
use super::mempool::{LosMemPoolInfo, LosMemPoolStatus};
#[cfg(feature = "task_static_allocation")]
use super::memstat;
use super::multiple_dlink_head;
use super::multiple_dlink_head::LosMultipleDlinkHead;

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
pub extern "C" fn os_mem_system_init(mem_start: usize) -> u32 {
    let ret = unsafe {
        m_aucSysMem1 = mem_start as *mut u8;
        let pool_size = os_sys_mem_size() as u32;
        let ret = los_mem_init(mem_start as *mut core::ffi::c_void, pool_size);
        // dprintf(
        //     b"LiteOS system heap memory address:%p,size:0x%x\n\0" as *const u8,
        //     m_aucSysMem1,
        //     pool_size,
        // );
        m_aucSysMem0 = m_aucSysMem1;
        ret
    };
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
        let next_node = os_mem_next_node(new_free_node);
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
        // os_check_null_return!(list_node_head);

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
        #[cfg(feature = "task_static_allocation")]
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
            // os_check_null_return!(list_node_head);

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
            // os_check_null_return!(list_node_head);

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
            // dprintf(b"Wrong memory pool address: {%p}\n" as *const u8, pool_info);
            return NOK;
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
        #[cfg(feature = "task_static_allocation")]
        {
            pool_status.usage_water_line = (*pool_info).stat.mem_total_peak;
        }
        OK
    }
}

#[unsafe(export_name = "OsMemInfoPrint")]
pub fn os_mem_info_print(pool_info: *mut LosMemPoolInfo) {
    let mut status: LosMemPoolStatus = LosMemPoolStatus::default();
    if os_mem_info_get(pool_info, &mut status) == NOK {
        return;
    }
    #[cfg(feature = "task_static_allocation")]
    // unsafe {
    // dprintf(
    //     b"pool addr          pool size    used size     free size    \
    //      max free node size   used node num     free node num      \
    //      UsageWaterLine\n\0" as *const u8,
    // );
    // dprintf(
    //     b"---------------    --------     -------       --------     \
    //      --------------       -------------      ------------      \
    //      ------------\n\0" as *const u8,
    // );
    // dprintf(
    //     b"%-16p   0x%-8x   0x%-8x    0x%-8x   0x%-16x   0x%-13x    0x%-13x    \
    //      0x%-13x\n\0" as *const u8,
    //     (*pool_info).pool,
    //     (*pool_info).pool_size,
    //     status.total_used_size,
    //     status.total_free_size,
    //     status.max_free_node_size,
    //     status.used_node_num,
    //     status.free_node_num,
    //     status.usage_water_line,
    // );
    // }
    #[cfg(not(feature = "task_static_allocation"))]
    unsafe {
        dprintf(
            b"pool addr          pool size    used size     free size    \
             max free node size   used node num     free node num\n\0" as *const u8,
        );
        dprintf(
            b"---------------    --------     -------       --------     \
             --------------       -------------      ------------\n\0" as *const u8,
        );
        dprintf(
            b"%-16p   0x%-8x   0x%-8x    0x%-8x   0x%-16x   0x%-13x    0x%-13x\n\0" as *const u8,
            (*pool_info).pool,
            (*pool_info).pool_size,
            status.total_used_size,
            status.total_free_size,
            status.max_free_node_size,
            status.used_node_num,
            status.free_node_num,
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
        #[cfg(feature = "task_static_allocation")]
        memstat::os_memstat_task_used_inc(
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
            #[cfg(feature = "task_static_allocation")]
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
        #[cfg(feature = "task_static_allocation")]
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
        #[cfg(feature = "task_static_allocation")]
        {
            let zeroed_struct = core::mem::zeroed();
            (*pool_info).stat = zeroed_struct;
            (*pool_info).stat.mem_total_used =
                (OS_MEM_POOL_INFO_SIZE + OS_MEM_NODE_HEAD_SIZE + OS_MEM_NODE_HEAD_SIZE) as u32;
            (*pool_info).stat.mem_total_peak = (*pool_info).stat.mem_total_used;
        }
        Ok(())
    }
}

#[unsafe(export_name = "LOS_MemInit")]
pub fn los_mem_init(pool: *mut core::ffi::c_void, mut size: u32) -> u32 {
    if pool.is_null() || size < OS_MEM_MIN_POOL_SIZE as u32 {
        return NOK;
    }
    if !is_aligned(size as usize, OS_MEM_ALIGN_SIZE)
        || !is_aligned(pool as usize, OS_MEM_ALIGN_SIZE)
    {
        // 打印警告信息
        // unsafe {
        //     dprintf(
        //         b"pool [%p, %p) size 0x%x should be aligned with OS_MEM_ALIGN_SIZE\n\0"
        //             as *const u8,
        //         pool as usize,
        //         pool as usize + size as usize,
        //         size,
        //     )
        // };
        size = os_mem_align(size as usize, OS_MEM_ALIGN_SIZE) as u32 - OS_MEM_ALIGN_SIZE as u32;
    }
    // 加锁
    let mut int_save: u32 = 0;
    mem_lock(&mut int_save);
    // 初始化内存池
    match os_mem_init(pool, size) {
        Ok(_) => {
            mem_unlock(int_save);
            return OK;
        }
        Err(_) => {
            mem_unlock(int_save);
            return NOK;
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
            // unsafe {
            //     dprintf(
            //         b"[%s:%d]gapSize:0x%x error\n\0" as *const u8,
            //         b"OsMemFree\0" as *const u8,
            //         line!(),
            //         gap_size,
            //     )
            // };
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
                // unsafe { dprintf(b"illegal gapSize: 0x%x\n\0" as *const u8, gap_size) };
                break;
            }
            node = (ptr as usize - gap_size as usize - OS_MEM_NODE_HEAD_SIZE as usize)
                as *mut LosMemDynNode;
        }
        os_do_mem_free(pool, node);
        break;
    }
    OK
}

#[unsafe(export_name = "LOS_MemFree")]
pub fn los_mem_free(pool: *mut core::ffi::c_void, ptr: *mut core::ffi::c_void) -> u32 {
    let mut int_save: u32 = 0;
    // 参数检查
    if pool.is_null()
        || ptr.is_null()
        || !is_aligned(
            pool as usize,
            core::mem::size_of::<*mut core::ffi::c_void>(),
        )
        || !is_aligned(ptr as usize, core::mem::size_of::<*mut core::ffi::c_void>())
    {
        return NOK;
    }
    // 加锁
    mem_lock(&mut int_save);
    // 尝试释放普通内存
    let ret = os_mem_free(pool, ptr);
    // 解锁
    mem_unlock(int_save);
    ret
}

fn os_get_real_ptr(
    pool: *const core::ffi::c_void,
    ptr: *mut core::ffi::c_void,
) -> Option<*mut core::ffi::c_void> {
    let mut real_ptr = ptr;
    let mut gap_size: u32;
    unsafe {
        gap_size = *((ptr as usize - core::mem::size_of::<u32>()) as *const u32);
    }
    // 检查 gapSize 的标志位
    if os_mem_node_get_aligned_flag(gap_size) && os_mem_node_get_used_flag(gap_size) {
        // unsafe {
        //     dprintf(
        //         b"[%s:%d]gapSize:0x%x error\n\0" as *const u8,
        //         b"os_get_real_ptr\0" as *const u8,
        //         line!(),
        //         gap_size,
        //     )
        // };
        return None;
    }
    // 如果节点有对齐标志
    if os_mem_node_get_aligned_flag(gap_size) {
        gap_size = os_mem_node_get_aligned_gap_size(gap_size);
        // 检查 gapSize 的合法性
        if (gap_size & (OS_MEM_ALIGN_SIZE - 1) as u32) != 0
            || gap_size > (ptr as usize - OS_MEM_NODE_HEAD_SIZE as usize - pool as usize) as u32
        {
            // unsafe {
            //     dprintf(
            //         b"[%s:%d]gapSize:0x%x error\n\0" as *const u8,
            //         b"os_get_real_ptr\0" as *const u8,
            //         line!(),
            //         gap_size,
            //     )
            // };
            return None;
        }
        // 计算实际指针
        real_ptr = (ptr as usize - gap_size as usize) as *mut core::ffi::c_void;
    }
    Some(real_ptr)
}

fn os_mem_realloc(
    pool: *mut core::ffi::c_void,
    ptr: *mut core::ffi::c_void,
    size: u32,
) -> *mut core::ffi::c_void {
    let alloc_size = os_mem_align(size as usize + OS_MEM_NODE_HEAD_SIZE, OS_MEM_ALIGN_SIZE) as u32;
    // 获取实际指针
    let real_ptr = match os_get_real_ptr(pool, ptr) {
        Some(real_ptr) => real_ptr,
        None => {
            // unsafe {
            //     dprintf(
            //         b"[%s:%d]get real ptr error\n\0" as *const u8,
            //         b"os_mem_realloc\0" as *const u8,
            //         line!(),
            //     )
            // };
            return core::ptr::null_mut();
        }
    };

    let node = (real_ptr as usize - OS_MEM_NODE_HEAD_SIZE as usize) as *mut LosMemDynNode;

    // 获取节点大小
    let node_size: u32;
    unsafe {
        node_size = os_mem_node_get_size((*node).self_node.size_and_flag);
    }

    // 如果当前节点大小足够，调整为更小的分配
    if node_size >= alloc_size {
        os_mem_realloc_smaller(pool as *mut LosMemPoolInfo, alloc_size, node, node_size);
        return ptr;
    }

    // 获取下一个节点
    let next_node = os_mem_next_node(node);
    // 如果下一个节点未被使用且合并后大小足够，合并节点
    unsafe {
        if !os_mem_node_get_used_flag((*next_node).self_node.size_and_flag)
            && ((*next_node).self_node.size_and_flag + node_size >= alloc_size)
        {
            os_mem_merge_node_for_realloc_bigger(
                pool as *mut LosMemPoolInfo,
                alloc_size,
                node,
                node_size,
                next_node,
            );
            return ptr;
        }
    }

    // 分配新的内存块
    let tmp_ptr = os_mem_alloc_with_check(pool as *mut LosMemPoolInfo, size);
    if !tmp_ptr.is_null() {
        let gap_size = (ptr as usize - real_ptr as usize) as u32;

        unsafe {
            core::ptr::copy_nonoverlapping(
                real_ptr,
                tmp_ptr,
                (node_size - OS_MEM_NODE_HEAD_SIZE as u32 - gap_size) as usize,
            )
        };

        // 释放旧节点
        os_mem_free_node(node, pool as *mut LosMemPoolInfo);
    }

    tmp_ptr
}

#[unsafe(export_name = "LOS_MemRealloc")]
pub fn los_mem_realloc(
    pool: *mut core::ffi::c_void,
    ptr: *mut core::ffi::c_void,
    size: u32,
) -> *mut core::ffi::c_void {
    let mut int_save: u32 = 0;

    // 参数检查
    if os_mem_node_get_used_flag(size) || os_mem_node_get_aligned_flag(size) || pool.is_null() {
        return core::ptr::null_mut();
    }

    // 如果 ptr 为 NULL，直接分配新内存
    if ptr.is_null() {
        return los_mem_alloc(pool, size);
    }

    // 如果 size 为 0，释放内存并返回 NULL
    if size == 0 {
        los_mem_free(pool, ptr);
        return core::ptr::null_mut();
    }

    // 加锁
    mem_lock(&mut int_save);

    // 尝试重新分配普通内存
    let new_ptr = os_mem_realloc(pool, ptr, size);

    // 解锁
    mem_unlock(int_save);

    new_ptr
}

#[unsafe(export_name = "LOS_MemTotalUsedGet")]
pub fn los_mem_total_used_get(pool: *mut LosMemPoolInfo) -> u32 {
    if pool.is_null() {
        return NOK;
    }

    let mut mem_used: u32 = 0;
    let mut int_save: u32 = 0;

    mem_lock(&mut int_save);
    unsafe {
        let mut tmp_node = os_mem_first_node(pool);
        while tmp_node <= os_mem_end_node(pool) {
            if os_mem_node_get_used_flag((*tmp_node).self_node.size_and_flag) {
                mem_used += os_mem_node_get_size((*tmp_node).self_node.size_and_flag);
            }
            tmp_node = os_mem_next_node(tmp_node);
        }
    }
    mem_unlock(int_save);

    mem_used
}

#[unsafe(export_name = "LOS_MemUsedBlksGet")]
pub fn los_mem_used_blks_get(pool: *mut LosMemPoolInfo) -> u32 {
    if pool.is_null() {
        return NOK;
    }

    let mut blk_nums: u32 = 0;
    let mut int_save: u32 = 0;

    mem_lock(&mut int_save);
    unsafe {
        let mut tmp_node = os_mem_first_node(pool);
        while tmp_node <= os_mem_end_node(pool) {
            if os_mem_node_get_used_flag((*tmp_node).self_node.size_and_flag) {
                blk_nums += 1;
            }
            tmp_node = os_mem_next_node(tmp_node);
        }
    }
    mem_unlock(int_save);

    blk_nums
}

#[unsafe(export_name = "LOS_MemTaskIdGet")]
pub fn los_mem_task_id_get(ptr: *const core::ffi::c_void) -> u32 {
    let pool_info;
    unsafe {
        pool_info = m_aucSysMem1 as *mut LosMemPoolInfo;
    }

    if ptr.is_null()
        || ptr < os_mem_first_node(pool_info) as *const core::ffi::c_void
        || ptr > os_mem_end_node(pool_info) as *const core::ffi::c_void
    {
        // unsafe {
        //     dprintf(
        //         b"input ptr %p is out of system memory range[%p, %p]\n\0" as *const u8,
        //         ptr,
        //         os_mem_first_node(pool_info),
        //         os_mem_end_node(pool_info),
        //     );
        // }
        return OS_INVALID;
    }

    let mut int_save: u32 = 0;

    mem_lock(&mut int_save);
    unsafe {
        let mut tmp_node = os_mem_first_node(pool_info);
        while tmp_node <= os_mem_end_node(pool_info) {
            if (ptr as usize) < (tmp_node as usize) {
                if os_mem_node_get_used_flag(
                    (*(*tmp_node).self_node.pre_node).self_node.size_and_flag,
                ) {
                    mem_unlock(int_save);
                    return (*(*tmp_node).self_node.pre_node)
                        .self_node
                        .node_info
                        .used_node_info
                        .task_id as u32;
                } else {
                    mem_unlock(int_save);
                    // dprintf(
                    //     b"input ptr %p is belong to a free mem node\n\0" as *const u8,
                    //     ptr,
                    // );
                    return OS_INVALID;
                }
            }
            tmp_node = os_mem_next_node(tmp_node);
        }
    }
    mem_unlock(int_save);
    OS_INVALID
}

#[unsafe(export_name = "LOS_MemFreeBlksGet")]
pub fn los_mem_free_blks_get(pool: *mut LosMemPoolInfo) -> u32 {
    if pool.is_null() {
        return NOK;
    }

    let mut blk_nums: u32 = 0;
    let mut int_save: u32 = 0;

    mem_lock(&mut int_save);
    unsafe {
        let mut tmp_node = os_mem_first_node(pool);
        while tmp_node <= os_mem_end_node(pool) {
            if !os_mem_node_get_used_flag((*tmp_node).self_node.size_and_flag) {
                blk_nums += 1;
            }
            tmp_node = os_mem_next_node(tmp_node);
        }
    }
    mem_unlock(int_save);

    blk_nums
}

#[unsafe(export_name = "LOS_MemLastUsedGet")]
pub fn los_mem_last_used_get(pool: *mut LosMemPoolInfo) -> usize {
    if pool.is_null() {
        return NOK as usize;
    }

    unsafe {
        let node = (*os_mem_end_node(pool)).self_node.pre_node;
        if os_mem_node_get_used_flag((*node).self_node.size_and_flag) {
            return node as usize
                + os_mem_node_get_size((*node).self_node.size_and_flag) as usize
                + core::mem::size_of::<LosMemDynNode>();
        } else {
            return node as usize + core::mem::size_of::<LosMemDynNode>();
        }
    }
}

#[unsafe(export_name = "OsMemResetEndNode")]
pub fn os_mem_reset_end_node(pool: *mut LosMemPoolInfo, pre_addr: usize) {
    unsafe {
        // 获取内存池的结束节点
        let end_node = os_mem_end_node(pool);

        // 设置结束节点的大小和标志
        (*end_node).self_node.size_and_flag = OS_MEM_NODE_HEAD_SIZE as u32;

        // 如果提供了前一个节点的地址，则设置结束节点的前置节点
        if pre_addr != 0 {
            (*end_node).self_node.pre_node =
                (pre_addr - OS_MEM_NODE_HEAD_SIZE) as *mut LosMemDynNode;
        }

        // 设置结束节点的已使用标志
        os_mem_node_set_used_flag(&mut (*end_node).self_node.size_and_flag);

        // 设置结束节点的魔数和任务 ID
        os_mem_set_magic_num_and_task_id(end_node);
    }
}

#[unsafe(export_name = "LOS_MemPoolSizeGet")]
pub fn los_mem_pool_size_get(pool: *const LosMemPoolInfo) -> u32 {
    if pool.is_null() {
        return NOK;
    }
    unsafe { (*pool).pool_size }
}
#[unsafe(export_name = "LOS_MemInfoGet")]
pub fn los_mem_info_get(pool: *mut LosMemPoolInfo, pool_status: *mut LosMemPoolStatus) -> u32 {
    if pool_status.is_null() {
        // unsafe {
        //     dprintf(b"can't use NULL addr to save info\n\0" as *const u8);
        // }
        return NOK;
    }
    let pool_status = unsafe { &mut *pool_status };
    if pool.is_null() || (pool as usize) != unsafe { (*pool).pool as usize } {
        // unsafe {
        //     dprintf(
        //         b"wrong mem pool addr: %p, line:%d\n\0" as *const u8,
        //         pool,
        //         line!(),
        //     );
        // }
        return NOK;
    }
    let mut int_save: u32 = 0;
    mem_lock(&mut int_save);
    let ret = os_mem_info_get(pool, pool_status);
    mem_unlock(int_save);
    ret
}

fn os_show_free_node(index: usize, length: usize, count_num: &[u32]) {
    let _ = index;
    let _ = length;
    let _ = count_num;
    let mut count = 0;

    // 打印块大小
    // unsafe {
    //     dprintf(b"\n    block size:  \0" as *const u8);
    // }
    while count < length {
        // unsafe {
        //     dprintf(
        //         b"2^%-5u \0" as *const u8,
        //         index + OS_MIN_MULTI_DLNK_LOG2 as usize + count,
        //     );
        // }
        count += 1;
    }

    // // 打印节点数量
    // unsafe {
    //     dprintf(b"\n    node number: \0" as *const u8);
    // }
    count = 0;
    while count < length {
        // unsafe {
        //     dprintf(b"%-5u \0" as *const u8, count_num[count + index]);
        // }
        count += 1;
    }
}

#[unsafe(export_name = "LOS_MemFreeNodeShow")]
pub fn los_mem_free_node_show(pool: *mut LosMemPoolInfo) -> u32 {
    if pool.is_null() || (pool as usize) != unsafe { (*pool).pool as usize } {
        // unsafe {
        //     dprintf(
        //         b"wrong mem pool addr: %p, line:%d\n\0" as *const u8,
        //         pool,
        //         line!(),
        //     );
        // }
        return NOK;
    }

    let mut count_num = [0; OS_MULTI_DLNK_NUM];
    let mut int_save: u32 = 0;

    // unsafe {
    //     dprintf(
    //         b"\n   ************************ left free node number**********************\n\0"
    //             as *const u8,
    //     );
    // }
    mem_lock(&mut int_save);

    let head_addr =
        (pool as usize + core::mem::size_of::<LosMemPoolInfo>()) as *mut LosMultipleDlinkHead;
    unsafe {
        for link_head_index in 0..OS_MULTI_DLNK_NUM {
            let mut list_node_head = (*head_addr).list_head[link_head_index].next;
            while list_node_head != &mut (*head_addr).list_head[link_head_index] {
                list_node_head = (*list_node_head).next;
                count_num[link_head_index] += 1;
            }
        }
    }

    mem_unlock(int_save);

    const COLUMN_NUM: usize = 8;
    for link_head_index in (0..OS_MULTI_DLNK_NUM).step_by(COLUMN_NUM) {
        let length = if link_head_index + COLUMN_NUM < OS_MULTI_DLNK_NUM {
            COLUMN_NUM
        } else {
            OS_MULTI_DLNK_NUM - link_head_index
        };
        os_show_free_node(link_head_index, length, &count_num);
    }
    // unsafe {
    // dprintf(
    //     b"\n   ********************************************************************\n\n\0"
    //         as *const u8,
    // );
    // }
    OK
}

unsafe extern "C" {
    #[link_name = "OsMemSetMagicNumAndTaskID"]
    unsafe fn os_mem_set_magic_num_and_task_id(node: *mut LosMemDynNode);
}
