use crate::{ffi::get_current_task, utils::list::LinkedList};
use core::sync::atomic::{AtomicU32, Ordering};

use super::types::TaskCB;

const OS_PRIORITY_QUEUE_NUM: usize = 32;
const PRIQUEUE_PRIOR0_BIT: u32 = 0x8000_0000;

static mut PRI_QUEUE_LIST: [LinkedList; OS_PRIORITY_QUEUE_NUM] =
    [LinkedList::UNINIT; OS_PRIORITY_QUEUE_NUM];

static PRI_QUEUE_BITMAP: AtomicU32 = AtomicU32::new(0);

/// 初始化优先级队列
#[unsafe(export_name = "OsPriQueueInit")]
pub extern "C" fn priority_queue_init() {
    for priority in 0..OS_PRIORITY_QUEUE_NUM {
        unsafe {
            LinkedList::init(&mut PRI_QUEUE_LIST[priority]);
        }
    }
    // 打印percpu
    let percpu = crate::percpu::os_percpu_get();
    unsafe {
        crate::utils::printf::dprintf(
            b"%d %d\n\0" as *const u8,
            crate::percpu::SchedFlag::Pending as u32,
            percpu.sched_flag,
        )
    };
}

/// 将任务节点插入优先级队列头部
#[unsafe(export_name = "OsPriQueueEnqueueHead")]
pub extern "C" fn priority_queue_insert_at_front(priqueue_item: &mut LinkedList, priority: u32) {
    assert!(priqueue_item.next.is_null(), "节点next指针必须为null");
    unsafe {
        // 如果该优先级队列为空，则在位图中设置对应位
        if LinkedList::is_empty(&mut PRI_QUEUE_LIST[priority as usize]) {
            PRI_QUEUE_BITMAP.fetch_or(PRIQUEUE_PRIOR0_BIT >> priority, Ordering::Release);
        }

        // 将节点插入到优先级队列的头部
        LinkedList::head_insert(&mut PRI_QUEUE_LIST[priority as usize], priqueue_item);
    }
}

/// 将任务节点插入优先级队列尾部
#[unsafe(export_name = "OsPriQueueEnqueue")]
pub extern "C" fn priority_queue_insert_at_back(priqueue_item: &mut LinkedList, priority: u32) {
    assert!(priqueue_item.next.is_null(), "节点next指针必须为null");
    unsafe {
        // 如果该优先级队列为空，则在位图中设置对应位
        if LinkedList::is_empty(&mut PRI_QUEUE_LIST[priority as usize]) {
            PRI_QUEUE_BITMAP.fetch_or(PRIQUEUE_PRIOR0_BIT >> priority, Ordering::Release);
        }

        // 将节点插入到优先级队列的尾部
        LinkedList::tail_insert(&mut PRI_QUEUE_LIST[priority as usize], priqueue_item);
    }
}

/// 从优先级队列中移除任务节点
#[unsafe(export_name = "OsPriQueueDequeue")]
pub extern "C" fn priority_queue_remove(priqueue_item: &mut LinkedList) {
    // 从链表中删除节点
    LinkedList::remove(priqueue_item);

    // 获取包含此节点的任务控制块
    let run_task = TaskCB::from_pend_list(priqueue_item);

    unsafe {
        // 如果该优先级队列为空，原子更新位图
        if LinkedList::is_empty(&mut PRI_QUEUE_LIST[run_task.priority as usize]) {
            PRI_QUEUE_BITMAP.fetch_and(
                !(PRIQUEUE_PRIOR0_BIT >> run_task.priority),
                Ordering::Release,
            );
        }
    }
}

#[unsafe(export_name = "OsPriQueueSize")]
pub extern "C" fn priority_queue_get_size(priority: u32) -> u32 {
    let mut item_count = 0;
    // TODO
    // assert!(crate::arch::core::arch_int_locked());
    unsafe {
        // 获取优先级队列的头节点
        let list_head = &mut PRI_QUEUE_LIST[priority as usize];
        // 手动遍历链表
        let mut current = list_head.next;
        while !current.is_null() && current != list_head {
            item_count += 1;
            current = (*current).next;
        }
    }
    item_count
}

/// 获取优先级队列中优先级最高的任务
#[unsafe(export_name = "OsGetTopTask")]
pub extern "C" fn priority_queue_get_top_task() -> *mut TaskCB {
    // 原子读取优先级位图
    let bitmap = PRI_QUEUE_BITMAP.load(Ordering::Acquire);
    let mut top_task: *mut TaskCB = core::ptr::null_mut();

    if bitmap != 0 {
        // 计算最高优先级（前导零的数量）
        let priority = bitmap.leading_zeros();
        unsafe {
            // 获取该优先级队列的第一个任务节点
            let list_head = &mut PRI_QUEUE_LIST[priority as usize];
            let first_node = list_head.next;
            // 通过pendList获取任务控制块
            top_task = TaskCB::from_pend_list(&mut *first_node);
            // 从队列中移除该任务
            priority_queue_remove(&mut *first_node);
        }
    }

    top_task
}

/// 任务重新调度函数
#[unsafe(export_name = "OsSchedResched")]
pub extern "C" fn schedule_reschedule() {
    // TODO
    // assert!(crate::arch::core::arch_int_locked());

    // 检查是否可以进行调度
    if !is_preemptable_in_schedule() {
        return;
    }

    unsafe {
        // 获取当前运行的任务和最高优先级的就绪任务
        let run_task = get_current_task();
        let new_task = priority_queue_get_top_task();

        // 断言必须能获取到一个任务
        assert!(!new_task.is_null(), "无法获取就绪任务");

        // 将新任务从就绪状态改为非就绪状态
        (*new_task).task_status &= !crate::task::OS_TASK_STATUS_READY;

        // 如果新任务和当前运行任务相同，直接返回
        if run_task == new_task {
            return;
        }

        // 更新任务状态
        (*run_task).taskStatus &= !crate::task::OS_TASK_STATUS_RUNNING;
        (*new_task).taskStatus |= crate::task::OS_TASK_STATUS_RUNNING;

        // SMP相关配置
        #[cfg(feature = "kernel_smp")]
        {
            // 标记新运行任务的所属处理器
            (*run_task).currCpu = crate::task::OS_TASK_INVALID_CPUID;
            (*new_task).currCpu = crate::arch::core::arch_current_cpu_id();
        }

        // 更新任务时间统计
        crate::task::hook::task_time_update_hook((*run_task).taskId, crate::tick::get_tick_count());

        // CPU使用率统计
        #[cfg(feature = "kernel_cpup")]
        crate::task::cpup::task_cycle_end_start(new_task);

        // 任务切换监控
        #[cfg(feature = "base_core_tsk_monitor")]
        crate::task::monitor::task_switch_check(run_task, new_task);

        // 调度统计
        #[cfg(feature = "debug_sched_statistics")]
        crate::sched::statistics::sched_statistics(run_task, new_task);

        // 时间片处理
        #[cfg(feature = "base_core_timeslice")]
        if (*new_task).timeSlice == 0 {
            (*new_task).timeSlice = crate::task::KERNEL_TIMESLICE_TIMEOUT;
        }

        // 设置当前任务
        crate::task::current::set_current_task(new_task.cast());

        // 执行任务上下文切换
        crate::arch::dispatch::task_schedule(new_task, run_task);
    }
}

#[inline]
pub fn is_preemptable_in_schedule() -> bool {
    let percpu = crate::percpu::os_percpu_get();
    let preemptable = percpu.task_lock_cnt == 0;

    if !preemptable {
        // 如果不可调度，设置调度标志以便之后处理
        percpu.sched_flag = crate::percpu::SchedFlag::Pending as u32;
    }
    preemptable
}
