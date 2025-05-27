#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub struct LinkedList {
    pub prev: *mut LinkedList,
    pub next: *mut LinkedList,
}

impl LinkedList {
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
    pub fn remove(node: *mut LinkedList) {
        unsafe {
            // 检查node指针是否4字节对齐
            assert_eq!(node as usize & 0x3, 0, "节点指针未对齐: {:p}", node);
            // 检查next和prev指针是否4字节对齐
            assert_eq!(
                (*node).next as usize & 0x3,
                0,
                "next指针未对齐: {:p} {:p} {:p}",
                node,
                (*node).prev,
                (*node).next,
            );
            assert_eq!(
                (*node).prev as usize & 0x3,
                0,
                "prev指针未对齐: {:p} {:p} {:p}",
                node,
                (*node).prev,
                (*node).next,
            );
            // 确保next和prev不为空
            assert!(!(*node).next.is_null(), "next指针为空");
            assert!(!(*node).prev.is_null(), "prev指针为空");
            (*(*node).next).prev = (*node).prev;
            (*(*node).prev).next = (*node).next;
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
