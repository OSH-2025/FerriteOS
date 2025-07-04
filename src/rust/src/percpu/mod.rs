use crate::{
    interrupt::{disable_interrupts, restore_interrupt_state},
    queue::types::QueueId,
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
    pub timer_task_id: u32,

    /// 调度器标志位
    pub needs_reschedule: u32,
}

impl Percpu {
    pub const UNINIT: Self = Self {
        task_sort_link: SortLinkAttribute::UNINIT,
        swtmr_sort_link: SortLinkAttribute::UNINIT,
        idle_task_id: 0,
        task_lock_cnt: 0,
        swtmr_handler_queue: 0,
        timer_task_id: 0,
        needs_reschedule: 0,
    };

    #[inline]
    pub fn get_timer_queue_id(&self) -> QueueId {
        self.swtmr_handler_queue.into()
    }

    #[inline]
    pub fn set_timer_queue_id(&mut self, queue_id: QueueId) {
        self.swtmr_handler_queue = queue_id.into();
    }

    #[inline]
    pub fn set_timer_task_id(&mut self, task_id: u32) {
        self.timer_task_id = task_id;
    }
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
        percpu.needs_reschedule = 1;
    }
    preemptable
}

#[inline]
pub fn can_preempt() -> bool {
    let int_save = disable_interrupts();
    let preemptable = can_preempt_in_scheduler();
    restore_interrupt_state(int_save);
    preemptable
}
