use crate::{
    config::{
        STACK_POINT_ALIGN_SIZE, TASK_DEFAULT_STACK_SIZE, TASK_MIN_STACK_SIZE, TASK_PRIORITY_LOWEST,
    },
    ffi::bindings::task_stack_init,
    interrupt::{disable_interrupts, restore_interrupt_state},
    mem::{
        defs::{m_aucSysMem0, os_sys_mem_size},
        memory::los_mem_alloc_align,
    },
    result::{SystemError, SystemResult},
    task::{
        error::TaskError,
        global::{FREE_TASK_LIST, get_tcb_from_id, is_scheduler_active},
        sched::{priority_queue_insert_at_back, schedule},
        types::{TaskCB, TaskInitParam, TaskStatus},
    },
    utils::{
        align::{align_up, is_aligned},
        list::LinkedList,
    },
};
use core::ffi::c_void;

fn check_task_init_param(init_param: &TaskInitParam) -> SystemResult<()> {
    // 检查任务名称是否为空
    if init_param.name.is_null() {
        return Err(SystemError::Task(TaskError::NameEmpty));
    }
    // 检查任务入口函数是否为空
    if init_param.task_entry.is_none() {
        return Err(SystemError::Task(TaskError::EntryNull));
    }
    // 检查任务优先级是否有效
    if init_param.priority > TASK_PRIORITY_LOWEST {
        return Err(SystemError::Task(TaskError::PriorityError));
    }
    Ok(())
}

fn check_task_create_param_dynamic(init_param: &mut TaskInitParam) -> SystemResult<()> {
    // 检查初始化参数
    check_task_init_param(init_param)?;

    // 获取内存池信息
    let pool_size = os_sys_mem_size();

    // 检查栈大小是否超过内存池大小
    if init_param.stack_size > pool_size as u32 {
        return Err(SystemError::Task(TaskError::StackSizeTooLarge));
    }

    // 如果栈大小为0，设置默认大小
    if init_param.stack_size == 0 {
        init_param.stack_size = TASK_DEFAULT_STACK_SIZE;
    }

    // 对齐栈大小
    init_param.stack_size = align_up(init_param.stack_size, STACK_POINT_ALIGN_SIZE);

    // 检查栈大小是否太小
    if init_param.stack_size < TASK_MIN_STACK_SIZE {
        return Err(SystemError::Task(TaskError::StackSizeTooSmall));
    }

    Ok(())
}

#[cfg(feature = "task_static_allocation")]
fn check_task_create_param_static(
    init_param: &TaskInitParam,
    top_stack: *mut c_void,
) -> SystemResult<()> {
    // 基础参数检查
    check_task_init_param(init_param)?;

    // 检查栈顶指针
    if top_stack.is_null() {
        return Err(SystemError::Task(TaskError::ParamNull));
    }

    // 检查栈顶指针是否对齐
    if !is_aligned(top_stack as u32, STACK_POINT_ALIGN_SIZE) {
        return Err(SystemError::Task(TaskError::StackNotAligned));
    }

    // 检查栈大小是否对齐
    if !is_aligned(init_param.stack_size, STACK_POINT_ALIGN_SIZE) {
        return Err(SystemError::Task(TaskError::StackNotAligned));
    }

    // 检查栈大小是否太小
    if init_param.stack_size < TASK_MIN_STACK_SIZE {
        return Err(SystemError::Task(TaskError::StackSizeTooSmall));
    }

    Ok(())
}

fn allocate_task_stack(stack_size: u32) -> SystemResult<*mut core::ffi::c_void> {
    let pool = unsafe { m_aucSysMem0 as *mut core::ffi::c_void };
    let top_stack = los_mem_alloc_align(pool, stack_size, STACK_POINT_ALIGN_SIZE);
    if top_stack.is_null() {
        return Err(SystemError::Task(TaskError::OutOfMemory));
    }
    Ok(top_stack)
}

fn get_free_task_cb() -> SystemResult<&'static mut TaskCB> {
    // 检查空闲任务列表是否为空
    if LinkedList::is_empty(&raw const FREE_TASK_LIST) {
        return Err(SystemError::Task(TaskError::NoFreeTasks));
    }
    // 获取第一个空闲任务控制块
    let first_node = LinkedList::first(&raw const FREE_TASK_LIST);
    let task_cb = TaskCB::from_pend_list(first_node);
    LinkedList::remove(first_node);
    Ok(task_cb)
}

/// 初始化任务控制块
fn init_task_cb(
    task_cb: &mut TaskCB,
    init_param: &TaskInitParam,
    stack_ptr: *mut c_void,
    top_stack: *mut c_void,
    use_usr_stack: bool,
) {
    // 基本信息设置
    task_cb.stack_pointer = stack_ptr;
    task_cb.args = init_param.args;
    task_cb.top_of_stack = top_stack;
    task_cb.stack_size = init_param.stack_size;

    #[cfg(feature = "compat_posix")]
    {
        task_cb.thread_join = core::ptr::null_mut();
        task_cb.thread_join_retval = core::ptr::null_mut();
    }

    // 任务状态和优先级
    task_cb.task_status = TaskStatus::SUSPEND;
    task_cb.priority = init_param.priority;
    task_cb.priority_bitmap = 0;
    task_cb.task_entry = init_param.task_entry;

    #[cfg(feature = "ipc_event")]
    {
        LinkedList::init(&raw mut task_cb.event.wait_list);
        task_cb.event.event_id = 0;
        task_cb.event_mask = 0;
    }

    // 任务名称和消息
    task_cb.task_name = init_param.name;
    task_cb.msg = core::ptr::null_mut();

    // 设置任务标志
    task_cb.clear_all_flags();
    task_cb.set_detached(init_param.is_detached());

    // 栈类型标志：0-动态分配栈空间；1-用户提供栈空间
    task_cb.usr_stack = if use_usr_stack { 1 } else { 0 };

    // 信号
    task_cb.clear_all_signals();

    // 时间片相关
    #[cfg(feature = "time_slice")]
    {
        task_cb.time_slice = 0;
    }

    // 调度统计相关
    #[cfg(feature = "debug_sched_statistics")]
    {
        // 清零调度统计信息
        unsafe {
            core::ptr::write_bytes(
                &mut task_cb.sched_stat as *mut _ as *mut u8,
                0,
                core::mem::size_of::<SchedStat>(),
            );
        }
    }
}

/// 仅创建任务（不启动）
fn task_create_internal(
    task_id: &mut u32,
    init_param: &mut TaskInitParam,
    top_stack: *mut c_void,
    use_usr_stack: bool,
) -> SystemResult<()> {
    // 参数检查
    #[cfg(feature = "task_static_allocation")]
    if use_usr_stack {
        check_task_create_param_static(init_param, top_stack)?;
    } else {
        check_task_create_param_dynamic(init_param)?;
    }

    #[cfg(not(feature = "task_static_allocation"))]
    check_task_create_param_dynamic(init_param)?;

    // 获取空闲任务控制块
    let int_save = disable_interrupts();
    let task_cb = match get_free_task_cb() {
        Ok(tcb) => {
            restore_interrupt_state(int_save);
            tcb
        }
        Err(err) => {
            restore_interrupt_state(int_save);
            return Err(err);
        }
    };

    // 栈分配和初始化
    let top_stack_ptr = if use_usr_stack {
        // 使用用户提供的栈
        top_stack
    } else {
        // 动态分配栈
        match allocate_task_stack(init_param.stack_size) {
            Ok(stack_ptr) => stack_ptr,
            Err(err) => {
                // 分配失败，需要将任务控制块归还到空闲列表
                let int_save = disable_interrupts();
                LinkedList::insert(&raw mut FREE_TASK_LIST, &raw mut task_cb.pend_list);
                restore_interrupt_state(int_save);
                return Err(err);
            }
        }
    };

    // 初始化栈
    let stack_ptr = task_stack_init(task_cb.task_id, init_param.stack_size, top_stack_ptr);

    // 初始化任务控制块
    init_task_cb(task_cb, init_param, stack_ptr, top_stack_ptr, use_usr_stack);
    *task_id = task_cb.task_id;
    Ok(())
}

fn task_resume(task_id: u32) {
    // 根据任务ID获取任务控制块
    let task_cb = get_tcb_from_id(task_id);
    // 加锁进行原子操作
    let int_save = disable_interrupts();

    task_cb.task_status.remove(TaskStatus::SUSPEND);
    task_cb.task_status.insert(TaskStatus::READY);

    priority_queue_insert_at_back(&mut task_cb.pend_list, task_cb.priority as u32);

    restore_interrupt_state(int_save);

    if is_scheduler_active() {
        schedule();
    }
}

/// 仅创建静态任务
#[cfg(feature = "task_static_allocation")]
#[inline]
pub fn task_create_only_static(
    task_id: &mut u32,
    init_param: &mut TaskInitParam,
    top_stack: *mut c_void,
) -> SystemResult<()> {
    task_create_internal(task_id, init_param, top_stack, true)
}

/// 创建并启动静态任务
#[cfg(feature = "task_static_allocation")]
#[inline]
pub fn task_create_static(
    task_id: &mut u32,
    init_param: &mut TaskInitParam,
    top_stack: *mut c_void,
) -> SystemResult<()> {
    // 首先创建任务
    task_create_only_static(task_id, init_param, top_stack)?;
    task_resume(*task_id);
    Ok(())
}

#[inline]
pub fn task_create_only(task_id: &mut u32, init_param: &mut TaskInitParam) -> SystemResult<()> {
    task_create_internal(task_id, init_param, core::ptr::null_mut(), false)
}

#[inline]
pub fn task_create(task_id: &mut u32, init_param: &mut TaskInitParam) -> SystemResult<()> {
    // 首先创建任务
    task_create_only(task_id, init_param)?;
    // 启动任务
    // unsafe {
    //     crate::utils::printf::dprintf(
    //         b"Creating task [%d]: %s\n\0" as *const core::ffi::c_char,
    //         *task_id,
    //         init_param.name,
    //     )
    // };
    task_resume(*task_id);
    Ok(())
}
