use crate::{
    ffi::bindings::wfi,
    hwi::{int_lock, int_restore},
    mem::{defs::m_aucSysMem0, memory::los_mem_free},
    percpu::os_percpu_get,
    task::{
        global::{FREE_TASK_LIST, TASK_RECYCLE_LIST},
        types::TaskCB,
    },
    utils::list::LinkedList,
};

#[unsafe(export_name = "LOS_TaskResRecycle")]
pub extern "C" fn los_task_recycle() {
    let int_save = int_lock();
    while !LinkedList::is_empty(&raw mut TASK_RECYCLE_LIST) {
        // 获取回收列表中的第一个任务控制块
        unsafe {
            let first_node = TASK_RECYCLE_LIST.next;

            // 从回收列表中移除任务控制块
            LinkedList::remove(first_node);

            // 获取包含此链表节点的任务控制块
            let task_cb = TaskCB::from_pend_list(first_node);

            // 将任务控制块添加到空闲列表
            LinkedList::insert(&raw mut FREE_TASK_LIST, &mut task_cb.pend_list);
            los_mem_free(
                m_aucSysMem0 as *mut core::ffi::c_void,
                task_cb.top_of_stack as *mut core::ffi::c_void,
            );
            // 重置栈顶指针
            task_cb.top_of_stack = core::ptr::null_mut();
        }
    }
    int_restore(int_save);
}

#[unsafe(export_name = "OsIdleTask")]
pub extern "C" fn idle_task() {
    loop {
        los_task_recycle();
        // let hook = core::mem::transmute::<_, extern "C" fn()>(IDLE_HANDLER_HOOK);
        wfi();
    }
}

#[unsafe(export_name = "OsGetIdleTaskId")]
pub extern "C" fn get_idle_task_id() -> u32 {
    // 获取当前CPU的percpu结构，返回空闲任务ID
    let percpu = os_percpu_get();
    percpu.idle_task_id
}

// #[unsafe(export_name = "OsIdleTaskCreate")]
// pub extern "C" fn idle_task_create() -> u32 {
//     // 初始化任务参数
//     let mut task_init_param = TaskInitParam {
//         pfn_task_entry: Some(
//             idle_task as extern "C" fn(*mut core::ffi::c_void) -> *mut core::ffi::c_void,
//         ),
//         task_prio: crate::config::OS_TASK_PRIORITY_LOWEST,
//         p_args: core::ptr::null_mut(),
//         stack_size: crate::config::KERNEL_TSK_IDLE_STACK_SIZE,
//         name: b"IdleCore000\0".as_ptr(),
//         resved: 0,
//     };

//     // 获取当前CPU的percpu结构
//     let percpu = os_percpu_get();
//     let idle_task_id = &mut percpu.idle_task_id;

//     // 创建任务
//     let ret = unsafe { os_task_create(idle_task_id, &mut task_init_param) };

//     // 如果创建成功，设置系统任务标志
//     if ret == 0 {
//         unsafe {
//             let task_cb = task_cb_from_tid(*idle_task_id);
//             (*task_cb).task_flags |= OS_TASK_FLAG_SYSTEM;
//         }
//     }

//     ret
// }
