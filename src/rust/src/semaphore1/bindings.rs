use crate::utils::list::LinkedList;

/// 架构相关的中断锁定
#[inline]
pub fn arch_int_lock() -> u32 {
    crate::interrupt::disable_interrupts()
}

/// 架构相关的中断恢复
#[inline]
pub fn arch_int_restore(state: u32) {
    crate::interrupt::restore_interrupt_state(state)
}

/// 判断是否在中断上下文中
#[inline]
pub fn is_interrupt_context() -> bool {
    crate::interrupt::is_int_active()
}

/// 判断是否可抢占
#[inline]
pub fn is_preemptable() -> bool {
    crate::percpu::can_preempt()
}

/// 任务调度
#[inline]
pub fn schedule() {
    crate::task::sched::schedule()
}

/// 任务控制块结构
pub use crate::task::types::TaskCB;

/// 系统任务标志
pub const OS_TASK_FLAG_SYSTEM: u32 = 0x0002;

/// 任务超时状态
pub const OS_TASK_STATUS_TIMEOUT: u32 = 0x0040;

/// 获取当前任务
#[inline]
pub fn get_current_task() -> &'static mut TaskCB {
    crate::ffi::bindings::get_current_task()
}

/// 任务等待
#[inline]
pub fn task_wait(list: &mut LinkedList, timeout: u32) {
    crate::task::sync::wait::task_wait(list, timeout)
}

/// 唤醒任务
#[inline]
pub fn task_wait_abort(task: &mut TaskCB) {
    crate::task::sync::wait::task_wake(task)
}

/// 从等待列表获取任务
#[inline]
pub fn task_from_wait_list(node: *const LinkedList) -> &'static mut TaskCB {
    crate::task::types::TaskCB::from_pend_list(node)
}