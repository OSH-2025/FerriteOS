use crate::utils::list::LinkedList;

pub const OS_MAX_MULTI_DLNK_LOG2: u32 = 29;
pub const OS_MIN_MULTI_DLNK_LOG2: u32 = 4;
pub const OS_MULTI_DLNK_NUM: usize = (OS_MAX_MULTI_DLNK_LOG2 - OS_MIN_MULTI_DLNK_LOG2 + 1) as usize;
pub const OS_MULTI_DLNK_HEAD_SIZE: usize = core::mem::size_of::<LosMultipleDlinkHead>();
pub const OS_DLNK_HEAD_SIZE: usize = OS_MULTI_DLNK_HEAD_SIZE;

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
            list_node_head.init();
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
}

#[inline]
fn os_log2(size: u32) -> u32 {
    size.checked_ilog2().unwrap_or(0)
}

// #[unsafe(export_name = "OsDLnkInitMultiHead")]
pub fn os_dlnk_init_multi_head(head_addr: *mut ()) {
    let dlink_head = head_addr as *mut LosMultipleDlinkHead;
    unsafe { (*dlink_head).init() };
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
