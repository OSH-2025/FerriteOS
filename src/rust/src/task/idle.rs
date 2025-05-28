use crate::{
    config::{TASK_IDLE_STACK_SIZE, TASK_PRIORITY_LOWEST},
    ffi::bindings::wfi,
    hwi::{int_lock, int_restore},
    mem::{defs::m_aucSysMem0, memory::los_mem_free},
    percpu::os_percpu_get,
    task::{
        global::{FREE_TASK_LIST, TASK_RECYCLE_LIST, get_tcb_mut},
        lifecycle::create::task_create,
        types::{TaskCB, TaskEntryFunc, TaskError, TaskFlags, TaskInitParam},
    },
    utils::list::LinkedList,
};
use core::mem::transmute;

fn los_task_recycle() {
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

fn idle_task() {
    loop {
        los_task_recycle();
        wfi();
    }
}

// TODO 移除 extern "C" 函数
#[unsafe(export_name = "OsGetIdleTaskId")]
pub extern "C" fn get_idle_task_id() -> u32 {
    // 获取当前CPU的percpu结构，返回空闲任务ID
    let percpu = os_percpu_get();
    percpu.idle_task_id
}

pub fn idle_task_create() -> Result<(), TaskError> {
    // 初始化任务参数
    let mut task_init_param = TaskInitParam {
        task_entry: unsafe { transmute::<_, TaskEntryFunc>(idle_task as usize) },
        priority: TASK_PRIORITY_LOWEST,
        stack_size: TASK_IDLE_STACK_SIZE,
        name: b"IdleCore000\0".as_ptr(),
        ..Default::default()
    };

    // 获取当前CPU的percpu结构
    let percpu = os_percpu_get();
    let idle_task_id = &mut percpu.idle_task_id;

    // 创建任务
    task_create(idle_task_id, &mut task_init_param)?;

    // 如果创建成功，设置系统任务标志
    let task_cb = get_tcb_mut(*idle_task_id);
    task_cb.task_flags.insert(TaskFlags::SYSTEM);

    Ok(())
}
