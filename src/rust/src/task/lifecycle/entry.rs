// src/task/lifecycle/entry.rs

use crate::{
    interrupt::int_unlock,
    spin::spin_unlock,
    task::{
        lifecycle::delete::{task_delete_check_detached, task_delete_detached, task_delete_joined},
        manager::get_tcb_from_id,
        types::TaskCB,
    },
};

/// 任务入口函数 - 所有任务的公共入口点
///
/// # Arguments
/// * `task_id` - 要运行的任务ID
///
/// # Safety
/// 此函数在任务初始创建时自动调用，不应由用户代码直接调用
pub unsafe fn os_task_entry(task_id: u32) {
    debug_assert!(task_id < crate::config::KERNEL_TSK_LIMIT);

    // 释放任务自旋锁并使能中断
    spin_unlock(&crate::task::global::TASK_SPINLOCK);
    int_unlock();

    // 获取任务控制块
    let task_cb = get_tcb_from_id(task_id);
    let mut ret: *mut core::ffi::c_void = core::ptr::null_mut();

    // 调用实际的任务入口函数
    #[cfg(feature = "obsolete_api")]
    {
        let args = &(*task_cb).args;
        ret = (*task_cb).task_entry.unwrap()(args[0], args[1], args[2], args[3]);
    }
    #[cfg(not(feature = "obsolete_api"))]
    {
        ret = (*task_cb).task_entry.unwrap()((*task_cb).args);
    }

    // 处理任务退出
    if task_delete_check_detached(task_cb) {
        task_delete_detached(task_cb);
    } else {
        task_delete_joined(task_cb, ret);
    }
}

/// C兼容的任务入口函数
#[no_mangle]
pub extern "C" fn OsTaskEntry(task_id: u32) {
    unsafe {
        os_task_entry(task_id);
    }
}
