use crate::{
    hwi::{int_lock, int_restore},
    utils::sortlink::SortLinkAttribute,
};

const LOSCFG_KERNEL_CORE_NUM: usize = 1;

/// 每个CPU核心的特定数据结构
#[repr(C)]
#[derive(Debug)]
pub struct Percpu {
    /// 任务排序链表
    pub task_sort_link: SortLinkAttribute,

    /// 软件定时器排序链表
    pub swtmr_sort_link: SortLinkAttribute,

    /// 空闲任务ID
    pub idle_task_id: u32,

    /// 任务锁定计数
    pub task_lock_cnt: u32,

    /// 软件定时器超时队列ID
    pub swtmr_handler_queue: u32,

    /// 软件定时器任务ID
    pub swtmr_task_id: u32,

    /// 调度器标志位
    pub sched_flag: u32,
}

impl Percpu {
    pub const UNINIT: Self = Self {
        task_sort_link: SortLinkAttribute::UNINIT,
        swtmr_sort_link: SortLinkAttribute::UNINIT,
        idle_task_id: 0,
        task_lock_cnt: 0,
        swtmr_handler_queue: 0,
        swtmr_task_id: 0,
        sched_flag: 0,
    };
}

pub enum SchedFlag {
    #[allow(dead_code)]
    NotNeeded = 0,
    Pending = 1,
}

#[unsafe(export_name = "g_percpu")]
pub static mut PERCPU: [Percpu; LOSCFG_KERNEL_CORE_NUM] = [Percpu::UNINIT; LOSCFG_KERNEL_CORE_NUM];

#[inline]
pub fn os_percpu_get() -> &'static mut Percpu {
    unsafe { &mut PERCPU[0] }
}

#[inline]
pub fn can_preempt_in_scheduler() -> bool {
    let percpu = os_percpu_get();
    let preemptable = percpu.task_lock_cnt == 0;
    if !preemptable {
        percpu.sched_flag = SchedFlag::Pending as u32;
    }
    preemptable
}

#[inline]
pub fn can_preempt() -> bool {
    let int_save = int_lock();
    let preemptable = can_preempt_in_scheduler();
    int_restore(int_save);
    preemptable
}
