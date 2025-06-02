//! 优先级继承相关功能

use core::ptr::addr_of;

use crate::{
    task::{manager::priority::modify_task_priority_raw, types::TaskCB},
    utils::bitmap::{clear_bit, get_highest_bit, get_lowest_bit, set_bit},
};

use super::types::MutexControlBlock;

/// 优先级继承管理器
pub struct PriorityInheritance;

impl PriorityInheritance {
    /// 处理互斥锁等待时的优先级继承
    pub fn handle_mutex_pend(waiting_task: &mut TaskCB, owner_task: &mut TaskCB) {
        let waiting_priority = waiting_task.priority;
        let owner_priority = owner_task.priority;

        // 如果等待任务的优先级高于所有者，进行优先级继承
        if owner_priority > waiting_priority {
            // 记录原始优先级
            set_bit(&mut owner_task.priority_bitmap, owner_priority);
            // 提升所有者优先级
            modify_task_priority_raw(owner_task, waiting_priority);
        }
    }

    /// 处理互斥锁释放时的优先级恢复
    pub fn handle_mutex_post(
        run_task: &mut TaskCB,
        resumed_task: &TaskCB,
        mutex: &mut MutexControlBlock,
    ) {

        #[cfg(feature = "mutex-waitmode-prio")]
        {
            if resumed_task.priority > run_task.priority {
                // 检查是否需要清除位图中的优先级记录
                if get_highest_bit(run_task.priority_bitmap) != resumed_task.priority {
                    clear_bit(&mut run_task.priority_bitmap, resumed_task.priority);
                }
            } else if run_task.priority_bitmap != 0 {
                Self::restore_priority_complex(run_task, mutex);
            }
        }

        #[cfg(not(feature = "mutex-waitmode-prio"))]
        {
            if run_task.priority_bitmap != 0 {
                Self::restore_priority_complex(run_task, mutex);
            }
        }
    }

    /// 复杂的优先级恢复处理
    fn restore_priority_complex(run_task: &mut TaskCB, mutex: &mut MutexControlBlock) {
        if mutex.has_waiting_tasks() {
            let priority = get_highest_bit(run_task.priority_bitmap);

            // 在中间查找合适位置
            let mut cur_task = TaskCB::from_pend_list(mutex.mux_list.next);

            while addr_of!(cur_task.pend_list) != addr_of!(mutex.mux_list) {
                if priority != cur_task.priority {
                    // 清除不再需要的优先级记录
                    clear_bit(&mut run_task.priority_bitmap, cur_task.priority);
                }
                cur_task = TaskCB::from_pend_list(cur_task.pend_list.next);
            }
        }

        // 恢复到最高的必要优先级
        let priority = get_lowest_bit(run_task.priority_bitmap);
        clear_bit(&mut run_task.priority_bitmap, priority);
        modify_task_priority_raw(mutex.get_owner(), priority);
    }

    /// 恢复任务的原始优先级（在超时时调用）
    pub fn restore_priority_on_timeout(run_task: &mut TaskCB, owner_task: &mut TaskCB) {
        if owner_task.priority >= run_task.priority {
            let priority = get_lowest_bit(owner_task.priority_bitmap);
            if priority != 32 {
                clear_bit(&mut owner_task.priority_bitmap, priority);
                modify_task_priority_raw(owner_task, priority);
            }
        } else {
            // 检查是否需要清除当前任务的优先级记录
            if get_highest_bit(owner_task.priority_bitmap) != run_task.priority {
                clear_bit(&mut owner_task.priority_bitmap, run_task.priority);
            }
        }
    }
}
