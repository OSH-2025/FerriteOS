//! 任务相关定义

#[repr(C)]
pub struct LosTaskCB {
    pub task_status: u32,
    pub pend_list: crate::event::LOS_DL_LIST, // 添加这个字段，用于container_of!宏
    pub event_mask: u32,
    pub event_mode: u32,
    // 其他字段...
}