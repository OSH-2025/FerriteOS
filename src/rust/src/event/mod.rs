use crate::utils::list::LinkedList;

#[repr(C)]
#[derive(Debug)]
pub struct EventCB {
    pub event_id: u32,          // 事事件ID，每一位标识一种事件类型
    pub event_list: LinkedList, // 读取事件的任务链表
}
