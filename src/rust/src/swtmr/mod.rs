use crate::{
    percpu::os_percpu_get,
    utils::{
        list::LinkedList,
        sortlink::{SortLinkList, os_add_to_sort_link},
    },
};

pub type SwtmrProcFunc = Option<unsafe extern "C" fn(arg: usize) -> ()>;

#[repr(C)]
pub struct SwtmrHandlerItem {
    pub handler: SwtmrProcFunc,
    pub arg: usize,
}

pub enum SwtmrState {
    Unused = 0,  // 软件定时器未使用
    Created = 1, // 软件定时器已创建
    Ticking = 2, // 软件定时器正在计时
}

pub enum SwtmrMode {
    Once = 0,
    Period = 1,
    NoSelfDelete = 2,
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

#[unsafe(no_mangle)]
#[allow(non_upper_case_globals)]
pub static mut g_swtmrFreeList: LinkedList = LinkedList {
    prev: core::ptr::null_mut(),
    next: core::ptr::null_mut(),
};

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
