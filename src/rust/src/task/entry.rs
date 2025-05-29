use core::ffi::c_void;

use crate::task::types::{TaskCB, TaskFlags};

/// 检查任务是否是分离式的
#[inline]
fn is_task_detached(task_cb: &TaskCB) -> bool {
    #[cfg(feature = "compat_posix")]
    {
        task_cb.task_flags.contains(TaskFlags::DETACHED)
    }

    #[cfg(not(feature = "compat_posix"))]
    {
        true
    }
}

/// 删除分离式任务
fn delete_detached_task(task_cb: *const TaskCB) {
    // 禁用中断
    let int_save = int_lock();

    // 清除任务锁定计数
    unsafe {
        (*os_percpu_get()).task_lock_cnt = 0;
    }

    // 恢复中断
    int_restore(int_save);

    // 删除任务
    unsafe {
        let _ = crate::task::lifecycle::task_delete((*task_cb).task_id);
    }
}

/// 处理可连接任务的删除
///
/// # Arguments
/// * `task_cb` - 任务控制块指针
/// * `ret` - 任务返回值
pub fn handle_joined_task_exit(task_cb: *mut TaskCB, ret: *mut c_void) {
    #[cfg(feature = "compat_posix")]
    unsafe {
        use crate::semaphore::los_sem_post;

        // 保存线程返回值
        (*task_cb).thread_join_retval = ret;

        // 禁用中断
        let int_save = int_lock();

        // 设置任务锁定计数
        (*os_percpu_get()).task_lock_cnt = 1;

        // 如果有线程在等待这个任务结束，发送信号量
        if !(*task_cb).thread_join.is_null() {
            let sem_cb = (*task_cb).thread_join as *mut crate::semaphore::types::SemCB;
            if los_sem_post((*sem_cb).sem_id) != OK {
                println!("OsTaskEntry LOS_SemPost fail!");
            }
            (*task_cb).thread_join = core::ptr::null_mut();
        }

        // 清除任务锁定计数
        (*os_percpu_get()).task_lock_cnt = 0;

        // 获取任务自旋锁，进行重新调度
        spin_lock(&TASK_SPINLOCK);
        os_sched_resched();
        spin_unlock(&TASK_SPINLOCK);

        // 恢复中断
        int_restore(int_save);
    }
}

/// 处理正在运行任务的删除操作
///
/// # Arguments
/// * `task_cb` - 要删除的任务控制块指针
pub fn task_del_action_on_run(task_cb: *mut TaskCB) {
    unsafe {
        // 获取特殊任务控制块（用于保存正在运行任务的信息）
        let run_task = &mut TASK_CB_ARRAY[KERNEL_TSK_LIMIT as usize];

        // 转移任务信息
        run_task.task_id = (*task_cb).task_id;
        run_task.task_status = (*task_cb).task_status;
        run_task.top_of_stack = (*task_cb).top_of_stack;
        run_task.task_name = (*task_cb).task_name;

        // 标记原任务为未使用
        (*task_cb).task_status = TaskStatus::UNUSED;
    }
}

/// 任务入口函数
///
/// # Arguments
/// * `task_id` - 任务ID
///
/// # Safety
/// 这是一个底层函数，通常由系统自动调用
#[no_mangle]
pub unsafe extern "C" fn OsTaskEntry(task_id: u32) {
    debug_assert!(task_id < KERNEL_TSK_LIMIT);

    // 释放任务自旋锁并使能中断
    spin_unlock(&TASK_SPINLOCK);
    let _ = int_unlock();

    // 获取任务控制块
    let task_cb = get_tcb_from_id(task_id);
    let mut ret: *mut c_void = core::ptr::null_mut();

    // 调用任务入口函数
    #[cfg(feature = "obsolete_api")]
    {
        ret = (*task_cb).task_entry.unwrap()(
            (*task_cb).args[0],
            (*task_cb).args[1],
            (*task_cb).args[2],
            (*task_cb).args[3],
        );
    }

    #[cfg(not(feature = "obsolete_api"))]
    {
        ret = (*task_cb).task_entry.unwrap()((*task_cb).args);
    }

    // 根据任务类型执行相应的删除操作
    if task_delete_check_detached(task_cb) {
        task_delete_detached(task_cb);
    } else {
        task_delete_joined(task_cb, ret);
    }
}

/// C兼容函数导出
#[cfg(feature = "compat_posix")]
pub mod c_compat {
    use super::*;

    #[no_mangle]
    pub extern "C" fn OsTaskDeleteCheckDetached(task_cb: *const TaskCB) -> bool {
        task_delete_check_detached(task_cb)
    }

    #[no_mangle]
    pub extern "C" fn OsTaskDeleteDetached(task_cb: *const TaskCB) {
        task_delete_detached(task_cb)
    }

    #[no_mangle]
    pub extern "C" fn OsTaskDeleteJoined(task_cb: *mut TaskCB, ret: *mut c_void) {
        task_delete_joined(task_cb, ret)
    }

    #[no_mangle]
    pub extern "C" fn OsTaskDelActionOnRun(task_cb: *mut TaskCB) {
        task_del_action_on_run(task_cb)
    }
}
