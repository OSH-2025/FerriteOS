//! 栈信息查询和检查命令实现

use crate::print_common;
use crate::print_error;
use crate::shellcmd::types::{CmdType, ShellCmd};
use crate::stack::get_stack_waterline;
use crate::stack::global::get_stack_info;
use core::ffi::c_char;

// 常量定义
const OS_STACK_MAGIC_WORD: usize = 0xCCCCCCCC;
const LOSCFG_KERNEL_CORE_NUM: u32 = 1;

/// 检查栈是否溢出
#[unsafe(export_name = "OsExcStackCheck")]
pub fn os_exc_stack_check() {
    // 获取栈信息
    let (maybe_stack_info, stack_num) = get_stack_info();
    let stack_info = match maybe_stack_info {
        Some(info) => info,
        None => return,
    };

    // 遍历所有栈
    for index in 0..stack_num as usize {
        for cpu_id in 0..LOSCFG_KERNEL_CORE_NUM {
            // 计算当前CPU的栈顶指针
            let stack_offset = cpu_id * stack_info[index].stack_size;
            let stack_top_addr = (stack_info[index].stack_addr as usize) + (stack_offset as usize);

            // 检查栈魔术字
            let stack_top = stack_top_addr as *const usize;
            if unsafe { *stack_top } != OS_STACK_MAGIC_WORD {
                print_error!(
                    "cpu:{} {} overflow, magic word changed to 0x{:x}\n",
                    LOSCFG_KERNEL_CORE_NUM - 1 - cpu_id,
                    unsafe { &*(*stack_info.as_ptr().add(index)).stack_name },
                    unsafe { *stack_top }
                );
            }
        }
    }
}

/// 显示栈信息
#[unsafe(export_name = "OsExcStackInfo")]
pub fn os_exc_stack_info() {
    // 获取栈信息
    let (maybe_stack_info, stack_num) = get_stack_info();
    let stack_info = match maybe_stack_info {
        Some(info) => info,
        None => return,
    };

    // 打印表头
    print_common!(
        "\n stack name    cpu id     stack addr     total size   used size\n ----------    ------     ---------      --------     --------\n"
    );

    // 遍历所有栈
    for index in 0..stack_num as usize {
        for cpu_id in 0..LOSCFG_KERNEL_CORE_NUM {
            // 计算当前CPU的栈顶和栈底指针
            let stack_offset = cpu_id * stack_info[index].stack_size;
            let stack_top_addr = (stack_info[index].stack_addr as usize) + (stack_offset as usize);
            let stack_top = stack_top_addr as *mut usize;
            let stack_size = stack_info[index].stack_size;
            let stack_addr =
                unsafe { stack_top.add(stack_size as usize / core::mem::size_of::<usize>()) };

            // 调用正确的函数签名 - 使用引用符号 & 配合解引用
            let size = unsafe {
                match get_stack_waterline(&*stack_top, &*stack_addr) {
                    Ok(size) => size,
                    Err(_) => 0, // 处理错误情况
                }
            };

            // 打印栈信息
            print_common!(
                "{:11}      {:<5}    {:<10p}     0x{:<8x}   0x{:<4x}\n",
                c_str_to_str(stack_info[index].stack_name), // 修正栈名称显示
                LOSCFG_KERNEL_CORE_NUM - 1 - cpu_id,
                stack_top,
                stack_size,
                size
            );
        }
    }

    // 检查栈是否溢出
    os_exc_stack_check();
}

/// 将 C 字符串转换为 Rust 字符串，处理空指针和无效 UTF-8 的情况
fn c_str_to_str(c_str: *const c_char) -> &'static str {
    if c_str.is_null() {
        return "<null>";
    }

    unsafe {
        let c_str = core::ffi::CStr::from_ptr(c_str);
        match c_str.to_str() {
            Ok(s) => s,
            Err(_) => "<invalid>",
        }
    }
}

/// 栈信息命令实现的C接口
#[unsafe(no_mangle)]
pub extern "C" fn rust_stack_cmd(_argc: i32, _argv: *const *const u8) -> u32 {
    os_exc_stack_info();
    0
}

// 注册stack命令
#[used]
#[unsafe(link_section = ".shell.cmds")]
pub static STACK_SHELL_CMD: ShellCmd = ShellCmd {
    cmd_type: CmdType::Ex,
    cmd_key: b"stack\0".as_ptr() as *const c_char,
    para_num: 1,
    cmd_hook: rust_stack_cmd,
};
