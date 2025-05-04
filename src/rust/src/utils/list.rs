#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct LinkedList {
    pub prev: *mut LinkedList,
    pub next: *mut LinkedList,
}

impl LinkedList {
    pub fn init(&mut self) {
        self.prev = self as *mut LinkedList;
        self.next = self as *mut LinkedList;
    }

    // LOS_ListTailInsert(LOS_DL_LIST *list, LOS_DL_LIST *node)
    // {
    // LOS_ListAdd(list->pstPrev, node);
    // }
    pub fn add(list: *mut LinkedList, node: *mut LinkedList) {
        unsafe {
            (*node).next = (*list).next;
            (*node).prev = list;
            (*(*list).next).prev = node;
            (*list).next = node;
        }
    }

    #[inline]
    pub fn tail_insert(list: *mut LinkedList, node: *mut LinkedList) {
        unsafe {
            LinkedList::add((*list).prev, node);
        }
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
