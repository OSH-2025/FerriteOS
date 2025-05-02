use crate::utils::dl_list::DlList;

pub const OS_MAX_MULTI_DLNK_LOG2: u32 = 29;
pub const OS_MIN_MULTI_DLNK_LOG2: u32 = 4;
pub const OS_MULTI_DLNK_NUM: usize = (OS_MAX_MULTI_DLNK_LOG2 - OS_MIN_MULTI_DLNK_LOG2 + 1) as usize;
pub const OS_MULTI_DLNK_HEAD_SIZE: usize = core::mem::size_of::<LosMultipleDlinkHead>();
pub const OS_DLNK_HEAD_SIZE: usize = OS_MULTI_DLNK_HEAD_SIZE;

/// 多级双向链表头结构
#[repr(C)]
pub struct LosMultipleDlinkHead {
    pub list_head: [DlList; OS_MULTI_DLNK_NUM],
}

impl LosMultipleDlinkHead {
    /// 初始化多级双向链表头
    fn init(&mut self) {
        for list_node_head in self.list_head.iter_mut() {
            list_node_head.init();
        }
    }

    /// 根据内存块大小获取对应的链表头节点
    pub fn get_multi_head(&self, size: u32) -> Option<&DlList> {
        let index = os_log2(size);
        if index > OS_MAX_MULTI_DLNK_LOG2 {
            None
        } else {
            let index = if index <= OS_MIN_MULTI_DLNK_LOG2 {
                OS_MIN_MULTI_DLNK_LOG2
            } else {
                index
            };
            Some(&self.list_head[(index - OS_MIN_MULTI_DLNK_LOG2) as usize])
        }
    }
}

#[inline]
fn os_log2(size: u32) -> u32 {
    size.checked_ilog2().unwrap_or(0)
}

#[unsafe(export_name = "OsDLnkInitMultiHead")]
pub fn os_dlnk_init_multi_head(head_addr: *mut ()) {
    let dlink_head = head_addr as *mut LosMultipleDlinkHead;
    unsafe { (*dlink_head).init() };
}

#[unsafe(export_name = "OsDLnkMultiHead")]
pub fn os_dlnk_multi_head(head_addr: *mut (), size: u32) -> *mut DlList {
    let dlink_head = head_addr as *mut LosMultipleDlinkHead;
    unsafe {
        // 使用 get_multi_head 方法获取链表头节点
        match (*dlink_head).get_multi_head(size) {
            Some(list_head) => list_head as *const DlList as *mut DlList,
            None => core::ptr::null_mut(),
        }
    }
}
