use crate::{
    percpu::os_percpu_get,
    result::SystemResult,
    timer::{global::TimerPool, types::TimerHandlerItem},
    utils::sortlink::os_sort_link_init,
};

/// 初始化定时器模块
pub fn timer_init() -> SystemResult<()> {
    TimerPool::init();
    // 非ISR模式下的初始化
    #[cfg(not(feature = "timer-in-isr"))]
    {
        // 创建定时器处理队列
        use crate::{config::TIMER_LIMIT, queue::management::create_queue, timer::TimerError};
        match create_queue(TIMER_LIMIT as u16, 4) {
            Ok(queue_id) => os_percpu_get().set_timer_queue_id(queue_id),
            Err(_) => return Err(TimerError::QueueCreateFailed.into()),
        }

        // 创建定时器任务
        match timer_task_create() {
            Ok(_) => {}
            Err(_) => return Err(TimerError::TaskCreateFailed.into()),
        }
    }
    // 初始化排序链表
    os_sort_link_init(&mut os_percpu_get().swtmr_sort_link);
    Ok(())
}

#[cfg(not(feature = "timer-in-isr"))]
extern "C" fn timer_task(_arg: *mut core::ffi::c_void) {
    use crate::queue::operation::queue_read;
    use crate::timer::types::TIMER_HANDLE_ITEM_SIZE;
    use core::{ffi::c_void, ptr::addr_of_mut};

    let mut read_size = TIMER_HANDLE_ITEM_SIZE as u32;
    // 获取当前CPU的软件定时器队列
    let timer_queue_id = os_percpu_get().get_timer_queue_id();
    let mut timer_handler_item = TimerHandlerItem::UNINIT;

    // 无限循环处理软件定时器回调
    loop {
        // 从队列中读取定时器处理项
        let ret = queue_read(
            timer_queue_id,
            addr_of_mut!(timer_handler_item) as *mut c_void,
            &mut read_size,
            u32::MAX,
        );
        // 检查读取结果和读取大小
        if ret.is_ok() && read_size == TIMER_HANDLE_ITEM_SIZE as u32 {
            // 如果处理函数有效，则执行处理函数
            if let Some(handler) = timer_handler_item.handler {
                handler();
            }
        }
    }
}

fn timer_task_create() -> SystemResult<()> {
    use crate::config::TIMER_TASK_STACK_SIZE;
    use crate::task::global::get_tcb_from_id;
    use crate::task::manager::create::task_create;
    use crate::task::types::TaskInitParam;

    let mut timer_task_id: u32 = 0;

    // 创建任务参数结构
    let mut timer_task_init_param = TaskInitParam {
        task_entry: Some(timer_task),
        stack_size: TIMER_TASK_STACK_SIZE,
        name: b"Swt_Task\0".as_ptr(),
        priority: 0,
        ..Default::default()
    };

    // 创建任务
    match task_create(&mut timer_task_id, &mut timer_task_init_param) {
        Ok(_) => {
            os_percpu_get().set_timer_task_id(timer_task_id);
            // 设置系统任务标志
            get_tcb_from_id(timer_task_id).set_system_task();
            Ok(())
        }
        Err(err) => Err(err),
    }
}
