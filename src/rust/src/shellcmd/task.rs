//! 任务信息命令实现

use crate::print_common;
use crate::shellcmd::types::{CmdType, ShellCmd};
use crate::task::types::{TaskCB, TaskStatus};
use core::ffi::c_char;
use core::mem::size_of;
use crate::stack::{get_stack_waterline};
use crate::exception::backtrace::task_back_trace as os_task_back_trace;
#[cfg(feature = "mem_task_stat")]
use crate::mem::{memstat::os_memstat_task_usage as os_mem_usage,
                 memory::{los_mem_alloc, los_mem_free}};

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
        for loop_idx in 0..KERNEL_TSK_LIMIT {
            let task_cb = all_task_array.offset(loop_idx as isize);
            
            // 跳过未使用的任务
            if (*task_cb).task_status == OS_TASK_STATUS_UNUSED {
                continue;
            }

            // 获取栈指针地址
            let stack_bottom = (((*task_cb).top_of_stack as usize) + (*task_cb).stack_size as usize) as *const usize;
            let stack_top = (*task_cb).top_of_stack as *const usize;

            let task_id = (*task_cb).task_id as usize;
            let water_line_ptr = core::ptr::addr_of_mut!(G_TASK_WATER_LINE[task_id]) as *mut u32;
            
            // 修正：解引用指针并传递引用，处理返回结果
            match get_stack_waterline(&*stack_top, &*stack_bottom) {
                Ok(value) => *water_line_ptr = value,
                Err(_) => *water_line_ptr = 0, // 错误时设置为零
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

            let task_entry_ptr = match (*task_cb).task_entry {
                Some(func) => func as usize,
                None => 0,
            };

            print_common!(
                "{:<23}0x{:08x}        0x{:<5x}",
                (*task_cb).name(),
                task_entry_ptr,
                (*task_cb).task_id
            );

            print_common!(
                "{:<11}{:<13}0x{:<11x}0x{:<11x}  0x{:08x}   0x{:08x}   ",
                (*task_cb).priority,
                os_shell_cmd_convert_tsk_status((*task_cb).task_status),
                (*task_cb).stack_size,
                G_TASK_WATER_LINE[(*task_cb).task_id as usize],
                (*task_cb).stack_pointer as usize,
                (*task_cb).top_of_stack as usize
            );

            #[cfg(feature = "base_ipc_event")]
            print_common!("0x{:<6x}", (*task_cb).event_mask);

            #[cfg(feature = "mem_task_stat")]
            print_common!("    {:<11}", os_mem_usage((*task_cb).task_id));

            print_common!("\n");
        }
    }
}

/// 获取任务信息
#[unsafe(export_name = "OsShellCmdTskInfoGet")]
fn os_shell_cmd_tsk_info_get(task_id: u32) -> u32 {
    unsafe {
        if task_id == OS_ALL_TASK_MASK {
            // 获取所有任务信息
            let size = KERNEL_TSK_LIMIT as usize * size_of::<TaskCB>();
            let tcb_array = los_mem_alloc(m_aucSysMem1 as *mut core::ffi::c_void, size as u32) as *mut TaskCB;
            let backup_flag;

            if tcb_array.is_null() {
                print_common!("Memory is not enough to save task info!\n");
                // let tcb_array = g_taskCBArray as *mut TaskCB;
                backup_flag = false;
            } else {
                backup_flag = true;
            }

            // 初始化水线数组
            G_TASK_WATER_LINE = [0; LOSCFG_BASE_CORE_TSK_LIMIT as usize];

            if backup_flag {
                let ret = memcpy_s(
                    tcb_array as *mut core::ffi::c_void,
                    size,
                    g_taskCBArray as *const core::ffi::c_void,
                    size,
                );

                if ret != EOK {
                    return LOS_NOK;
                }
            }

            // 获取水线信息
            os_shell_cmd_task_water_line_get(tcb_array);

            // 打印任务信息
            os_shell_cmd_tsk_info_title();
            os_shell_cmd_tsk_info_data(tcb_array);

            // 释放内存
            if backup_flag {
                let _ = los_mem_free(m_aucSysMem1 as *mut core::ffi::c_void, tcb_array as *mut core::ffi::c_void);
            }
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
    let task_id: usize;

    if argc < 2 {
        if argc == 0 {
            task_id = OS_ALL_TASK_MASK as usize;
        } else {
            // 解析参数为数字
            let arg = unsafe { parse_argv_to_cstr(argv, 0) };

            match parse_num(arg) {
                Some(id) if id < KERNEL_TSK_LIMIT as usize => task_id = id,
                _ => {
                    print_common!("\ntask ID can't access {}.\n", arg);
                    return OS_ERROR;
                }
            }
        }

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
#[used]
#[unsafe(link_section = ".shell.cmds")]
pub static TASK_SHELL_CMD: ShellCmd = ShellCmd {
    cmd_type: CmdType::Ex,
    cmd_key: b"task\0".as_ptr() as *const c_char,
    para_num: 1,
    cmd_hook: rust_task_cmd,
};
