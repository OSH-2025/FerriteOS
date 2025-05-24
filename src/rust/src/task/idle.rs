use super::{
    TaskCB,
    global::{TASK_FREE_LIST, TASK_RECYCLE_LIST},
};
use crate::{
    hwi::{los_int_lock, los_int_restore},
    mem::{defs::m_aucSysMem0, memory::los_mem_free},
    utils::list::LinkedList,
};

#[unsafe(export_name = "LOS_TaskResRecycle")]
pub extern "C" fn los_task_recycle() {
    let int_save = los_int_lock();
    while !LinkedList::is_empty(&raw mut TASK_RECYCLE_LIST) {
        // 获取回收列表中的第一个任务控制块
        let first_node = unsafe { TASK_RECYCLE_LIST.next };

        // 从回收列表中移除任务控制块
        LinkedList::remove(first_node);

        // 获取包含此链表节点的任务控制块
        let task_cb = TaskCB::from_pend_list(first_node);

        // 将任务控制块添加到空闲列表
        LinkedList::insert(&raw mut TASK_FREE_LIST, &mut task_cb.pend_list);
        unsafe {
            los_mem_free(
                m_aucSysMem0 as *mut core::ffi::c_void,
                task_cb.top_of_stack as *mut core::ffi::c_void,
            );
        }

        // 重置栈顶指针
        task_cb.top_of_stack = core::ptr::null_mut();
    }
    los_int_restore(int_save);
}
