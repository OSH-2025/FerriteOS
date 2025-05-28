//! 打印模块 - LiteOS 打印功能的 Rust 实现
//! 
//! 本模块提供打印到UART、控制台和异常信息输出等功能。

use core::ffi::c_void;
use core::fmt;

/// 输出类型枚举
#[repr(u32)]
#[derive(Clone, Copy, PartialEq)]
enum OutputType {
    NoOutput = 0,
    UartOutput = 1,
    ConsoleOutput = 2,
    ExcOutput = 3,
}

// 定义UART锁定常量
const UART_WITH_LOCK: bool = true;
const UART_WITHOUT_LOCK: bool = false;

// 默认缓冲区大小
const SIZEBUF: usize = 256;

// 将VaList结构体定义移到extern块外部
#[repr(C)]
pub struct VaList {
    // 空结构体，作为不透明类型
    _data: [u8; 0],
    _marker: core::marker::PhantomData<*mut ()>,
}


// 外部函数声明
unsafe extern "C" {
    fn UartPuts(str: *const u8, len: u32, is_lock: bool);
    fn OsCheckUartLock() -> bool;
    fn OsLogMemcpyRecord(str: *const u8, len: u32) -> i32;
    fn ConsoleEnable() -> bool;
    fn LOS_MemAlloc(pool: *mut c_void, size: u32) -> *mut c_void;
    fn LOS_MemFree(pool: *mut c_void, ptr: *mut c_void) -> u32;
    fn vsnprintf_s(str: *mut u8, size: usize, count: usize, format: *const u8, args: *const VaList) -> i32;
    fn strlen(s: *const u8) -> usize;

    // // 为C语言中的可变参数声明
    // type va_list;
    // fn va_start(ap: *mut va_list, last_arg: *const u8) -> ();
    // fn va_end(ap: *mut va_list) -> ();

    // #[link_name = "m_aucSysMem0"]
    // static mut m_aucSysMem0: *mut c_void;

    // #[cfg(feature = "kernel_console")]
    // fn write(fd: i32, buf: *const c_void, count: usize) -> isize;
    
    // #[cfg(feature = "shell_excinfo_dump")]
    // fn WriteExcBufVa(fmt: *const u8, ap: *const va_list);
    

    fn va_start(_: *mut VaList, _: *const u8) -> ();
    fn va_end(_: *mut VaList) -> ();

    #[link_name = "m_aucSysMem0"]
    static mut m_aucSysMem0: *mut c_void;

    #[cfg(feature = "kernel_console")]
    fn write(fd: i32, buf: *const c_void, count: usize) -> isize;
    
    #[cfg(feature = "shell_excinfo_dump")]
    fn WriteExcBufVa(fmt: *const u8, ap: *const VaList);
    
    // 引用C中实现的变参函数
    fn UartPrintf(fmt: *const u8, ...) -> ();
    fn dprintf(fmt: *const u8, ...) -> ();
    fn ExcPrintf(fmt: *const u8, ...) -> ();
    fn PrintExcInfo(fmt: *const u8, ...) -> ();
    fn PrintErrWrapper(fmt: *const u8, ...) -> ();
    fn PrintkWrapper(fmt: *const u8, ...) -> ();
    fn printf(fmt: *const u8, ...) -> i32;
    
    // va_list版本函数
    fn UartVprintf(fmt: *const u8, ap: *const VaList);
    fn ConsoleVprintf(fmt: *const u8, ap: *const VaList);
    fn LkDprintf(fmt: *const u8, ap: *const VaList);
    #[cfg(feature = "shell_dmesg")]
    fn DmesgPrintf(fmt: *const u8, ap: *const VaList);
}

// 为存储va_list提供足够大小的缓冲区
// 在不同架构上va_list的大小可能不同，这里取较大值以确保安全
#[repr(C)]
struct VaListBuf {
    data: [u8; 32], // 通常32字节足以存储任何平台上的va_list
}

// STDOUT文件描述符常量
#[cfg(feature = "kernel_console")]
const STDOUT_FILENO: i32 = 1;

/// 错误消息输出
unsafe fn error_msg() {
    let p = b"Output illegal string! vsnprintf_s failed!\n\0";
    UartPuts(p.as_ptr(), (strlen(p.as_ptr()) - 1) as u32, UART_WITH_LOCK);
}

/// UART输出函数
unsafe fn uart_output(str: *const u8, len: u32, is_lock: bool) {
    #[cfg(feature = "shell_dmesg")]
    {
        if !OsCheckUartLock() {
            UartPuts(str, len, is_lock);
        }
        if is_lock != UART_WITHOUT_LOCK {
            let _ = OsLogMemcpyRecord(str, len);
        }
    }

    #[cfg(not(feature = "shell_dmesg"))]
    {
        UartPuts(str, len, is_lock);
    }
}

/// 控制输出目的地
unsafe fn output_control(str: *const u8, len: u32, output_type: OutputType) {
    match output_type {
        OutputType::ConsoleOutput => {
            #[cfg(feature = "kernel_console")]
            {
                if ConsoleEnable() {
                    let _ = write(STDOUT_FILENO, str as *const c_void, len as usize);
                    return;
                }
            }
            // 如果不支持控制台或控制台不可用，回退到UART输出
            uart_output(str, len, UART_WITH_LOCK);
        }
        OutputType::UartOutput => {
            uart_output(str, len, UART_WITH_LOCK);
        }
        OutputType::ExcOutput => {
            uart_output(str, len, UART_WITHOUT_LOCK);
        }
        _ => {}
    }
}

/// 释放缓冲区
unsafe fn os_vprintf_free(buf: *mut u8, buf_len: usize) {
    if buf_len != SIZEBUF {
        LOS_MemFree(m_aucSysMem0, buf as *mut c_void);
    }
}

/// 核心打印实现
#[unsafe(export_name = "OsVprintf")]
unsafe fn os_vprintf(fmt: *const u8, ap: *const VaList, output_type: OutputType) {
    let err_msg_malloc = b"OsVprintf, malloc failed!\n\0";
    let err_msg_len = b"OsVprintf, length overflow!\n\0";
    let mut a_buf = [0u8; SIZEBUF];
    let mut b_buf: *mut u8;
    let mut buf_len = SIZEBUF;

    b_buf = a_buf.as_mut_ptr();
    
    let len = vsnprintf_s(b_buf, buf_len, buf_len - 1, fmt, ap);
    
    if len == -1 && *b_buf == 0 {
        // 参数不合法或格式不支持
        error_msg();
        return;
    }

    while len == -1 {
        // b_buf不够大，需要分配更大的缓冲区
        os_vprintf_free(b_buf, buf_len);

        buf_len <<= 1;
        if (buf_len as i32) <= 0 {
            UartPuts(err_msg_len.as_ptr(), (strlen(err_msg_len.as_ptr()) - 1) as u32, UART_WITH_LOCK);
            return;
        }

        b_buf = LOS_MemAlloc(m_aucSysMem0, buf_len as u32) as *mut u8;
        if b_buf.is_null() {
            UartPuts(err_msg_malloc.as_ptr(), (strlen(err_msg_malloc.as_ptr()) - 1) as u32, UART_WITH_LOCK);
            return;
        }

        let len = vsnprintf_s(b_buf, buf_len, buf_len - 1, fmt, ap);
        if *b_buf == 0 {
            LOS_MemFree(m_aucSysMem0, b_buf as *mut c_void);
            error_msg();
            return;
        }
    }

    *b_buf.add(len as usize) = 0;
    output_control(b_buf, len as u32, output_type);
    os_vprintf_free(b_buf, buf_len);
}

/// UART变参打印
#[unsafe(export_name = "UartVprintf")]
pub extern "C" fn uart_vprintf(fmt: *const u8, ap: *const VaList) {
    unsafe {
        os_vprintf(fmt, ap, OutputType::UartOutput);
    }
}

/// 控制台变参打印
#[unsafe(export_name = "ConsoleVprintf")]
pub extern "C" fn console_vprintf(fmt: *const u8, ap: *const VaList) {
    unsafe {
        os_vprintf(fmt, ap, OutputType::ConsoleOutput);
    }
}

// /// UART打印
// #[unsafe(export_name = "UartPrintf")]
// pub extern "C" fn uart_printf(fmt: *const u8, ...) {
//     unsafe {
//         let mut va_buf = VaListBuf { data: [0; 32] };
//         let ap = &mut va_buf.data as *mut _ as *mut va_list;
//         va_start(ap, fmt);
//         os_vprintf(fmt, ap, OutputType::UartOutput);
//         va_end(ap);
//     }
// }

// /// 调试打印函数
// #[unsafe(export_name = "dprintf")]
// pub extern "C" fn dprintf(fmt: *const u8, ...) {
//     unsafe {
//         let mut va_buf = VaListBuf { data: [0; 32] };
//         let ap = &mut va_buf.data as *mut _ as *mut va_list;
//         va_start(ap, fmt);
//         os_vprintf(fmt, ap, OutputType::ConsoleOutput);
//         va_end(ap);
//     }
// }

/// LK调试打印
#[unsafe(export_name = "LkDprintf")]
pub extern "C" fn lk_dprintf(fmt: *const u8, ap: *const VaList) {
    unsafe {
        os_vprintf(fmt, ap, OutputType::ConsoleOutput);
    }
}

/// Dmesg打印
#[cfg(feature = "shell_dmesg")]
#[unsafe(export_name = "DmesgPrintf")]
pub extern "C" fn dmesg_printf(fmt: *const u8, ap: *const VaList) {
    unsafe {
        os_vprintf(fmt, ap, OutputType::ConsoleOutput);
    }
}
