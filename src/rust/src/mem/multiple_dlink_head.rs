use super::defs::*;
use crate::utils::list::LinkedList;
use crate::utils::printf::dprintf;

/// 多级双向链表头结构
#[repr(C)]
pub struct LosMultipleDlinkHead {
    pub list_head: [LinkedList; OS_MULTI_DLNK_NUM],
}

impl LosMultipleDlinkHead {
    /// 初始化多级双向链表头
    #[inline]
    fn init(&mut self) {
        for list_node_head in self.list_head.iter_mut() {
            LinkedList::init(list_node_head);
        }
    }

    /// 根据内存块大小获取对应的链表头节点
    #[inline]
    pub fn get_list_head_by_size(&self, size: u32) -> *const LinkedList {
        let index = os_log2(size);
        if index > OS_MAX_MULTI_DLNK_LOG2 {
            core::ptr::null_mut()
        } else {
            let index = u32::max(index, OS_MIN_MULTI_DLNK_LOG2);
            &self.list_head[(index - OS_MIN_MULTI_DLNK_LOG2) as usize] as *const LinkedList
        }
    }

    pub fn print(head_addr: *mut LosMultipleDlinkHead) {
        let head_addr = unsafe { &*head_addr };
        for (i, list_node_head) in head_addr.list_head.iter().enumerate() {
            unsafe {
                dprintf(
                    b"list_head[%d]: %p, prev: %p, next: %p\0\n" as *const u8,
                    i,
                    list_node_head,
                    list_node_head.prev,
                    list_node_head.next,
                )
            };
        }
    }
}

#[inline]
fn os_log2(size: u32) -> u32 {
    size.checked_ilog2().unwrap_or(0)
}

pub fn os_dlnk_init_multi_head(head_addr: *mut LosMultipleDlinkHead) {
    unsafe { (*head_addr).init() };
}

pub fn os_dlnk_next_multi_head(
    head_addr: *mut LosMultipleDlinkHead,
    list_node_head: *mut LinkedList,
) -> *mut LinkedList {
    unsafe {
        if list_node_head
            == &(*head_addr).list_head[OS_MULTI_DLNK_NUM - 1] as *const LinkedList
                as *mut LinkedList
        {
            core::ptr::null_mut()
        } else {
            list_node_head.add(1)
        }
    }
}
