use super::list::LinkedList;

/// 无效值常量
pub const OS_INVALID_VALUE: u32 = 0xFFFFFFFF;

// #[cfg(feature = "LOSCFG_BASE_CORE_USE_SINGLE_LIST")]
// mod config {
//     pub const OS_TSK_SORTLINK_LOGLEN: u32 = 0;
//     pub const OS_TSK_SORTLINK_LEN: u32 = 1;
//     pub const OS_TSK_MAX_ROLLNUM: u32 = 0xFFFFFFFE;
//     pub const OS_TSK_LOW_BITS_MASK: u32 = 0xFFFFFFFF;
// }

#[cfg(feature = "LOSCFG_BASE_CORE_USE_MULTI_LIST")]
mod config {
    pub const OS_TSK_HIGH_BITS: u32 = 3;
    pub const OS_TSK_LOW_BITS: u32 = 32 - OS_TSK_HIGH_BITS;
    pub const OS_TSK_SORTLINK_LOGLEN: u32 = OS_TSK_HIGH_BITS;
    pub const OS_TSK_SORTLINK_LEN: u32 = 1 << OS_TSK_SORTLINK_LOGLEN;
    pub const OS_TSK_SORTLINK_MASK: u32 = OS_TSK_SORTLINK_LEN - 1;
    pub const OS_TSK_MAX_ROLLNUM: u32 = 0xFFFFFFFF - OS_TSK_SORTLINK_LEN;
    pub const OS_TSK_HIGH_BITS_MASK: u32 = OS_TSK_SORTLINK_MASK << OS_TSK_LOW_BITS;
    pub const OS_TSK_LOW_BITS_MASK: u32 = !OS_TSK_HIGH_BITS_MASK;
}

// use config::*;

#[repr(C)]
pub struct SortLinkList {
    /// 链表节点
    pub sort_link_node: LinkedList,
    /// 索引和轮数
    pub idx_roll_num: u32,
}

// impl SortLinkList {
//     /// 创建新的排序链表节点
//     pub fn new() -> Self {
//         Self {
//             sort_link_node: DLListNode::new(),
//             idx_roll_num: 0,
//         }
//     }

//     /// 获取轮数
//     #[cfg(feature = "use_single_list")]
//     pub fn roll_num(&self) -> u32 {
//         self.idx_roll_num
//     }

//     #[cfg(not(feature = "use_single_list"))]
//     pub fn roll_num(&self) -> u32 {
//         self.idx_roll_num & OS_TSK_LOW_BITS_MASK
//     }

//     /// 设置轮数
//     #[cfg(feature = "use_single_list")]
//     pub fn set_roll_num(&mut self, value: u32) {
//         self.idx_roll_num = value;
//     }

//     #[cfg(not(feature = "use_single_list"))]
//     pub fn set_roll_num(&mut self, value: u32) {
//         self.idx_roll_num = (self.idx_roll_num & OS_TSK_HIGH_BITS_MASK) | value;
//     }

//     /// 减少轮数
//     pub fn roll_num_sub(&mut self, other: u32) {
//         let roll = self.roll_num();
//         self.set_roll_num(roll - other);
//     }

//     /// 增加轮数
//     pub fn roll_num_add(&mut self, other: u32) {
//         let roll = self.roll_num();
//         self.set_roll_num(roll + other);
//     }

//     /// 轮数减一
//     pub fn roll_num_dec(&mut self) {
//         let roll = self.roll_num();
//         self.set_roll_num(roll - 1);
//     }

//     /// 获取排序索引
//     #[cfg(not(feature = "use_single_list"))]
//     pub fn sort_index(&self) -> u32 {
//         self.idx_roll_num >> OS_TSK_LOW_BITS
//     }

//     /// 设置排序索引
//     #[cfg(not(feature = "use_single_list"))]
//     pub fn set_sort_index(&mut self, value: u32) {
//         self.idx_roll_num = (self.idx_roll_num & OS_TSK_LOW_BITS_MASK) | (value << OS_TSK_LOW_BITS);
//     }
// }

/// 排序链表属性
#[repr(C)]
pub struct SortLinkAttribute {
    /// 排序链表头
    pub sort_link: *mut LinkedList,
    /// 游标
    pub cursor: u16,
    _reserved: u16,
}

// impl SortLinkAttribute {
//     /// 获取当前游标位置的链表对象
//     #[cfg(not(feature = "use_single_list"))]
//     pub fn get_cursor_list(&self) -> &DLList {
//         &self.sort_link[self.cursor as usize]
//     }

//     /// 获取当前游标位置的链表对象可变引用
//     #[cfg(not(feature = "use_single_list"))]
//     pub fn get_cursor_list_mut(&mut self) -> &mut DLList {
//         &mut self.sort_link[self.cursor as usize]
//     }

//     /// 更新游标
//     #[cfg(not(feature = "use_single_list"))]
//     pub fn update_cursor(&mut self) {
//         self.cursor = (self.cursor + 1) & (OS_TSK_SORTLINK_MASK as u16);
//     }
// }

// /// 排序链表实现
// #[cfg(feature = "use_single_list")]
// impl SortLinkAttribute {
//     /// 初始化排序链表
//     pub fn init() -> Result<Self, &'static str> {
//         // 分配内存
//         let mut sort_link = Vec::with_capacity(1);
//         sort_link.push(DLList::new());

//         Ok(Self {
//             sort_link,
//             cursor: 0,
//         })
//     }

//     /// 添加节点到排序链表
//     pub fn add_node(&mut self, list: &mut SortLinkList) {
//         // 检查轮数是否超过最大值
//         if list.idx_roll_num > OS_TSK_MAX_ROLLNUM {
//             list.idx_roll_num = OS_TSK_MAX_ROLLNUM;
//         }

//         let list_object = &mut self.sort_link[0];

//         if list_object.is_empty() {
//             // 如果链表为空，直接插入
//             list_object.tail_insert(&mut list.sort_link_node);
//         } else {
//             // 找到合适的位置插入
//             let mut current = list_object.get_next_entry::<SortLinkList>(
//                 SortLinkList::sort_link_node_offset());

//             loop {
//                 let current_node = unsafe { current.as_mut() };

//                 if current_node.roll_num() <= list.roll_num() {
//                     list.roll_num_sub(current_node.roll_num());
//                 } else {
//                     current_node.roll_num_sub(list.roll_num());
//                     break;
//                 }

//                 // 移动到下一个节点
//                 if current_node.sort_link_node.get_next() == list_object as *mut _ {
//                     break;
//                 }

//                 current = current_node.sort_link_node.get_next_entry::<SortLinkList>(
//                     SortLinkList::sort_link_node_offset());
//             }

//             // 插入链表
//             let current_node = unsafe { current.as_mut() };
//             current_node.sort_link_node.tail_insert(&mut list.sort_link_node);
//         }
//     }

//     /// 删除排序链表中的节点
//     pub fn delete_node(&mut self, list: &mut SortLinkList) {
//         let list_object = &mut self.sort_link[0];

//         // 检查节点是否在正确的链表中
//         if !self.check_sort_link(list_object, &list.sort_link_node) {
//             // 错误处理：节点不在链表中
//             panic!("Invalid sortlink node");
//         }

//         // 如果不是链表的最后一个节点，调整下一个节点的轮数
//         if list.sort_link_node.get_next() != list_object as *mut _ {
//             let next = list.sort_link_node.get_next_entry::<SortLinkList>(
//                 SortLinkList::sort_link_node_offset());
//             let next_node = unsafe { next.as_mut() };
//             next_node.roll_num_add(list.roll_num());
//         }

//         // 从链表中删除节点
//         list.sort_link_node.delete();
//     }

//     /// 获取下一个到期时间
//     pub fn get_next_expire_time(&self) -> u32 {
//         let list_object = &self.sort_link[0];

//         if !list_object.is_empty() {
//             let first = list_object.get_next_entry::<SortLinkList>(
//                 SortLinkList::sort_link_node_offset());
//             let first_node = unsafe { first.as_ref() };
//             return first_node.idx_roll_num;
//         }

//         OS_INVALID_VALUE
//     }

//     /// 更新到期时间
//     pub fn update_expire_time(&mut self, sleep_ticks: u32) {
//         if sleep_ticks == 0 {
//             return;
//         }

//         let list_object = &mut self.sort_link[0];

//         if !list_object.is_empty() {
//             let first = list_object.get_next_entry::<SortLinkList>(
//                 SortLinkList::sort_link_node_offset());
//             let first_node = unsafe { first.as_mut() };
//             first_node.roll_num_sub(sleep_ticks - 1);
//         }
//     }

//     /// 获取目标节点的到期时间
//     pub fn get_target_expire_time(&self, target: &SortLinkList) -> u32 {
//         let list_object = &self.sort_link[0];
//         let mut roll_num = target.idx_roll_num;

//         let mut current = list_object.get_next_entry::<SortLinkList>(
//             SortLinkList::sort_link_node_offset());

//         while unsafe { !core::ptr::eq(current.as_ptr(), target) } {
//             let current_node = unsafe { current.as_ref() };
//             roll_num += current_node.idx_roll_num;

//             current = current_node.sort_link_node.get_next_entry::<SortLinkList>(
//                 SortLinkList::sort_link_node_offset());
//         }

//         roll_num
//     }

//     /// 检查节点是否在链表中
//     fn check_sort_link(&self, list_head: &DLList, list_node: &DLListNode) -> bool {
//         let mut tmp = list_node.prev;

//         // 从节点向前遍历，查找链表头
//         while tmp != list_node as *const _ as *mut _ {
//             if tmp == list_head as *const _ as *mut _ {
//                 return true;
//             }
//             let tmp_node = unsafe { &*(tmp as *const DLListNode) };
//             tmp = tmp_node.prev;
//         }

//         false
//     }
// }

// /// 多链表实现
// #[cfg(not(feature = "use_single_list"))]
// impl SortLinkAttribute {
//     /// 初始化排序链表
//     pub fn init() -> Result<Self, &'static str> {
//         // 分配内存
//         let mut sort_link = Vec::with_capacity(OS_TSK_SORTLINK_LEN as usize);

//         // 初始化所有链表头
//         for _ in 0..OS_TSK_SORTLINK_LEN {
//             sort_link.push(DLList::new());
//         }

//         Ok(Self {
//             sort_link,
//             cursor: 0,
//         })
//     }

//     /// 计算过期时间
//     fn calc_expire_time(roll_num: u32, sort_index: u32, cur_sort_index: u16) -> u32 {
//         let mut sort_index = sort_index;

//         if sort_index > cur_sort_index as u32 {
//             sort_index -= cur_sort_index as u32;
//         } else {
//             sort_index = OS_TSK_SORTLINK_LEN - (cur_sort_index as u32) + sort_index;
//         }

//         ((roll_num - 1) << OS_TSK_SORTLINK_LOGLEN) + sort_index
//     }

//     /// 添加节点到排序链表
//     pub fn add_node(&mut self, list: &mut SortLinkList) {
//         // 检查轮数是否超过最大值
//         if list.idx_roll_num > OS_TSK_MAX_ROLLNUM {
//             list.idx_roll_num = OS_TSK_MAX_ROLLNUM;
//         }

//         let timeout = list.idx_roll_num;
//         let mut sort_index = timeout & OS_TSK_SORTLINK_MASK;
//         let mut roll_num = (timeout >> OS_TSK_SORTLINK_LOGLEN) + 1;

//         if sort_index == 0 {
//             roll_num -= 1;
//         }

//         // 设置轮数
//         list.set_roll_num(roll_num);

//         // 调整排序索引
//         sort_index = (sort_index + self.cursor as u32) & OS_TSK_SORTLINK_MASK;
//         list.set_sort_index(sort_index);

//         let list_object = &mut self.sort_link[sort_index as usize];

//         if list_object.is_empty() {
//             // 如果链表为空，直接插入
//             list_object.tail_insert(&mut list.sort_link_node);
//         } else {
//             // 找到合适的位置插入
//             let mut current = list_object.get_next_entry::<SortLinkList>(
//                 SortLinkList::sort_link_node_offset());

//             loop {
//                 let current_node = unsafe { current.as_mut() };

//                 if current_node.roll_num() <= list.roll_num() {
//                     list.roll_num_sub(current_node.roll_num());
//                 } else {
//                     current_node.roll_num_sub(list.roll_num());
//                     break;
//                 }

//                 // 移动到下一个节点
//                 if current_node.sort_link_node.get_next() == list_object as *mut _ {
//                     break;
//                 }

//                 current = current_node.sort_link_node.get_next_entry::<SortLinkList>(
//                     SortLinkList::sort_link_node_offset());
//             }

//             // 插入链表
//             let current_node = unsafe { current.as_mut() };
//             current_node.sort_link_node.tail_insert(&mut list.sort_link_node);
//         }
//     }

//     /// 删除排序链表中的节点
//     pub fn delete_node(&mut self, list: &mut SortLinkList) {
//         let sort_index = list.sort_index();
//         let list_object = &mut self.sort_link[sort_index as usize];

//         // 检查节点是否在正确的链表中
//         if !self.check_sort_link(list_object, &list.sort_link_node) {
//             // 错误处理：节点不在链表中
//             panic!("Invalid sortlink node");
//         }

//         // 如果不是链表的最后一个节点，调整下一个节点的轮数
//         if list.sort_link_node.get_next() != list_object as *mut _ {
//             let next = list.sort_link_node.get_next_entry::<SortLinkList>(
//                 SortLinkList::sort_link_node_offset());
//             let next_node = unsafe { next.as_mut() };
//             next_node.roll_num_add(list.roll_num());
//         }

//         // 从链表中删除节点
//         list.sort_link_node.delete();
//     }

//     /// 获取下一个到期时间
//     pub fn get_next_expire_time(&self) -> u32 {
//         let cursor = (self.cursor + 1) & (OS_TSK_SORTLINK_MASK as u16);
//         let mut min_sort_index = OS_INVALID_VALUE;
//         let mut min_roll_num = OS_TSK_LOW_BITS_MASK;
//         let mut expire_time = OS_INVALID_VALUE;

//         // 查找具有最小轮数的节点
//         for i in 0..OS_TSK_SORTLINK_LEN {
//             let idx = ((cursor as u32 + i) & OS_TSK_SORTLINK_MASK) as usize;
//             let list_object = &self.sort_link[idx];

//             if !list_object.is_empty() {
//                 let first = list_object.get_next_entry::<SortLinkList>(
//                     SortLinkList::sort_link_node_offset());
//                 let first_node = unsafe { first.as_ref() };

//                 if min_roll_num > first_node.roll_num() {
//                     min_roll_num = first_node.roll_num();
//                     min_sort_index = (cursor as u32 + i) & OS_TSK_SORTLINK_MASK;
//                 }
//             }
//         }

//         // 计算到期时间
//         if min_roll_num != OS_TSK_LOW_BITS_MASK {
//             expire_time = Self::calc_expire_time(min_roll_num, min_sort_index, self.cursor);
//         }

//         expire_time
//     }

//     /// 更新到期时间
//     pub fn update_expire_time(&mut self, sleep_ticks: u32) {
//         if sleep_ticks == 0 {
//             return;
//         }

//         let sort_index = sleep_ticks & OS_TSK_SORTLINK_MASK;
//         let mut roll_num = (sleep_ticks >> OS_TSK_SORTLINK_LOGLEN) + 1;
//         let mut sort_idx = sort_index;

//         if sort_index == 0 {
//             roll_num -= 1;
//             sort_idx = OS_TSK_SORTLINK_LEN;
//         }

//         // 更新所有链表中节点的轮数
//         for i in 0..OS_TSK_SORTLINK_LEN {
//             let idx = ((self.cursor as u32 + i) & OS_TSK_SORTLINK_MASK) as usize;
//             let list_object = &mut self.sort_link[idx];

//             if !list_object.is_empty() {
//                 let first = list_object.get_next_entry::<SortLinkList>(
//                     SortLinkList::sort_link_node_offset());
//                 let first_node = unsafe { first.as_mut() };

//                 first_node.roll_num_sub(roll_num - 1);

//                 if (i > 0) && (i < sort_idx) {
//                     first_node.roll_num_dec();
//                 }
//             }
//         }

//         // 更新游标
//         self.cursor = ((self.cursor as u32 + sleep_ticks - 1) % OS_TSK_SORTLINK_LEN) as u16;
//     }

//     /// 获取目标节点的到期时间
//     pub fn get_target_expire_time(&self, target: &SortLinkList) -> u32 {
//         let sort_index = target.sort_index();
//         let mut roll_num = target.roll_num();

//         let list_object = &self.sort_link[sort_index as usize];
//         let mut current = list_object.get_next_entry::<SortLinkList>(
//             SortLinkList::sort_link_node_offset());

//         while unsafe { !core::ptr::eq(current.as_ptr(), target) } {
//             let current_node = unsafe { current.as_ref() };
//             roll_num += current_node.roll_num();

//             current = current_node.sort_link_node.get_next_entry::<SortLinkList>(
//                 SortLinkList::sort_link_node_offset());
//         }

//         Self::calc_expire_time(roll_num, sort_index, self.cursor)
//     }

//     /// 检查节点是否在链表中
//     fn check_sort_link(&self, list_head: &DLList, list_node: &DLListNode) -> bool {
//         let mut tmp = list_node.prev;

//         // 从节点向前遍历，查找链表头
//         while tmp != list_node as *const _ as *mut _ {
//             if tmp == list_head as *const _ as *mut _ {
//                 return true;
//             }
//             let tmp_node = unsafe { &*(tmp as *const DLListNode) };
//             tmp = tmp_node.prev;
//         }

//         false
//     }
// }

// // 公共 API 函数
// pub fn os_sort_link_init(sort_link_header: &mut SortLinkAttribute) -> Result<(), &'static str> {
//     *sort_link_header = SortLinkAttribute::init()?;
//     Ok(())
// }

// pub fn os_add_to_sort_link(sort_link_header: &mut SortLinkAttribute, sort_list: &mut SortLinkList) {
//     sort_link_header.add_node(sort_list);
// }

// pub fn os_delete_sort_link(sort_link_header: &mut SortLinkAttribute, sort_list: &mut SortLinkList) {
//     sort_link_header.delete_node(sort_list);
// }

// pub fn os_sort_link_get_next_expire_time(sort_link_header: &SortLinkAttribute) -> u32 {
//     sort_link_header.get_next_expire_time()
// }

// pub fn os_sort_link_get_target_expire_time(sort_link_header: &SortLinkAttribute, target_sort_list: &SortLinkList) -> u32 {
//     sort_link_header.get_target_expire_time(target_sort_list)
// }

// pub fn os_sort_link_update_expire_time(sleep_ticks: u32, sort_link_header: &mut SortLinkAttribute) {
//     sort_link_header.update_expire_time(sleep_ticks);
// }
