/// 检查是否为异常交互任务
#[cfg(feature = "exc_interaction")]
#[unsafe(export_name = "OsCheckExcInteractionTask")]
pub extern "C" fn os_check_exc_interaction_task(init_param: *const c_void) -> u32 {
    extern "C" {
        fn ShellTask() -> !;
        fn ShellEntry() -> !;
        fn OsIdleTask() -> !;
    }

    // 这里需要访问C结构体的字段，这取决于TSK_INIT_PARAM_S的具体定义
    // 以下代码基于假设struct TSK_INIT_PARAM_S { pfnTaskEntry: fn() -> !, ... }
    struct TaskInitParam {
        pfn_task_entry: unsafe extern "C" fn() -> !,
        // ... 其他字段
    }

    let param = unsafe { &*(init_param as *const TaskInitParam) };

    if param.pfn_task_entry as usize == ShellTask as usize
        || param.pfn_task_entry as usize == ShellEntry as usize
        || param.pfn_task_entry as usize == OsIdleTask as usize
    {
        return 0; // LOS_OK
    }

    1 // LOS_NOK
}

/// 保持异常交互任务
/// 注意：这个特性无法打开，故不用管这个函数了！
#[cfg(feature = "exc_interaction")]
#[unsafe(export_name = "OsKeepExcInteractionTask")]
pub extern "C" fn os_keep_exc_interaction_task() {
    pub const OS_TASK_STATUS_UNUSED: u16 = 0x0001; // 任务未使用状态，对应于C代码中的0x0001U

    extern "C" {
        fn OsIrqNestingCntSet(cnt: u32);
        fn IsIdleTask(task_id: u32) -> bool;
        fn IsShellTask(task_id: u32) -> bool;
        fn IsSwtmrTask(task_id: u32) -> bool;
        fn LOS_TaskDelete(task_id: u32) -> u32;
        fn OsHwiInit();
        fn LOS_HwiEnable(hwi_num: u32);
        fn LOS_HwiDisable(hwi_num: u32);
        fn OsIntNumGet() -> u32;
    }

    const NUM_HAL_INTERRUPT_UART: u32 = 32; // UART中断号，需要根据实际情况调整
    const OS_TASK_FLAG_SYSTEM: u32 = 0x0002; // 系统任务标志

    unsafe {
        // 重置中断嵌套计数
        OsIrqNestingCntSet(0);

        // 获取当前最大任务数
        let g_task_max_num = {
            extern "C" {
                static g_task_max_num: u32;
            }
            g_task_max_num
        };

        // 删除除当前任务、空闲任务和Shell任务外的所有任务
        for task_id in 0..g_task_max_num {
            let curr_task = OsCurrTaskGet();
            if task_id == (*curr_task).task_id || IsIdleTask(task_id) || IsShellTask(task_id) {
                continue;
            }

            let task_cb = crate::task::OS_TCB_FROM_TID(task_id);
            if (*task_cb).task_status & OS_TASK_STATUS_UNUSED != 0 {
                continue;
            }

            if IsSwtmrTask(task_id) {
                (*task_cb).task_flags_usr_stack &= !OS_TASK_FLAG_SYSTEM;
            }

            LOS_TaskDelete(task_id);
        }

        // 重新初始化硬件中断
        OsHwiInit();
        LOS_HwiEnable(NUM_HAL_INTERRUPT_UART);

        // 禁用当前中断并删除当前任务
        let cur_irq_num = OsIntNumGet();
        LOS_HwiDisable(cur_irq_num);
        LOS_TaskDelete((*OsCurrTaskGet()).task_id);

        // 不应该到达这里
        loop {}
    }
}
