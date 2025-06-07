//! 任务信息命令实现

use crate::exception::backtrace::task_back_trace as os_task_back_trace;
#[cfg(feature = "mem_task_stat")]
use crate::mem::{
    memory::{los_mem_alloc, los_mem_free},
    memstat::os_memstat_task_usage as os_mem_usage,
};
use crate::print_common;
use crate::shellcmd::types::{CmdType, ShellCmd};
use crate::stack::get_stack_waterline;
use crate::task::types::{TaskCB, TaskStatus};
use core::ffi::c_char;
use core::mem::size_of;

// 引入需要的外部函数和变量
unsafe extern "C" {
    static g_taskCBArray: *const TaskCB;
    static m_aucSysMem1: *mut u8;

    fn memcpy_s(
        dest: *mut core::ffi::c_void,
        dest_max: usize,
        src: *const core::ffi::c_void,
        count: usize,
    ) -> i32;
}

// 常量定义
const LOSCFG_BASE_CORE_TSK_LIMIT: u32 = 64;
const KERNEL_TSK_LIMIT: u32 = LOSCFG_BASE_CORE_TSK_LIMIT;
const OS_ALL_TASK_MASK: u32 = 0xFFFFFFFF;
const OS_ERROR: u32 = u32::MAX;
const LOS_OK: u32 = 0;
const LOS_NOK: u32 = 1;
const EOK: i32 = 0;

// 任务状态常量
const OS_TASK_STATUS_UNUSED: TaskStatus = TaskStatus::UNUSED;
const OS_TASK_STATUS_RUNNING: TaskStatus = TaskStatus::RUNNING;
const OS_TASK_STATUS_READY: TaskStatus = TaskStatus::READY;
const OS_TASK_STATUS_DELAY: TaskStatus = TaskStatus::DELAY;
const OS_TASK_STATUS_PEND: TaskStatus = TaskStatus::PEND;
const OS_TASK_STATUS_SUSPEND: TaskStatus = TaskStatus::SUSPEND;
const OS_TASK_STATUS_PEND_TIME: TaskStatus = TaskStatus::PEND_TIME;

// 任务水线数组
static mut G_TASK_WATER_LINE: [u32; LOSCFG_BASE_CORE_TSK_LIMIT as usize] =
    [0; LOSCFG_BASE_CORE_TSK_LIMIT as usize];

/// 将任务状态转换为字符串
fn os_shell_cmd_convert_tsk_status(task_status: TaskStatus) -> &'static str {
    if task_status & OS_TASK_STATUS_RUNNING != TaskStatus::empty() {
        "Running"
    } else if task_status & OS_TASK_STATUS_READY != TaskStatus::empty() {
        "Ready"
    } else {
        // 注意这里使用嵌套结构，与C代码完全匹配
        if task_status & OS_TASK_STATUS_DELAY != TaskStatus::empty() {
            "Delay"
        } else if task_status & OS_TASK_STATUS_PEND_TIME != TaskStatus::empty() {
            if task_status & OS_TASK_STATUS_SUSPEND != TaskStatus::empty() {
                "SuspendTime"
            } else if task_status & OS_TASK_STATUS_PEND != TaskStatus::empty() {
                "PendTime"
            } else {
                "Invalid"
            }
        } else if task_status & OS_TASK_STATUS_PEND != TaskStatus::empty() {
            "Pend"
        } else if task_status & OS_TASK_STATUS_SUSPEND != TaskStatus::empty() {
            "Suspend"
        } else {
            "Invalid"
        }
    }
}

/// 获取任务水线信息
fn os_shell_cmd_task_water_line_get(all_task_array: *const TaskCB) {
    unsafe {
        // 初始化有效任务计数
        let mut valid_tasks = 0;

        for loop_idx in 0..KERNEL_TSK_LIMIT {
            // 跳过明显无效的任务
            if valid_tasks > 32 {
                // 假设系统不会超过32个正常任务
                print_common!("检测到过多可能的有效任务，可能是任务控制块数组已损坏\n");
                break;
            }

            let task_cb = all_task_array.offset(loop_idx as isize);

            // 跳过未使用的任务
            if (*task_cb).task_status & OS_TASK_STATUS_UNUSED != TaskStatus::empty() {
                continue;
            }

            // 获取任务ID
            let task_id = (*task_cb).task_id as usize;
            let task_idx = task_id % (LOSCFG_BASE_CORE_TSK_LIMIT as usize);

            print_common!("Task ID: {}, mapped index: {}\n", task_id, task_idx);

            // 安全计算栈指针地址，防止溢出
            let top_of_stack = (*task_cb).top_of_stack as usize;
            let stack_size = (*task_cb).stack_size as usize;

            // 检查栈大小是否合理
            if stack_size > 0x1000000 {
                // 16MB作为合理的最大栈大小
                print_common!(
                    "警告: 任务 {} 栈大小异常: {}，跳过水线计算\n",
                    task_id,
                    stack_size
                );
                // 设置水线为0
                G_TASK_WATER_LINE[task_idx] = 0;
                continue;
            }

            // 使用checked_add防止溢出
            let bottom_addr = match top_of_stack.checked_add(stack_size) {
                Some(addr) => addr as *const usize,
                None => {
                    print_common!("警告: 任务 {} 栈地址计算溢出，跳过水线计算\n", task_id);
                    // 设置水线为0
                    G_TASK_WATER_LINE[task_idx] = 0;
                    continue;
                }
            };

            let stack_top = top_of_stack as *const usize;

            let water_line_ptr = core::ptr::addr_of_mut!(G_TASK_WATER_LINE[task_idx]) as *mut u32;

            // 增加额外的有效性检查
            if task_id > 100000
                || (*task_cb).priority > 32
                || (stack_size > 0x100000 && stack_size < 0xF0000000)
            {
                continue; // 跳过明显异常的任务
            }

            // 对于看起来有效的任务，递增计数
            valid_tasks += 1;

            // 安全获取栈水线
            match get_stack_waterline(&*stack_top, &*bottom_addr) {
                Ok(value) => *water_line_ptr = value,
                Err(_) => {
                    print_common!("警告: 无法计算任务 {} 水线\n", task_id);
                    *water_line_ptr = 0;
                }
            }
        }
    }
}

/// 打印任务信息表头
fn os_shell_cmd_tsk_info_title() {
    print_common!("\r\nName                   TaskEntryAddr       TID    ");
    print_common!(
        "Priority   Status       StackSize    WaterLine      StackPoint   TopOfStack   EventMask"
    );

    #[cfg(feature = "mem_task_stat")]
    print_common!("   MEMUSE");

    print_common!("\n");
    print_common!("----                   -------------       ---    ");
    print_common!(
        "--------   --------     ---------    ----------     ----------   ----------   ---------"
    );

    #[cfg(feature = "mem_task_stat")]
    print_common!("   ------");

    print_common!("\n");
}

/// 打印任务信息数据
fn os_shell_cmd_tsk_info_data(all_task_array: *const TaskCB) {
    unsafe {
        for loop_idx in 0..KERNEL_TSK_LIMIT {
            let task_cb = all_task_array.offset(loop_idx as isize);
            if (*task_cb).task_status & OS_TASK_STATUS_UNUSED != TaskStatus::empty() {
                continue;
            }

            // 获取任务ID和安全索引
            let task_id = (*task_cb).task_id as usize;
            let task_idx = task_id % (LOSCFG_BASE_CORE_TSK_LIMIT as usize);

            // 打印任务基本信息
            let task_entry_ptr = (*task_cb).task_entry.map(|f| f as usize).unwrap_or(0);

            // 安全获取任务名称
            let task_name = if (*task_cb).task_name.is_null() {
                "<unnamed>"
            } else {
                // 尝试计算字符串长度（有限制地）
                let mut name_len = 0;
                let max_len = 32; // 名称最大长度限制

                while name_len < max_len {
                    if *(*task_cb).task_name.add(name_len) == 0 {
                        break;
                    }
                    name_len += 1;
                }

                if name_len > 0 {
                    match core::str::from_utf8(core::slice::from_raw_parts(
                        (*task_cb).task_name,
                        name_len,
                    )) {
                        Ok(s) => s,
                        Err(_) => "<invalid>",
                    }
                } else {
                    "<empty>"
                }
            };

            print_common!(
                "{:<23}0x{:08x}        0x{:<5x}",
                task_name,
                task_entry_ptr,
                (*task_cb).task_id
            );

            // 使用安全索引访问水线数组
            print_common!(
                "{:<11}{:<13}0x{:<11x}0x{:<11x}  0x{:08x}   0x{:08x}   ",
                (*task_cb).priority,
                os_shell_cmd_convert_tsk_status((*task_cb).task_status),
                (*task_cb).stack_size,
                G_TASK_WATER_LINE[task_idx], // 使用安全索引
                (*task_cb).stack_pointer as usize,
                (*task_cb).top_of_stack as usize
            );

            // 其他打印内容保持不变
            #[cfg(feature = "base_ipc_event")]
            print_common!("0x{:<6x}", (*task_cb).event_mask);

            #[cfg(feature = "mem_task_stat")]
            print_common!("    {:<11}", os_mem_usage((*task_cb).task_id));

            print_common!("\n");
        }
    }
}

/// 获取任务信息
/// 获取任务信息
#[unsafe(export_name = "OsShellCmdTskInfoGet")]
fn os_shell_cmd_tsk_info_get(task_id: u32) -> u32 {
    unsafe {
        if task_id == OS_ALL_TASK_MASK {
            // 使用Rust数组初始化语法代替memset_s
            G_TASK_WATER_LINE = [0; LOSCFG_BASE_CORE_TSK_LIMIT as usize];

            // 直接使用原始任务数组，避免内存分配和复制
            let tcb_array = g_taskCBArray;

            // 获取水线信息
            os_shell_cmd_task_water_line_get(tcb_array);

            // 打印任务信息
            os_shell_cmd_tsk_info_title();
            os_shell_cmd_tsk_info_data(tcb_array);
        } else {
            // 打印特定任务的调用栈
            os_task_back_trace(task_id);
        }
    }

    LOS_OK
}

/// 解析命令行参数
unsafe fn parse_argv_to_cstr(argv: *const *const u8, index: i32) -> &'static str {
    unsafe {
        if argv.is_null() || (*argv.offset(index as isize)).is_null() {
            return "";
        }

        let c_str = core::ffi::CStr::from_ptr(*argv.offset(index as isize) as *const u8);
        match c_str.to_str() {
            Ok(s) => s,
            Err(_) => "",
        }
    }
}

/// 解析数字字符串
fn parse_num(s: &str) -> Option<usize> {
    let radix = if s.starts_with("0x") || s.starts_with("0X") {
        16
    } else {
        10
    };

    let s = if radix == 16 && (s.starts_with("0x") || s.starts_with("0X")) {
        &s[2..]
    } else {
        s
    };

    usize::from_str_radix(s, radix).ok()
}

/// 任务信息命令实现
pub fn cmd_task(argc: i32, argv: *const *const u8) -> u32 {
    print_common!("hello\n");

    let task_id: usize;

    //打印调试
    print_common!("cmd_task argc: {}, argv: {:p}\n", argc, argv);

    if argc < 2 {
        if argc == 0 {
            task_id = OS_ALL_TASK_MASK as usize;
            print_common!("\naaaaaaaa\n");
        } else {
            // 解析参数为数字
            let arg = unsafe { parse_argv_to_cstr(argv, 0) };

            print_common!("arg: {}\n", arg);
            if arg.is_empty() {
                print_common!("\nUsage: task or task ID\n");
                return OS_ERROR;
            }

            match parse_num(arg) {
                Some(id) if id < KERNEL_TSK_LIMIT as usize => task_id = id,
                _ => {
                    print_common!("\ntask ID can't access {}.\n", arg);
                    return OS_ERROR;
                }
            }
        }

        print_common!("task_id: {}\n", task_id);

        return os_shell_cmd_tsk_info_get(task_id as u32);
    } else {
        print_common!("\nUsage: task or task ID\n");
        return OS_ERROR;
    }
}

/// 命令入口函数
#[unsafe(no_mangle)]
pub unsafe extern "C" fn rust_task_cmd(argc: i32, argv: *const *const u8) -> u32 {
    cmd_task(argc, argv)
}

// 注册task命令
#[unsafe(no_mangle)] // 防止编译器修改符号名
#[used]
#[unsafe(link_section = ".liteos.table.shellcmd.data")]
pub static task_shellcmd: ShellCmd = ShellCmd {
    cmd_type: CmdType::Ex,
    cmd_key: b"task\0".as_ptr() as *const c_char,
    para_num: 1,
    cmd_hook: rust_task_cmd,
};
