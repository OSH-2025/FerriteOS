//! 内存相关的Shell命令实现

use crate::mem::memory::{los_mem_pool_size_get, los_mem_total_used_get};
use crate::mem::mempool::LosMemPoolInfo;
use crate::print_common as print_k;
use crate::shellcmd::types::{CmdCallBackFunc, CmdType, ShellCmd};
use core::ffi::c_char;

unsafe extern "C" {
    // 内存完整性检查函数
    fn OsMemIntegrityMultiCheck() -> u32;

    // 内存数据显示函数
    fn OsDumpMemByte(length: usize, addr: usize);

    // 系统内存池指针
    static m_aucSysMem1: *mut LosMemPoolInfo;

    // 内存使用节点显示（仅在启用内存泄漏检查时使用）
    #[cfg(feature = "mem_leakcheck")]
    fn OsMemUsedNodeShow(pool: *mut u8);
}

// 常量定义
const MEM_SIZE_1K: usize = 0x400;
const MEM_SIZE_1M: usize = 0x100000;
const OS_ERROR: u32 = u32::MAX; // 等同于C中的(UINT32)(-1)

// 外部符号引用
unsafe extern "C" {
    pub static g_sys_mem_addr_end: usize;
    pub static __text_start: u8;
    pub static __text_end: u8;
    pub static __ram_data_start: u8;
    pub static __ram_data_end: u8;
    pub static __rodata_start: u8;
    pub static __rodata_end: u8;
    pub static __bss_start: u8;
    pub static __bss_end: u8;
}

/// 内存检查命令实现
#[unsafe(no_mangle)]
pub unsafe extern "C" fn os_shell_cmd_mem_check(_argc: i32, _argv: *const *const u8) -> u32 {
    if _argc > 0 {
        print_k!("\nUsage: memcheck\n");
        return OS_ERROR;
    }

    unsafe { OsMemIntegrityMultiCheck() };
    0
}

/// 读取内存内容命令实现
#[unsafe(no_mangle)]
pub unsafe extern "C" fn os_shell_cmd_mem_read(argc: i32, argv: *const *const u8) -> u32 {
    let temp_addr: usize;
    let length: usize;

    if argc == 0 || argc > 2 {
        print_k!("\nUsage: readreg [ADDRESS] [LENGTH]\n");
        return OS_ERROR;
    }

    // 解析参数
    if argc == 1 {
        length = 0;
    } else {
        let arg1 = unsafe { parse_argv_to_cstr(argv, 1) };
        match parse_num(arg1) {
            Some(len) => length = len,
            None => {
                print_k!("readreg invalid length {}\n", arg1);
                return OS_ERROR;
            }
        }
    }

    let arg0 = unsafe { parse_argv_to_cstr(argv, 0) };
    match parse_num(arg0) {
        Some(addr) => temp_addr = addr,
        None => {
            print_k!("readreg invalid address {}\n", arg0);
            return OS_ERROR;
        }
    }

    // 安全检查
    unsafe {
        if temp_addr > g_sys_mem_addr_end
            || (temp_addr + length) > g_sys_mem_addr_end
            || temp_addr > (usize::MAX - length)
        {
            print_k!("readreg invalid address {}\n", arg0);
            return OS_ERROR;
        }

        OsDumpMemByte(length, temp_addr);
    }

    0
}

/// 显示代码段信息
#[unsafe(no_mangle)]
pub unsafe extern "C" fn os_shell_cmd_section_info(argc: i32, argv: *const *const u8) {
    let text_len: usize;
    let data_len: usize;
    let rodata_len: usize;
    let bss_len: usize;

    // 计算各段大小
    unsafe {
        text_len = (&__text_end as *const u8).offset_from(&__text_start as *const u8) as usize;
        data_len =
            (&__ram_data_end as *const u8).offset_from(&__ram_data_start as *const u8) as usize;
        rodata_len =
            (&__rodata_end as *const u8).offset_from(&__rodata_start as *const u8) as usize;
        bss_len = (&__bss_end as *const u8).offset_from(&__bss_start as *const u8) as usize;
    }

    print_k!("\r\n        text         data          rodata        bss\n");

    if argc == 1 {
        let arg = unsafe { parse_argv_to_cstr(argv, 0) };

        if arg == "-k" {
            print_k!(
                "Mem:    {:<9}    {:<10}    {:<10}    {:<10}\n",
                text_len / MEM_SIZE_1K,
                data_len / MEM_SIZE_1K,
                rodata_len / MEM_SIZE_1K,
                bss_len / MEM_SIZE_1K
            );
        } else if arg == "-m" {
            print_k!(
                "Mem:    {:<9}    {:<10}    {:<10}    {:<10}\n",
                text_len / MEM_SIZE_1M,
                data_len / MEM_SIZE_1M,
                rodata_len / MEM_SIZE_1M,
                bss_len / MEM_SIZE_1M
            );
        } else {
            print_k!(
                "Mem:    {:<9}    {:<10}    {:<10}    {:<10}\n",
                text_len,
                data_len,
                rodata_len,
                bss_len
            );
        }
    } else {
        print_k!(
            "Mem:    {:<9}    {:<10}    {:<10}    {:<10}\n",
            text_len,
            data_len,
            rodata_len,
            bss_len
        );
    }
}

/// 显示内存使用情况
#[unsafe(no_mangle)]
pub unsafe extern "C" fn os_shell_cmd_free_info(argc: i32, argv: *const *const u8) -> u32 {
    let mem_used: u32;
    let total_mem: u32;
    let free_mem: u32;

    unsafe {
        mem_used = los_mem_total_used_get(m_aucSysMem1);
        total_mem = los_mem_pool_size_get(m_aucSysMem1);
    }
    free_mem = total_mem - mem_used;

    if argc == 0
        || (argc == 1 && unsafe { parse_argv_to_cstr(argv, 0) } == "-k")
        || (argc == 1 && unsafe { parse_argv_to_cstr(argv, 0) } == "-m")
    {
        print_k!("\r\n        total        used          free\n");
    }

    if argc == 1 {
        let arg = unsafe { parse_argv_to_cstr(argv, 0) };

        if arg == "-k" {
            print_k!(
                "Mem:    {:<9}    {:<10}    {:<10}\n",
                total_mem / MEM_SIZE_1K as u32,
                mem_used / MEM_SIZE_1K as u32,
                free_mem / MEM_SIZE_1K as u32
            );
        } else if arg == "-m" {
            print_k!(
                "Mem:    {:<9}    {:<10}    {:<10}\n",
                total_mem / MEM_SIZE_1M as u32,
                mem_used / MEM_SIZE_1M as u32,
                free_mem / MEM_SIZE_1M as u32
            );
        } else {
            print_k!("\nUsage: free or free [-k/-m]\n");
            return OS_ERROR;
        }
    } else if argc == 0 {
        print_k!(
            "Mem:    {:<9}    {:<10}    {:<10}\n",
            total_mem,
            mem_used,
            free_mem
        );
    } else {
        print_k!("\nUsage: free or free [-k/-m]\n");
        return OS_ERROR;
    }
    0
}

/// 内存使用命令实现
#[unsafe(no_mangle)]
pub unsafe extern "C" fn os_shell_cmd_free(argc: i32, argv: *const *const u8) -> u32 {
    if argc > 1 {
        print_k!("\nUsage: free or free [-k/-m]\n");
        return OS_ERROR;
    }

    unsafe {
        if os_shell_cmd_free_info(argc, argv) != 0 {
            return OS_ERROR;
        }

        os_shell_cmd_section_info(argc, argv);
        0
    }
}

/// 系统信息命令实现
#[unsafe(export_name = "OsShellCmdUname")]
pub unsafe extern "C" fn os_shell_cmd_uname(argc: i32, argv: *const *const u8) -> u32 {
    let kernel_version = option_env!("HW_LITEOS_KERNEL_VERSION_STRING").unwrap_or("1.0.0");
    let system_name = option_env!("HW_LITEOS_SYSNAME").unwrap_or("Huawei LiteOS");
    let liteos_ver = option_env!("HW_LITEOS_VER").unwrap_or("Huawei LiteOS");
    let build_date = option_env!("CARGO_BUILD_DATE").unwrap_or("Unknown");
    let build_time = option_env!("CARGO_BUILD_TIME").unwrap_or("Unknown");

    if argc == 0 {
        print_k!("Huawei LiteOS\n");
        return 0;
    }

    if argc == 1 {
        let arg = unsafe { parse_argv_to_cstr(argv, 0) };

        if arg == "-a" {
            print_k!(
                "{} {} {} {} {}\n",
                liteos_ver,
                system_name,
                kernel_version,
                build_date,
                build_time
            );
            return 0;
        } else if arg == "-s" {
            print_k!("Huawei LiteOS\n");
            return 0;
        } else if arg == "-t" {
            print_k!("build date : {} {}\n", build_date, build_time);
            return 0;
        } else if arg == "-v" {
            print_k!(
                "{} {} {} {}\n",
                system_name,
                kernel_version,
                build_date,
                build_time
            );
            return 0;
        } else if arg == "--help" {
            print_k!(
                "-a,            print all information\n\
                      -s,            print the kernel name\n\
                      -t,            print the build date\n\
                      -v,            print the kernel version\n"
            );
            return 0;
        }
    }

    print_k!(
        "uname: invalid option {}\n\
              Try 'uname --help' for more information.\n",
        unsafe { parse_argv_to_cstr(argv, 0) }
    );
    OS_ERROR
}

/// 内存使用节点命令实现
#[cfg(feature = "mem_leakcheck")]
pub unsafe extern "C" fn os_shell_cmd_mem_used(argc: i32, _argv: *const *const u8) -> u32 {
    if argc > 0 {
        print_k!("\nUsage: memused\n");
        return OS_ERROR;
    }

    unsafe { OsMemUsedNodeShow(m_aucSysMem1 as *mut u8) };
    0
}

// -------------------- 辅助函数 --------------------

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

// -------------------- 命令注册 --------------------

// 注册memcheck命令
#[unsafe(no_mangle)] // 防止编译器修改符号名
#[used]
#[unsafe(link_section = ".liteos.table.shellcmd.data")]
pub static memcheck_shellcmd: ShellCmd = ShellCmd {
    cmd_type: CmdType::Ex,
    cmd_key: b"memcheck\0".as_ptr() as *const c_char,
    para_num: 0,
    cmd_hook: os_shell_cmd_mem_check as CmdCallBackFunc,
};

// 注册memread命令
#[unsafe(no_mangle)] // 防止编译器修改符号名
#[used]
#[unsafe(link_section = ".liteos.table.shellcmd.data")]
pub static memread_shellcmd: ShellCmd = ShellCmd {
    cmd_type: CmdType::Ex,
    cmd_key: b"memread\0".as_ptr() as *const c_char,
    para_num: 0xFFFF, // XARGS
    cmd_hook: os_shell_cmd_mem_read as CmdCallBackFunc,
};

// 注册uname命令
#[unsafe(no_mangle)] // 防止编译器修改符号名
#[used]
#[unsafe(link_section = ".liteos.table.shellcmd.data")]
pub static uname_shellcmd: ShellCmd = ShellCmd {
    cmd_type: CmdType::Ex,
    cmd_key: b"uname\0".as_ptr() as *const c_char,
    para_num: 0xFFFF, // XARGS
    cmd_hook: os_shell_cmd_uname as CmdCallBackFunc,
};

// 注册free命令
#[unsafe(no_mangle)] // 防止编译器修改符号名
#[used]
#[unsafe(link_section = ".liteos.table.shellcmd.data")]
pub static free_shellcmd: ShellCmd = ShellCmd {
    cmd_type: CmdType::Ex,
    cmd_key: b"free\0".as_ptr() as *const c_char,
    para_num: 0xFFFF, // XARGS
    cmd_hook: os_shell_cmd_free as CmdCallBackFunc,
};

// 注册memused命令(仅当启用MEM_LEAKCHECK特性时)
#[cfg(feature = "mem_leakcheck")]
#[unsafe(no_mangle)] // 防止编译器修改符号名
#[used]
#[unsafe(link_section = ".liteos.table.shellcmd.data")]
pub static memused_shellcmd: ShellCmd = ShellCmd {
    cmd_type: CmdType::Ex,
    cmd_key: b"memused\0".as_ptr() as *const c_char,
    para_num: 0,
    cmd_hook: os_shell_cmd_mem_used as CmdCallBackFunc,
};
