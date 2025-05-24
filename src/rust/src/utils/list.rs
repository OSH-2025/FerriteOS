#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct LinkedList {
    pub prev: *mut LinkedList,
    pub next: *mut LinkedList,
}

impl LinkedList {
    pub const UNINIT: Self = Self {
        prev: core::ptr::null_mut(),
        next: core::ptr::null_mut(),
    };

    pub fn init(list: *mut LinkedList) {
        unsafe {
            (*list).prev = list;
            (*list).next = list;
        }
    }

    #[inline]
    pub fn tail_insert(list: *mut LinkedList, node: *mut LinkedList) {
        unsafe {
            LinkedList::insert((*list).prev, node);
        }
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
    pub fn remove(node: *mut LinkedList) {
        unsafe {
            // 更新相邻节点的指针，将当前节点从链表中移除
            (*(*node).next).prev = (*node).prev;
            (*(*node).prev).next = (*node).next;
            // 将节点指针设为 NULL，表示节点不再属于任何链表
            (*node).next = core::ptr::null_mut();
            (*node).prev = core::ptr::null_mut();
        }
    }

    #[inline]
    pub fn is_empty(list: *mut LinkedList) -> bool {
        unsafe { (*list).next == list }
    }
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
        let offset = offset_of!($type, $($field).+);
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
                    let $item: *mut $type = container_of!(current_node_ptr__, $type, $($field).+);
                    $code
                    current_node_ptr__ = (*current_node_ptr__).next;
                }
            }
        }
    };
}
