//! 异常处理模块 - LiteOS异常处理机制的Rust实现

use semihosting::println;

use crate::{
    config::TASK_LIMIT,
    ffi::bindings::{arch_back_trace, arch_back_trace_with_sp},
    task::{global::get_tcb_from_id, types::TaskStatus},
};

/// 异常信息转储格式结构
#[cfg(feature = "shell_excinfo_dump")]
#[repr(C)]
pub struct ExcInfoDumpFormat {
    /// 存储异常信息的缓冲区指针
    buf: *mut u8,
    /// 异常信息缓冲区的偏移量
    offset: u32,
    /// 存储异常信息的大小
    len: u32,
    /// 存储异常信息的地址
    dump_addr: usize,
}

/// 日志读写函数类型
#[cfg(feature = "shell_excinfo_dump")]
pub type LogReadWriteFunc =
    unsafe extern "C" fn(addr: usize, len: u32, is_read: i32, buf: *mut u8) -> i32;

#[cfg(feature = "shell_excinfo_dump")]
static mut G_EXC_INFO_POOL: ExcInfoDumpFormat = ExcInfoDumpFormat {
    buf: core::ptr::null_mut(),
    offset: 0xFFFFFFFF, // 初始化为MAX，在异常处理程序中发生时分配为0
    len: 0,
    dump_addr: 0,
};

/// 异常信息读写钩子函数
#[cfg(feature = "shell_excinfo_dump")]
static mut G_DUMP_HOOK: Option<LogReadWriteFunc> = None;

/// 注册异常信息钩子
///
/// # 参数
///
/// * `start_addr` - 起始地址
/// * `len` - 长度
/// * `buf` - 缓冲区指针
/// * `hook` - 钩子函数
#[cfg(feature = "shell_excinfo_dump")]
#[unsafe(export_name = "LOS_ExcInfoRegHook")]
pub extern "C" fn exc_info_reg_hook(
    start_addr: usize,
    len: u32,
    buf: *mut u8,
    hook: LogReadWriteFunc,
) {
    if buf.is_null() {
        unsafe {
            PrintErrWrapper(b"Buf or hook is null.\n\0".as_ptr());
        }
        return;
    }

    unsafe {
        G_EXC_INFO_POOL.dump_addr = start_addr;
        G_EXC_INFO_POOL.len = len;
        G_EXC_INFO_POOL.offset = 0xFFFFFFFF; // 初始化为MAX
        G_EXC_INFO_POOL.buf = buf;
        G_DUMP_HOOK = Some(hook);
    }
}

/// 设置异常信息读写函数
#[cfg(feature = "shell_excinfo_dump")]
#[unsafe(export_name = "OsSetExcInfoRW")]
pub extern "C" fn os_set_exc_info_rw(func: LogReadWriteFunc) {
    unsafe {
        G_DUMP_HOOK = Some(func);
    }
}

/// 获取异常信息读写函数
#[cfg(feature = "shell_excinfo_dump")]
#[unsafe(export_name = "OsGetExcInfoRW")]
pub extern "C" fn os_get_exc_info_rw() -> Option<LogReadWriteFunc> {
    unsafe { G_DUMP_HOOK }
}

/// 设置异常信息缓冲区
#[cfg(feature = "shell_excinfo_dump")]
#[unsafe(export_name = "OsSetExcInfoBuf")]
pub extern "C" fn os_set_exc_info_buf(buf: *mut u8) {
    unsafe {
        G_EXC_INFO_POOL.buf = buf;
    }
}

/// 获取异常信息缓冲区
#[cfg(feature = "shell_excinfo_dump")]
#[unsafe(export_name = "OsGetExcInfoBuf")]
pub extern "C" fn os_get_exc_info_buf() -> *mut u8 {
    unsafe { G_EXC_INFO_POOL.buf }
}

/// 设置异常信息偏移量
#[cfg(feature = "shell_excinfo_dump")]
#[unsafe(export_name = "OsSetExcInfoOffset")]
pub extern "C" fn os_set_exc_info_offset(offset: u32) {
    unsafe {
        G_EXC_INFO_POOL.offset = offset;
    }
}

/// 获取异常信息偏移量
#[cfg(feature = "shell_excinfo_dump")]
#[unsafe(export_name = "OsGetExcInfoOffset")]
pub extern "C" fn os_get_exc_info_offset() -> u32 {
    unsafe { G_EXC_INFO_POOL.offset }
}

/// 设置异常信息转储地址
#[cfg(feature = "shell_excinfo_dump")]
#[unsafe(export_name = "OsSetExcInfoDumpAddr")]
pub extern "C" fn os_set_exc_info_dump_addr(addr: usize) {
    unsafe {
        G_EXC_INFO_POOL.dump_addr = addr;
    }
}

/// 获取异常信息转储地址
#[cfg(feature = "shell_excinfo_dump")]
#[unsafe(export_name = "OsGetExcInfoDumpAddr")]
pub extern "C" fn os_get_exc_info_dump_addr() -> usize {
    unsafe { G_EXC_INFO_POOL.dump_addr }
}

/// 设置异常信息长度
#[cfg(feature = "shell_excinfo_dump")]
#[unsafe(export_name = "OsSetExcInfoLen")]
pub extern "C" fn os_set_exc_info_len(len: u32) {
    unsafe {
        G_EXC_INFO_POOL.len = len;
    }
}

/// 获取异常信息长度
#[cfg(feature = "shell_excinfo_dump")]
#[unsafe(export_name = "OsGetExcInfoLen")]
pub extern "C" fn os_get_exc_info_len() -> u32 {
    unsafe { G_EXC_INFO_POOL.len }
}

/// WriteExcBufVa - 写入异常信息到缓冲区（处理变参列表）
/// WriteExcInfoToBuf - 格式化异常信息并写入缓冲区（C接口）
#[cfg(feature = "shell_excinfo_dump")]
unsafe extern "C" {
    fn WriteExcBufVa(format: *const u8, arglist: *const c_void);
    fn WriteExcInfoToBuf(format: *const u8, ...);
}

/// 格式化异常信息并写入缓冲区（Rust内部使用）
#[cfg(feature = "shell_excinfo_dump")]
pub fn write_exc_info_to_buf(fmt: &str, args: fmt::Arguments) {
    // 保留您原来的Rust实现，用于内部Rust代码调用
    struct ExcBufWriter;

    impl Write for ExcBufWriter {
        fn write_str(&mut self, s: &str) -> fmt::Result {
            unsafe {
                if G_EXC_INFO_POOL.len > G_EXC_INFO_POOL.offset {
                    let available_len = G_EXC_INFO_POOL.len - G_EXC_INFO_POOL.offset;
                    let bytes_to_write = core::cmp::min(available_len as usize, s.len());

                    if bytes_to_write > 0 {
                        let dest = G_EXC_INFO_POOL.buf.add(G_EXC_INFO_POOL.offset as usize);
                        core::ptr::copy_nonoverlapping(s.as_ptr(), dest, bytes_to_write);
                        G_EXC_INFO_POOL.offset += bytes_to_write as u32;

                        // 确保添加字符串结束符
                        if G_EXC_INFO_POOL.offset < G_EXC_INFO_POOL.len {
                            *G_EXC_INFO_POOL.buf.add(G_EXC_INFO_POOL.offset as usize) = 0;
                        }
                    }
                }
            }
            Ok(())
        }
    }

    let _ = ExcBufWriter.write_fmt(args);
}

/// 写入异常信息到缓冲区（接口函数）
#[cfg(feature = "shell_excinfo_dump")]
#[macro_export]
macro_rules! print_exc_info {
    ($($arg:tt)*) => ({
        use core::fmt::Write;
        $crate::exc::write_exc_info_to_buf("", format_args!($($arg)*));
    })
}

/// 记录异常发生时间
#[cfg(feature = "shell_excinfo_dump")]
#[unsafe(export_name = "OsRecordExcInfoTime")]
pub extern "C" fn os_record_exc_info_time() {
    // 注意：在嵌入式环境中可能需要不同的时间获取方式
    // 这里使用一个简化的实现
    unsafe extern "C" {
        fn time(t: *mut u32) -> u32;
        fn localtime(t: *const u32) -> *mut c_void;
        fn strftime(s: *mut u8, max: usize, format: *const u8, tm: *const c_void) -> usize;
    }

    const NOW_TIME_LENGTH: usize = 24;
    let mut t: u32 = 0;
    let mut now_time: [u8; NOW_TIME_LENGTH] = [0; NOW_TIME_LENGTH];

    unsafe {
        time(&mut t as *mut u32);
        let tm_time = localtime(&t as *const u32);
        if !tm_time.is_null() {
            strftime(
                now_time.as_mut_ptr(),
                NOW_TIME_LENGTH,
                b"%Y-%m-%d %H:%M:%S\0".as_ptr(),
                tm_time,
            );

            print_exc_info!(
                "{} \n",
                core::str::from_utf8_unchecked(&now_time[..NOW_TIME_LENGTH - 1])
            );
        }
    }
}

/// Shell命令：读取异常信息
#[cfg(all(feature = "shell_excinfo_dump", feature = "shell"))]
#[unsafe(export_name = "OsShellCmdReadExcInfo")]
pub extern "C" fn os_shell_cmd_read_exc_info(_argc: i32, _argv: *mut *const u8) -> i32 {
    extern "C" {
        fn LOS_MemAlloc(pool: *mut c_void, size: u32) -> *mut c_void;
        fn LOS_MemFree(pool: *mut c_void, ptr: *mut c_void) -> u32;
        fn memset_s(dest: *mut c_void, dest_max: usize, c: i32, count: usize) -> i32;
        fn dprintf(fmt: *const u8, ...);
    }

    const OS_SYS_MEM_ADDR: usize = 0x20000000; // 系统内存基址，需要根据实际情况调整

    let record_space = os_get_exc_info_len();
    let buf = unsafe { LOS_MemAlloc(OS_SYS_MEM_ADDR as *mut c_void, record_space + 1) as *mut u8 };
    if buf.is_null() {
        return -1; // LOS_NOK
    }

    unsafe {
        memset_s(
            buf as *mut c_void,
            record_space as usize + 1,
            0,
            record_space as usize + 1,
        );

        let hook = os_get_exc_info_rw();
        if let Some(hook_fn) = hook {
            hook_fn(os_get_exc_info_dump_addr(), record_space, 1, buf);
        }

        // 打印信息
        dprintf(b"%s\n\0".as_ptr(), buf);

        LOS_MemFree(OS_SYS_MEM_ADDR as *mut c_void, buf as *mut c_void);
    }

    0 // LOS_OK
}

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

/// 获取当前任务的栈回溯
pub fn back_trace() {
    #[cfg(feature = "backtrace")]
    {
        arch_back_trace();
    }
}

/// 获取指定任务的栈回溯
pub fn task_back_trace(task_id: u32) {
    #[cfg(feature = "backtrace")]
    {
        if task_id >= TASK_LIMIT {
            println!("Task ID is out of range!");
            return;
        }
        let task_cb = get_tcb_from_id(task_id);
        if task_cb.task_status.contains(TaskStatus::UNUSED) {
            println!("The task is not created!");
            return;
        }
        arch_back_trace_with_sp(task_cb.stack_pointer);
    }

    #[cfg(not(feature = "backtrace"))]
    {
        let _ = task_id; // 防止未使用警告
    }
}
