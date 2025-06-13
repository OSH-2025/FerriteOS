use crate::{
    container_of,
    utils::{list::LinkedList, sortlink::SortLinkList},
};

pub type TimerHandler = Option<extern "C" fn() -> ()>;

#[repr(C)]
pub struct TimerHandlerItem {
    pub handler: TimerHandler,
}

impl TimerHandlerItem {
    pub const UNINIT: TimerHandlerItem = TimerHandlerItem { handler: None };

    #[inline]
    pub fn new(handler: TimerHandler) -> Self {
        TimerHandlerItem { handler }
    }
}

pub const TIMER_HANDLE_ITEM_SIZE: usize = core::mem::size_of::<TimerHandlerItem>();

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u8)]
pub enum TimerState {
    Unused = 0,
    Created = 1,
    Running = 2,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(u8)]
pub enum TimerMode {
    OneShot = 0,
    Periodic = 1,
    NoSelfDelete = 2,
}

impl TryFrom<u8> for TimerMode {
    type Error = ();
    fn try_from(value: u8) -> Result<Self, Self::Error> {
        match value {
            0 => Ok(TimerMode::OneShot),
            1 => Ok(TimerMode::Periodic),
            2 => Ok(TimerMode::NoSelfDelete),
            _ => Err(()),
        }
    }
}

/// 队列ID封装
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
#[repr(transparent)]
pub struct TimerId(pub u32);

impl TimerId {
    /// ID分割位数
    const SPLIT_BIT: u32 = 16;

    /// 从计数和索引创建定时器ID
    pub fn new(count: u16, index: u16) -> Self {
        Self(((count as u32) << Self::SPLIT_BIT) | (index as u32))
    }

    /// 获取索引部分
    pub fn get_index(&self) -> u16 {
        (self.0 & ((1 << Self::SPLIT_BIT) - 1)) as u16
    }

    /// 获取计数部分
    pub fn get_count(&self) -> u16 {
        (self.0 >> Self::SPLIT_BIT) as u16
    }

    /// 创建下一个版本的ID（计数+1）
    pub fn increment_count(&self) -> Self {
        Self::new(self.get_count().wrapping_add(1), self.get_index())
    }
}

impl From<u32> for TimerId {
    fn from(id: u32) -> Self {
        Self(id)
    }
}

impl From<TimerId> for u32 {
    fn from(id: TimerId) -> Self {
        id.0
    }
}

/// 软件定时器控制块
#[repr(C)]
pub struct TimerControlBlock {
    /// 排序链表节点
    pub sort_list: SortLinkList,
    /// 软件定时器状态
    pub state: TimerState,
    /// 软件定时器模式
    pub mode: TimerMode,
    /// 软件定时器ID
    pub timer_id: TimerId,
    /// 软件定时器的超时时间(单位:tick)
    pub timeout: u32,
    /// 软件定时器超时处理回调函数
    pub handler: TimerHandler,
}

impl TimerControlBlock {
    pub const UNINIT: TimerControlBlock = TimerControlBlock {
        sort_list: SortLinkList::new(),
        state: TimerState::Unused,
        mode: TimerMode::OneShot,
        timer_id: TimerId(0),
        timeout: 0,
        handler: None,
    };

    #[inline]
    pub fn set_state(&mut self, state: TimerState) {
        self.state = state;
    }

    #[inline]
    pub fn get_state(&self) -> TimerState {
        self.state
    }

    /// 队列ID
    #[inline]
    pub fn get_id(&self) -> TimerId {
        self.timer_id
    }

    /// 设置队列ID
    #[inline]
    pub fn set_id(&mut self, id: TimerId) {
        self.timer_id = id;
    }

    /// 检查是否为指定的句柄
    #[inline]
    pub fn matches_id(&self, id: TimerId) -> bool {
        self.get_id() == id
    }

    #[inline]
    pub fn increment_id_counter(&mut self) {
        self.set_id(self.timer_id.increment_count());
    }

    #[inline]
    pub fn get_mode(&self) -> TimerMode {
        self.mode
    }

    #[inline]
    pub fn set_mode(&mut self, mode: TimerMode) {
        self.mode = mode;
    }

    #[inline]
    pub fn get_timeout(&self) -> u32 {
        self.timeout
    }

    #[inline]
    pub fn set_timeout(&mut self, timeout: u32) {
        self.timeout = timeout;
    }

    #[inline]
    pub fn get_handler(&self) -> TimerHandler {
        self.handler
    }

    #[inline]
    pub fn set_handler(&mut self, handler: TimerHandler) {
        self.handler = handler;
    }

    #[inline]
    pub fn from_list(list: *const LinkedList) -> &'static mut Self {
        let ptr = container_of!(list, Self, sort_list.sort_link_node);
        unsafe { &mut *ptr }
    }

    #[inline]
    pub fn initialize(&mut self, mode: TimerMode, timeout: u32, handler: TimerHandler) {
        self.set_state(TimerState::Created);
        self.set_mode(mode);
        self.set_timeout(timeout);
        self.set_handler(handler);
    }
}

impl Default for TimerControlBlock {
    fn default() -> Self {
        TimerControlBlock {
            sort_list: SortLinkList::new(),
            state: TimerState::Unused,
            mode: TimerMode::OneShot,
            timer_id: TimerId(0),
            timeout: 0,
            handler: None,
        }
    }
}
