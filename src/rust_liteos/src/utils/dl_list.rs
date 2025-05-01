#[repr(C)]
pub struct DlList {
    pub prev: *mut DlList,
    pub next: *mut DlList,
}

impl DlList {
    pub fn init(&mut self) {
        self.prev = self as *mut DlList;
        self.next = self as *mut DlList;
    }
}
