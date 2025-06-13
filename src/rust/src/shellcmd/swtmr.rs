//! 软件定时器信息命令实现

use crate::print_common;
use crate::shellcmd::types::{CmdType, ShellCmd};
use core::ffi::c_char;

// 导入所需的软件定时器相关结构和函数
// 这里假设已经在 ffi::exports::swtmr 模块中导出了相关函数
use crate::shellcmd::types::{LosSwtmrCB, g_swtmrCBArray};

// 常量定义
// const SWTMR_STRLEN: usize = 12;
const OS_ERROR: u32 = u32::MAX; // 等同于C中的 (UINT32)(-1)
const LOS_OK: u32 = 0;
const OS_ALL_SWTMR_MASK: usize = 0xffffffff;
const LOSCFG_BASE_CORE_SWTMR_LIMIT: u16 = 1024;

// 软件定时器模式字符串
static SWTMR_MODE_STRINGS: [&str; 4] = ["Once", "Period", "NSD", "OPP"];

// 软件定时器状态字符串
static SWTMR_STATUS_STRINGS: [&str; 3] = ["UnUsed", "Created", "Ticking"];

/// 打印软件定时器信息
fn os_print_swtmr_msg(swtmr: &LosSwtmrCB) {
    print_common!(
        "0x{:08x}  {:<7}  {:<6}   {:<6}   0x{:08x}  {:p}\n",
        (swtmr.timer_id as u32) % (LOSCFG_BASE_CORE_SWTMR_LIMIT as u32),
        SWTMR_STATUS_STRINGS[swtmr.state as usize],
        SWTMR_MODE_STRINGS[swtmr.mode as usize],
        swtmr.interval,
        swtmr.arg,
        swtmr.handler
    );
}

/// 打印软件定时器信息表头
fn os_print_swtmr_msg_head() {
    print_common!("\r\nSwTmrID     State    Mode    Interval  Arg         handlerAddr\n");
    print_common!("----------  -------  ------- --------- ----------  --------\n");
}

/// 软件定时器信息命令实现
pub fn cmd_swtmr(argc: i32, argv: *const *const u8) -> u32 {
    // 参数检查
    if argc > 1 {
        print_common!("\nUsage: swtmr [ID]\n");
        return OS_ERROR;
    }

    let timer_id: usize;

    // 解析参数
    if argc == 0 {
        timer_id = OS_ALL_SWTMR_MASK;
    } else {
        // 解析参数为数字
        let arg = unsafe { parse_argv_to_cstr(argv, 0) };
        match parse_num(arg) {
            Some(id) if id <= LOSCFG_BASE_CORE_SWTMR_LIMIT as usize => timer_id = id,
            _ => {
                print_common!("\nswtmr ID can't access {}.\n", arg);
                return OS_ERROR;
            }
        }
    }

    unsafe {
        // 计算未使用的定时器数量
        let mut num = 0;
        for i in 0..LOSCFG_BASE_CORE_SWTMR_LIMIT as usize {
            if (*g_swtmrCBArray.add(i)).state == 0 {
                num += 1;
            }
        }

        // 如果所有定时器都未使用，则退出
        if num == LOSCFG_BASE_CORE_SWTMR_LIMIT as usize {
            print_common!("\r\nThere is no swtmr was created!\n");
            return OS_ERROR;
        }

        // 打印表头
        os_print_swtmr_msg_head();

        if timer_id == OS_ALL_SWTMR_MASK {
            // 打印所有活动定时器
            for i in 0..LOSCFG_BASE_CORE_SWTMR_LIMIT as usize {
                let swtmr = &*g_swtmrCBArray.add(i);
                if swtmr.state != 0 {
                    os_print_swtmr_msg(swtmr);
                }
            }
        } else {
            // 打印指定ID的定时器
            let mut found = false;
            for i in 0..LOSCFG_BASE_CORE_SWTMR_LIMIT as usize {
                let swtmr = &*g_swtmrCBArray.add(i);
                if (timer_id == ((swtmr.timer_id as u32) % (LOSCFG_BASE_CORE_SWTMR_LIMIT as u32)) as usize)
                    && (swtmr.state != 0)
                {
                    os_print_swtmr_msg(swtmr);
                    found = true;
                    break;
                }
            }

            if !found {
                print_common!("\r\nThe SwTimerID is not exist.\n");
            }
        }
    }

    LOS_OK
}

/// 将命令行参数转换为Rust字符串
unsafe fn parse_argv_to_cstr(argv: *const *const u8, index: i32) -> &'static str {
    unsafe {
        if argv.is_null() || (*argv.offset(index as isize)).is_null() {
            return "";
        }

        let c_str = core::ffi::CStr::from_ptr(*argv.offset(index as isize) as *const c_char);
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

/// 命令入口函数
#[unsafe(no_mangle)]
pub unsafe extern "C" fn rust_swtmr_cmd(argc: i32, argv: *const *const u8) -> u32 {
    cmd_swtmr(argc, argv)
}

// 注册swtmr命令
#[cfg(feature = "base_core_swtmr")]
#[unsafe(no_mangle)]  // 防止编译器修改符号名
#[used] // 防止未使用的静态项被优化掉
#[unsafe(link_section = ".liteos.table.shellcmd.data")] // 使用与C代码相同的段名
pub static swtmr_shellcmd: ShellCmd = ShellCmd {
    cmd_type: CmdType::Ex,
    cmd_key: b"swtmr\0".as_ptr() as *const c_char,
    para_num: 1,
    cmd_hook: rust_swtmr_cmd,
};
