use crate::{
    config::{TASK_IDLE_STACK_SIZE, TASK_PRIORITY_LOWEST},
    ffi::bindings::wfi,
    interrupt::{disable_interrupts, restore_interrupt_state},
    memory::free,
    percpu::os_percpu_get,
    result::SystemResult,
    task::{
        global::{FREE_TASK_LIST, TASK_RECYCLE_LIST, get_tcb_from_id},
        manager::create::task_create,
        types::{TaskCB, TaskInitParam},
    },
    utils::list::LinkedList,
};
use core::ffi::c_void;

fn los_task_recycle() {
    let int_save = disable_interrupts();
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
            free(task_cb.top_of_stack as *mut c_void);
            // 重置栈顶指针
            task_cb.top_of_stack = core::ptr::null_mut();
        }
    }
    restore_interrupt_state(int_save);
}

extern "C" fn idle_task(_arg: *mut c_void) {
    loop {
        los_task_recycle();
        wfi();
    }
}

pub fn idle_task_create() -> SystemResult<()> {
    // 初始化任务参数
    let mut task_init_param = TaskInitParam {
        task_entry: Some(idle_task),
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
    let task_cb = get_tcb_from_id(*idle_task_id);
    task_cb.set_system_task();

    Ok(())
}
