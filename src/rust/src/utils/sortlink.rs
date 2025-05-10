use super::list::LinkedList;
use crate::{
    LOS_NOK, LOS_OK, container_of,
    mem::{defs::m_aucSysMem0, memory::los_mem_alloc},
    offset_of,
};

pub const OS_TSK_HIGH_BITS: u32 = 3;
pub const OS_TSK_LOW_BITS: u32 = 32 - OS_TSK_HIGH_BITS;
pub const OS_TSK_SORTLINK_LOGLEN: u32 = OS_TSK_HIGH_BITS;
pub const OS_TSK_SORTLINK_LEN: u32 = 1 << OS_TSK_SORTLINK_LOGLEN;
pub const OS_TSK_SORTLINK_MASK: u32 = OS_TSK_SORTLINK_LEN - 1;
pub const OS_TSK_MAX_ROLLNUM: u32 = 0xFFFFFFFF - OS_TSK_SORTLINK_LEN;
pub const OS_TSK_HIGH_BITS_MASK: u32 = OS_TSK_SORTLINK_MASK << OS_TSK_LOW_BITS;
pub const OS_TSK_LOW_BITS_MASK: u32 = !OS_TSK_HIGH_BITS_MASK;

#[repr(C)]
pub struct SortLinkList {
    /// 链表节点
    pub sort_link_node: LinkedList,
    /// 索引和轮数
    pub idx_roll_num: u32,
}

impl SortLinkList {
    /// 设置轮数（低位部分）
    #[inline]
    fn set_roll_num(&mut self, value: u32) {
        self.idx_roll_num = (self.idx_roll_num & OS_TSK_HIGH_BITS_MASK) | value;
    }

    /// 设置排序索引（高位部分）
    #[inline]
    fn set_sort_index(&mut self, value: u32) {
        self.idx_roll_num = (self.idx_roll_num & OS_TSK_LOW_BITS_MASK) | (value << OS_TSK_LOW_BITS);
    }

    /// 获取轮数（低位部分）
    #[inline]
    fn get_roll_num(&self) -> u32 {
        self.idx_roll_num & OS_TSK_LOW_BITS_MASK
    }

    /// 获取排序索引（高位部分）
    #[inline]
    fn get_sort_index(&self) -> u32 {
        self.idx_roll_num >> OS_TSK_LOW_BITS
    }

    /// 从当前节点的轮数中减去指定的值，保留索引部分不变
    #[inline]
    fn roll_num_sub_value(&mut self, value: u32) {
        let self_roll_num = self.get_roll_num();
        self.set_roll_num(self_roll_num - value);
    }

    /// 将指定值添加到当前节点的轮数中，保留索引部分不变
    #[inline]
    fn roll_num_add_value(&mut self, value: u32) {
        let self_roll_num = self.get_roll_num();
        self.set_roll_num(self_roll_num + value);
    }
}

/// 排序链表属性
#[repr(C)]
pub struct SortLinkAttribute {
    /// 排序链表头
    pub sort_link: *mut LinkedList,
    /// 游标
    pub cursor: u16,
    _reserved: u16,
}

#[unsafe(export_name = "OsSortLinkInit")]
pub extern "C" fn os_sort_link_init(sort_link_header: &mut SortLinkAttribute) -> u32 {
    // 计算需要分配的内存大小
    let size = (size_of::<LinkedList>() as u32) << OS_TSK_SORTLINK_LOGLEN;

    // 分配内存
    let list_object =
        los_mem_alloc(unsafe { m_aucSysMem0 } as *mut core::ffi::c_void, size) as *mut LinkedList;
    if list_object.is_null() {
        return LOS_NOK;
    }

    // 设置排序链表头
    sort_link_header.sort_link = list_object;
    sort_link_header.cursor = 0;

    // 初始化每个链表
    unsafe {
        for index in 0..OS_TSK_SORTLINK_LEN {
            let list = list_object.add(index as usize);
            LinkedList::init(list);
        }
    }

    LOS_OK
}

/// 将排序节点添加到排序链表中
#[unsafe(export_name = "OsAdd2SortLink")]
pub extern "C" fn os_add_to_sort_link(
    sort_link_header: &SortLinkAttribute,
    sort_list: &mut SortLinkList,
) {
    // 限制 idxRollNum 的最大值，防止进位影响高位的索引计算
    if sort_list.idx_roll_num > OS_TSK_MAX_ROLLNUM {
        sort_list.idx_roll_num = OS_TSK_MAX_ROLLNUM;
    }

    // 计算超时值和排序索引
    let timeout = sort_list.idx_roll_num;
    let mut sort_index = timeout & OS_TSK_SORTLINK_MASK;
    let mut roll_num = (timeout >> OS_TSK_SORTLINK_LOGLEN) + 1;

    // 特殊情况：当索引为0时，轮数需要减1
    if sort_index == 0 {
        roll_num -= 1;
    }

    // 设置轮数部分(低位)
    sort_list.set_roll_num(roll_num);

    // 调整排序索引，加上当前游标位置并确保在有效范围内
    sort_index = (sort_index + sort_link_header.cursor as u32) & OS_TSK_SORTLINK_MASK;

    // 设置排序索引部分(高位)
    sort_list.set_sort_index(sort_index);

    unsafe {
        // 获取对应桶的链表头
        let list_object = sort_link_header.sort_link.add(sort_index as usize);

        // 如果链表为空，直接插入
        if LinkedList::is_empty(list_object) {
            LinkedList::tail_insert(list_object, &mut sort_list.sort_link_node);
        } else {
            // 获取第一个节点并开始查找合适的插入位置
            let mut current_list = container_of!((*list_object).next, SortLinkList, sort_link_node);

            loop {
                // 获取当前节点和新节点的轮数值
                let current_roll_num = (*current_list).get_roll_num();
                let sort_list_roll_num = sort_list.get_roll_num();

                if current_roll_num <= sort_list_roll_num {
                    // 当前节点轮数小于等于新节点轮数
                    // 新节点轮数减去当前节点轮数，表示相对时间差
                    sort_list.roll_num_sub_value(current_roll_num);
                } else {
                    // 当前节点轮数大于新节点轮数
                    // 当前节点轮数减去新节点轮数，准备在当前节点前插入
                    (*current_list).roll_num_sub_value(sort_list_roll_num);
                    break;
                }
                // 移动到下一个节点继续比较
                current_list = container_of!(
                    (*current_list).sort_link_node.next,
                    SortLinkList,
                    sort_link_node
                );

                // 如果已经到达链表末尾，结束查找
                if &mut (*current_list).sort_link_node as *mut LinkedList == list_object {
                    break;
                }
            }

            // 在找到的位置插入新节点
            LinkedList::tail_insert(
                &mut (*current_list).sort_link_node,
                &mut sort_list.sort_link_node,
            );
        }
    }
}

#[inline]
fn os_check_sort_link(list_head: *mut LinkedList, list_node: *mut LinkedList) {
    unsafe {
        let mut tmp = (*list_node).prev;
        while tmp != list_node {
            if tmp == list_head {
                return;
            }
            tmp = (*tmp).prev;
        }
    }
    // TODO OsBackTrace
}

#[unsafe(export_name = "OsDeleteSortLink")]
pub extern "C" fn os_delete_sort_link(
    sort_link_header: &SortLinkAttribute,
    sort_list: &mut SortLinkList,
) {
    // 获取排序索引
    let sort_index = sort_list.get_sort_index();

    unsafe {
        // 获取对应的链表对象
        let list_object = sort_link_header.sort_link.add(sort_index as usize);

        // 检查节点是否在正确的链表中
        os_check_sort_link(list_object, &mut sort_list.sort_link_node);

        // 如果不是链表的最后一个节点，将轮数加到下一个节点上
        if sort_list.sort_link_node.next != list_object {
            let next_sort_list =
                container_of!(sort_list.sort_link_node.next, SortLinkList, sort_link_node);

            // 将当前节点的轮数添加到下一个节点
            (*next_sort_list).roll_num_add_value(sort_list.get_roll_num());
        }

        // 从链表中删除节点
        LinkedList::remove(&mut sort_list.sort_link_node);
    }
}

#[inline]
fn os_calc_expire_time(roll_num: u32, sort_index: u32, cur_sort_index: u16) -> u32 {
    let mut sort_index = sort_index;

    // 计算 sort_index 和 cur_sort_index 之间的距离，考虑循环特性
    if sort_index > cur_sort_index as u32 {
        sort_index = sort_index - cur_sort_index as u32;
    } else {
        sort_index = OS_TSK_SORTLINK_LEN - cur_sort_index as u32 + sort_index;
    }

    // 计算过期时间
    ((roll_num - 1) << OS_TSK_SORTLINK_LOGLEN) + sort_index
}

#[unsafe(export_name = "OsSortLinkGetNextExpireTime")]
pub extern "C" fn os_sort_link_get_next_expire_time(sort_link_header: &SortLinkAttribute) -> u32 {
    let mut min_sort_index = u32::MAX;
    let mut min_roll_num = OS_TSK_LOW_BITS_MASK;

    // 计算新的游标位置（当前游标+1，并考虑环形特性）
    let cursor = (sort_link_header.cursor + 1) & (OS_TSK_SORTLINK_MASK as u16);

    // 遍历所有桶
    for i in 0..OS_TSK_SORTLINK_LEN {
        unsafe {
            // 获取对应桶的链表头
            let list_object = sort_link_header
                .sort_link
                .add(((cursor as u32 + i) & OS_TSK_SORTLINK_MASK) as usize);

            // 检查链表是否为空
            if !LinkedList::is_empty(list_object) {
                // 获取链表的第一个节点
                let list_sorted = container_of!((*list_object).next, SortLinkList, sort_link_node);

                // 获取节点的轮数
                let roll_num = (*list_sorted).get_roll_num();

                // 更新最小轮数和对应的排序索引
                if min_roll_num > roll_num {
                    min_roll_num = roll_num;
                    min_sort_index = (cursor as u32 + i) & OS_TSK_SORTLINK_MASK;
                }
            }
        }
    }

    // 如果找到有效的最小轮数，计算过期时间
    if min_roll_num != OS_TSK_LOW_BITS_MASK {
        os_calc_expire_time(min_roll_num, min_sort_index, sort_link_header.cursor)
    } else {
        // 如果没有找到有效的轮数，返回最大值
        u32::MAX
    }
}

/// 更新排序链表中所有节点的到期时间
///
/// 当系统休眠或跳过一段时间后，需要调整所有定时器的到期时间
#[unsafe(export_name = "OsSortLinkUpdateExpireTime")]
pub extern "C" fn os_sort_link_update_expire_time(
    sleep_ticks: u32,
    sort_link_header: &mut SortLinkAttribute,
) {
    // 如果跳过的时钟周期为0，直接返回
    if sleep_ticks == 0 {
        return;
    }

    // 计算排序索引和轮数
    let sort_index = sleep_ticks & OS_TSK_SORTLINK_MASK;
    let mut roll_num = (sleep_ticks >> OS_TSK_SORTLINK_LOGLEN) + 1;
    let mut sort_idx = sort_index;

    // 特殊情况处理：索引为0时
    if sort_index == 0 {
        roll_num -= 1;
        sort_idx = OS_TSK_SORTLINK_LEN;
    }

    // 遍历所有排序桶
    for i in 0..OS_TSK_SORTLINK_LEN {
        unsafe {
            // 获取当前桶的链表头
            let list_object = sort_link_header
                .sort_link
                .add(((sort_link_header.cursor as u32 + i) & OS_TSK_SORTLINK_MASK) as usize);

            // 检查链表是否为空
            if !LinkedList::is_empty(list_object) {
                // 获取第一个节点
                let sort_list = container_of!((*list_object).next, SortLinkList, sort_link_node);

                // 减少轮数，减去(roll_num - 1)
                (*sort_list).roll_num_sub_value(roll_num - 1);

                // 对于特定范围内的桶，额外减少1个轮数
                if (i > 0) && (i < sort_idx) {
                    (*sort_list).roll_num_sub_value(1);
                }
            }
        }
    }

    // 更新游标位置
    sort_link_header.cursor =
        ((sort_link_header.cursor as u32 + sleep_ticks - 1) % OS_TSK_SORTLINK_LEN) as u16;
}

/// 获取目标排序链表节点的到期时间
///
/// 计算从链表头到目标节点的累积轮数，然后转换为过期时间
#[unsafe(export_name = "OsSortLinkGetTargetExpireTime")]
pub extern "C" fn os_sort_link_get_target_expire_time(
    sort_link_header: &SortLinkAttribute,
    target_sort_list: &SortLinkList,
) -> u32 {
    // 获取目标节点的排序索引和初始轮数
    let sort_index = target_sort_list.get_sort_index();
    let mut roll_num = target_sort_list.get_roll_num();

    unsafe {
        // 获取对应桶的链表头
        let list_object = sort_link_header.sort_link.add(sort_index as usize);

        // 从链表的第一个节点开始
        let mut list_sorted = container_of!((*list_object).next, SortLinkList, sort_link_node);

        // 累加轮数直到找到目标节点
        while list_sorted != target_sort_list as *const _ as *mut _ {
            // 累加当前节点的轮数
            roll_num += (*list_sorted).get_roll_num();

            // 移动到下一个节点
            list_sorted = container_of!(
                (*list_sorted).sort_link_node.next,
                SortLinkList,
                sort_link_node
            );
        }

        // 计算并返回最终的到期时间
        os_calc_expire_time(roll_num, sort_index, sort_link_header.cursor)
    }
}
