use super::{types::TaskCB, types::TaskStatus};
use crate::{
    ffi::bindings::{arch_int_locked, curr_task_get, curr_task_set, os_task_schedule},
    hwi::{int_lock, int_restore},
    utils::list::LinkedList,
};
use core::sync::atomic::{AtomicU32, Ordering};

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
    assert!(arch_int_locked());
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
    assert!(arch_int_locked());

    // 检查是否可以进行调度
    if !is_preemptable_in_schedule() {
        return;
    }

    unsafe {
        // 获取当前运行的任务和最高优先级的就绪任务
        let run_task = curr_task_get();
        let new_task = priority_queue_get_top_task();

        // 断言必须能获取到一个任务
        assert!(!new_task.is_null(), "无法获取就绪任务");

        (*new_task).task_status.remove(TaskStatus::READY);
        if run_task == new_task {
            return;
        }

        // 更新任务状态
        (*run_task).task_status.remove(TaskStatus::RUNNING);
        (*new_task).task_status.insert(TaskStatus::RUNNING);
        // TODO
        // OsTaskTimeUpdateHook(runTask->taskId, LOS_TickCountGet());
        // OsTaskSwitchCheck(runTask, newTask);
        // OsSchedStatistics(runTask, newTask);

        if (*new_task).time_slice == 0 {
            (*new_task).time_slice = crate::config::KERNEL_TIMESLICE_TIMEOUT;
        }

        // 设置当前任务
        curr_task_set(new_task);

        // 执行任务上下文切换
        os_task_schedule(new_task, run_task);
    }
}

#[inline]
fn is_preemptable_in_schedule() -> bool {
    let percpu = crate::percpu::os_percpu_get();
    let preemptable = percpu.task_lock_cnt == 0;
    if !preemptable {
        // 如果不可调度，设置调度标志以便之后处理
        percpu.sched_flag = crate::percpu::SchedFlag::Pending as u32;
    }
    preemptable
}

#[inline]
fn is_preemptable() -> bool {
    let int_save = int_lock();
    let preemptable = is_preemptable_in_schedule();
    int_restore(int_save);
    preemptable
}

#[unsafe(export_name = "OsSchedPreempt")]
pub extern "C" fn schedule_preempt() {
    // 检查是否可以进行抢占
    if !is_preemptable() {
        return;
    }

    // 获取调度器锁
    let int_save = int_lock();
    unsafe {
        // 将当前任务添加回就绪队列
        let run_task = curr_task_get();
        (*run_task).task_status.insert(TaskStatus::READY);

        // 根据时间片情况，选择插入队列的方式
        if (*run_task).time_slice == 0 {
            priority_queue_insert_at_back(&mut (*run_task).pend_list, (*run_task).priority as u32);
        } else {
            priority_queue_insert_at_front(&mut (*run_task).pend_list, (*run_task).priority as u32);
        }
        // 调度到新线程
        crate::task::sched::schedule_reschedule();
    }

    int_restore(int_save);
}

#[unsafe(export_name = "OsTimesliceCheck")]
pub extern "C" fn timeslice_check() {
    unsafe {
        // 获取当前运行的任务
        let run_task = curr_task_get();

        // 检查时间片是否需要递减
        if (*run_task).time_slice != 0 {
            (*run_task).time_slice -= 1;
            if (*run_task).time_slice == 0 {
                schedule();
            }
        }
    }
}

/// 触发任务调度
#[inline]
pub(crate) fn schedule() {
    // 检查是否在中断上下文中
    if crate::hwi::is_int_active() {
        let percpu = crate::percpu::os_percpu_get();
        percpu.sched_flag = crate::percpu::SchedFlag::Pending as u32;
        return;
    }
    schedule_preempt();
}
