use crate::utils::sortlink::SortLinkAttribute;

const LOSCFG_KERNEL_CORE_NUM: usize = 1;

/// 每个CPU核心的特定数据结构
#[repr(C)]
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

unsafe extern "C" {
    static mut g_percpu: [Percpu; LOSCFG_KERNEL_CORE_NUM];
}

#[inline]
pub fn os_percpu_get() -> &'static mut Percpu {
    unsafe { &mut g_percpu[0] }
}
