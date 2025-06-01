//! 事件相关类型定义
use crate::utils::list::LinkedList;

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u32)]
pub enum EventWaitMode {
    /// 或模式：任意一个事件满足即可
    Or = 0x02,
    /// 与模式：所有事件都必须满足
    And = 0x04,
    /// 清除模式：读取后清除事件
    Clear = 0x01,
}

impl EventWaitMode {
    /// 检查是否包含OR模式
    pub fn is_or(mode: u32) -> bool {
        (mode & Self::Or as u32) != 0
    }

    /// 检查是否包含AND模式
    pub fn is_and(mode: u32) -> bool {
        (mode & Self::And as u32) != 0
    }

    /// 检查是否包含Clear模式
    pub fn is_clear(mode: u32) -> bool {
        (mode & Self::Clear as u32) != 0
    }

    /// 验证模式的有效性
    pub fn validate(mode: u32) -> bool {
        // 检查是否有未定义的位
        let valid_mask = Self::Or as u32 | Self::And as u32 | Self::Clear as u32;
        if (mode & !valid_mask) != 0 {
            return false;
        }

        let or_flag = Self::is_or(mode);
        let and_flag = Self::is_and(mode);

        and_flag ^ or_flag
    }
}

/// 事件控制块
#[repr(C)]
#[derive(Debug)]
pub struct EventCB {
    /// 事件ID，使用原子操作保证线程安全
    pub event_id: u32,
    /// 等待此事件的任务列表
    pub wait_list: LinkedList,
}

impl EventCB {
    /// 创建新的事件控制块
    pub const fn new() -> Self {
        Self {
            event_id: 0,
            wait_list: LinkedList::new(),
        }
    }

    /// 设置事件位
    pub fn set_events(&mut self, events: u32) {
        self.event_id |= events;
    }

    /// 清除事件位
    pub fn clear_events(&mut self, events: u32) {
        self.event_id &= !events;
    }

    /// 检查等待列表是否为空
    pub fn is_wait_list_empty(&self) -> bool {
        LinkedList::is_empty(&raw const self.wait_list)
    }
}

impl Default for EventCB {
    fn default() -> Self {
        Self::new()
    }
}
