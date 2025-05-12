use crate::{
    percpu::os_percpu_get,
    utils::{
        list::LinkedList,
        sortlink::{SortLinkList, os_add_to_sort_link},
    },
};

pub const KERNEL_SWTMR_LIMIT: u16 = 1024;
pub const OS_SWTMR_MAX_TIMERID: u16 = (u16::MAX / KERNEL_SWTMR_LIMIT) * KERNEL_SWTMR_LIMIT;

pub type SwtmrProcFunc = Option<unsafe extern "C" fn(arg: usize) -> ()>;

#[repr(C)]
pub struct SwtmrHandlerItem {
    pub handler: SwtmrProcFunc,
    pub arg: usize,
}

pub enum SwtmrState {
    Unused = 0,
    Created = 1,
    Ticking = 2,
}

impl SwtmrState {
    #[allow(dead_code)]
    fn from_u8(value: u8) -> Option<Self> {
        match value {
            0 => Some(SwtmrState::Unused),
            1 => Some(SwtmrState::Created),
            2 => Some(SwtmrState::Ticking),
            _ => None,
        }
    }
}

pub enum SwtmrMode {
    Once = 0,
    Period = 1,
    NoSelfDelete = 2,
}

impl SwtmrMode {
    fn from_u8(value: u8) -> Option<Self> {
        match value {
            0 => Some(SwtmrMode::Once),
            1 => Some(SwtmrMode::Period),
            2 => Some(SwtmrMode::NoSelfDelete),
            _ => None,
        }
    }
}

/// 软件定时器控制块
#[repr(C)]
pub struct LosSwtmrCB {
    /// 排序链表节点
    pub sort_list: SortLinkList,
    /// 软件定时器状态
    pub state: u8,
    /// 软件定时器模式
    pub mode: u8,
    /// 软件定时器重复计时次数
    pub overrun: u8,
    /// 软件定时器ID
    pub timer_id: u16,
    /// 周期性软件定时器的超时间隔(单位:tick)
    pub interval: u32,
    /// 一次性软件定时器的超时间隔(单位:tick)
    pub expiry: u32,
    /// 回调函数调用时传入的参数
    pub arg: usize,
    /// 软件定时器超时处理回调函数
    pub handler: SwtmrProcFunc,
}

/// Free list of Software Timers
#[unsafe(export_name = "g_swtmrFreeList")]
pub static mut SWTMR_FREE_LIST: LinkedList = LinkedList {
    prev: core::ptr::null_mut(),
    next: core::ptr::null_mut(),
};

// TODO 删除export_name
#[unsafe(export_name = "OsSwtmrStart")]
pub extern "C" fn os_swtmr_start(swtmr: &mut LosSwtmrCB) {
    // 根据定时器类型和重复次数选择合适的过期时间
    let timeout = if (swtmr.overrun == 0)
        && ((swtmr.mode == SwtmrMode::Once as u8) || (swtmr.mode == SwtmrMode::NoSelfDelete as u8))
    {
        swtmr.expiry
    } else {
        swtmr.interval
    };
    // 设置排序链表的值为选定的过期时间
    swtmr.sort_list.idx_roll_num = timeout;

    // 获取当前CPU的软件定时器排序链表，并添加定时器
    os_add_to_sort_link(&os_percpu_get().swtmr_sort_link, &mut swtmr.sort_list);
    // 更新定时器状态为正在计时
    swtmr.state = SwtmrState::Ticking as u8;
}

// TODO 删除export_name
#[unsafe(export_name = "OsSwtmrDelete")]
pub extern "C" fn os_swtmr_delete(swtmr: &mut LosSwtmrCB) {
    // 将定时器的排序链表节点插入到空闲链表尾部
    LinkedList::tail_insert(
        &raw mut SWTMR_FREE_LIST,
        &mut swtmr.sort_list.sort_link_node,
    );
    // 更新定时器状态为未使用
    swtmr.state = SwtmrState::Unused as u8;
}

// TODO 删除export_name
#[unsafe(export_name = "OsSwtmrUpdate")]
pub extern "C" fn os_swtmr_update(swtmr: &mut LosSwtmrCB) {
    match SwtmrMode::from_u8(swtmr.mode) {
        Some(SwtmrMode::Once) => {
            os_swtmr_delete(swtmr);
            if swtmr.timer_id < (OS_SWTMR_MAX_TIMERID - KERNEL_SWTMR_LIMIT) {
                swtmr.timer_id += KERNEL_SWTMR_LIMIT;
            } else {
                swtmr.timer_id %= KERNEL_SWTMR_LIMIT;
            }
        }
        Some(SwtmrMode::Period) => {
            swtmr.overrun += 1;
            os_swtmr_start(swtmr);
        }
        Some(SwtmrMode::NoSelfDelete) => {
            swtmr.state = SwtmrState::Created as u8;
        }
        None => {}
    }
}
