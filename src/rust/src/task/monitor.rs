use crate::task::types::TaskCB;

/// 任务切换钩子函数类型
pub type TaskSwitchHook = Option<extern "C" fn()>;

/// 用户定义的任务切换钩子
static mut USER_TASK_SWITCH_HOOK: TaskSwitchHook = None;

#[inline]
fn stack_magic_check(top_stack: *const usize) -> bool {
    const STACK_MAGIC_WORD: usize = 0xCCCCCCCC;
    unsafe { *top_stack == STACK_MAGIC_WORD }
}

/// 检查任务栈是否溢出或栈指针是否有效
fn check_task_stack(old_task: &TaskCB, new_task: &TaskCB) {
    if !stack_magic_check(old_task.top_of_stack as *const usize) {
        panic!(
            "current task id: {}:{} stack overflow! StackPointer: {:p} TopOfStack: {:p}\n",
            old_task.name(),
            old_task.task_id,
            old_task.stack_pointer,
            old_task.top_of_stack
        );
    }

    // 检查新任务的栈指针是否在有效范围内
    if (new_task.stack_pointer as usize <= new_task.top_of_stack as usize)
        || (new_task.stack_pointer as usize
            > (new_task.top_of_stack as usize + new_task.stack_size as usize))
    {
        panic!(
            "highest task ID: {}:{} SP error! StackPointer: {:p} TopOfStack: {:p}\n",
            new_task.name(),
            new_task.task_id,
            new_task.stack_pointer,
            new_task.top_of_stack
        );
    }
}

/// 初始化任务监控模块
pub fn init_task_monitor() {}

/// 注册任务切换钩子函数
pub fn register_task_switch_hook(hook: TaskSwitchHook) {
    unsafe { USER_TASK_SWITCH_HOOK = hook };
}

/// 执行任务切换检查
pub fn check_task_switch(old_task: &TaskCB, new_task: &TaskCB) {
    // 检查任务栈
    check_task_stack(old_task, new_task);

    unsafe {
        match USER_TASK_SWITCH_HOOK {
            Some(hook) => hook(),
            None => {}
        }
    }
}
