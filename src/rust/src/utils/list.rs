#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub struct LinkedList {
    pub prev: *mut LinkedList,
    pub next: *mut LinkedList,
}

unsafe impl Send for LinkedList {}
unsafe impl Sync for LinkedList {}

impl LinkedList {
    pub const fn new() -> Self {
        Self {
            prev: core::ptr::null_mut(),
            next: core::ptr::null_mut(),
        }
    }

    pub const UNINIT: Self = Self {
        prev: core::ptr::null_mut(),
        next: core::ptr::null_mut(),
    };

    #[inline]
    pub fn init(list: *mut LinkedList) {
        unsafe {
            (*list).prev = list;
            (*list).next = list;
        }
    }

    #[inline]
    #[allow(dead_code)]
    pub fn init_ref(&mut self) {
        self.prev = self as *mut LinkedList;
        self.next = self as *mut LinkedList;
    }

    #[inline]
    pub fn insert(list: *mut LinkedList, node: *mut LinkedList) {
        unsafe {
            (*node).next = (*list).next;
            (*node).prev = list;
            (*(*list).next).prev = node;
            (*list).next = node;
        }
    }

    #[inline]
    pub fn head_insert(list: *mut LinkedList, node: *mut LinkedList) {
        LinkedList::insert(list, node);
    }

    #[inline]
    pub fn tail_insert(list: *mut LinkedList, node: *mut LinkedList) {
        unsafe {
            LinkedList::insert((*list).prev, node);
        }
    }

    #[inline]
    #[allow(dead_code)]
    pub fn tail_insert_ref(&mut self, node: *mut LinkedList) {
        LinkedList::insert(self.prev, node);
    }

    #[inline]
    pub fn remove(node: *mut LinkedList) {
        unsafe {
            (*(*node).next).prev = (*node).prev;
            (*(*node).prev).next = (*node).next;
            (*node).next = core::ptr::null_mut();
            (*node).prev = core::ptr::null_mut();
        }
    }

    #[inline]
    pub fn is_empty(list: *const LinkedList) -> bool {
        unsafe { (*list).next as *const LinkedList == list }
    }

    #[inline]
    #[allow(dead_code)]
    pub fn is_empty_ref(&self) -> bool {
        self.next as *const LinkedList == self
    }

    #[inline]
    pub fn first(list: *const LinkedList) -> *mut LinkedList {
        unsafe { (*list).next }
    }

    #[inline]
    #[allow(dead_code)]
    pub fn first_ref(&self) -> *mut LinkedList {
        self.next
    }

    #[inline]
    pub fn last(list: *const LinkedList) -> *mut LinkedList {
        unsafe { (*list).prev }
    }

    // #[inline]
    // pub fn remove_first(list: *const LinkedList) -> Option<*mut LinkedList> {
    //     if LinkedList::is_empty(list) {
    //         None
    //     } else {
    //         let first_node = LinkedList::first(list);
    //         LinkedList::remove(first_node);
    //         Some(first_node)
    //     }
    // }
}

#[macro_export]
macro_rules! offset_of {
    ($type:ty, $($field:ident).+) => {{
        let uninit = core::mem::MaybeUninit::<$type>::uninit();
        let base = uninit.as_ptr();
        #[allow(unused_unsafe)]
        unsafe { core::ptr::addr_of!((*base).$($field).*) as usize - base as usize }
    }};
}

#[macro_export]
macro_rules! container_of {
    ($ptr:expr, $type:ty, $($field:ident).+) => {{
        let offset = crate::offset_of!($type, $($field).+);
        ($ptr as usize - offset) as *mut $type
    }};
}

#[macro_export]
macro_rules! list_for_each_entry {
    ($item:ident, $list:expr, $type:ty, $($field:ident).+, $code:block) => {
        unsafe {
            let list_head__ = $list;
            if !list_head__.is_null() {
                let mut current_node_ptr__ = (*list_head__).next;
                while current_node_ptr__ != list_head__ {
                    let $item: *mut $type = crate::container_of!(current_node_ptr__, $type, $($field).+);
                    $code
                    current_node_ptr__ = (*current_node_ptr__).next;
                }
            }
        }
    };
}
