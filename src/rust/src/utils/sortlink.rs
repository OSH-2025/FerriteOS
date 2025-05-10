use super::list::LinkedList;
use crate::{
    LOS_NOK, LOS_OK,
    mem::{defs::m_aucSysMem0, memory::los_mem_alloc},
};

/// 无效值常量
pub const OS_INVALID_VALUE: u32 = 0xFFFFFFFF;

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
        (self.idx_roll_num & OS_TSK_HIGH_BITS_MASK) >> OS_TSK_LOW_BITS
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
