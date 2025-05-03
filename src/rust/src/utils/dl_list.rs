#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct DlList {
    pub prev: *mut DlList,
    pub next: *mut DlList,
}

impl DlList {
    pub fn init(&mut self) {
        self.prev = self as *mut DlList;
        self.next = self as *mut DlList;
    }

    pub fn delete(&mut self) {
        unsafe {
            (*self.next).prev = self.prev;
            (*self.prev).next = self.next;
            self.next = core::ptr::null_mut();
            self.prev = core::ptr::null_mut();
        }
    }

    /// 在双向链表中添加节点
    pub fn add(&mut self, node: &mut DlList) {
        unsafe {
            node.next = self.next;
            node.prev = self as *mut DlList;
            (*self.next).prev = node;
            self.next = node;
        }
    }
}
