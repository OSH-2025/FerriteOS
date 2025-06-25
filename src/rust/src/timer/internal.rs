use crate::percpu::os_percpu_get;
use crate::timer::global::TimerPool;
use crate::timer::types::TimerControlBlock;
use crate::timer::types::TimerMode;
use crate::timer::types::TimerState;
use crate::utils::sortlink::add_to_sort_link;
use crate::utils::sortlink::delete_from_sort_link;
use crate::utils::sortlink::get_target_expire_time;

/// 启动定时器（内部函数）
pub(super) fn timer_start_internal(timer: &mut TimerControlBlock) {
    // 对应OsSwtmrStart
    timer.sort_list.set_timeout(timer.get_timeout());
    add_to_sort_link(&mut os_percpu_get().swtmr_sort_link, &mut timer.sort_list);
    timer.set_state(TimerState::Running);
}

/// 停止定时器（内部函数）
pub(super) fn timer_stop_internal(timer: &mut TimerControlBlock) {
    delete_from_sort_link(&mut os_percpu_get().swtmr_sort_link, &mut timer.sort_list);
    timer.state = TimerState::Created;
}

/// 删除定时器（内部函数）
pub(super) fn timer_delete_internal(timer: &mut TimerControlBlock) {
    TimerPool::deallocate(timer);
}

/// 更新定时器（内部函数）
pub(super) fn timer_update_internal(timer: &mut TimerControlBlock) {
    match timer.get_mode() {
        TimerMode::OneShot => {
            timer_delete_internal(timer);
            timer.increment_id_counter();
        }
        TimerMode::NoSelfDelete => {
            timer.set_state(TimerState::Created);
        }
        TimerMode::Periodic => {
            timer_start_internal(timer);
        }
    }
}

/// 获取定时器剩余时间（内部函数）
pub(super) fn timer_get_time_internal(timer: &TimerControlBlock) -> u32 {
    // 对应OsSwtmrTimeGet
    let sort_link_header = &os_percpu_get().swtmr_sort_link;
    get_target_expire_time(sort_link_header, &timer.sort_list)
}
