//! 错误处理模块
use core::ffi::c_void;

/// 错误处理函数类型定义
pub type ErrorHandleFunc = extern "C" fn(
    file_name: *const u8, // CHAR*
    line_no: u32,         // UINT32
    error_no: u32,        // UINT32
    para_len: u32,        // UINT32
    para: *mut c_void,    // VOID*
) -> u32; // UINT32

/// 全局错误处理函数指针
static mut ERROR_HANDLE_FUNC: Option<ErrorHandleFunc> = None;

/// 错误处理函数
///
/// # 参数
///
/// * `file_name` - 文件名
/// * `line_no` - 行号
/// * `error_no` - 错误码
/// * `para_len` - 参数长度
/// * `para` - 参数指针
///
/// # 返回值
///
/// 总是返回 LOS_OK (0)
#[unsafe(export_name = "LOS_ErrHandle")]
pub extern "C" fn err_handle(
    file_name: *const u8,
    line_no: u32,
    error_no: u32,
    para_len: u32,
    para: *mut c_void,
) -> u32 {
    unsafe {
        if let Some(func) = ERROR_HANDLE_FUNC {
            func(file_name, line_no, error_no, para_len, para);
        }
    }

    0 // LOS_OK
}

/// 注册错误处理函数
///
/// # 参数
///
/// * `func` - 错误处理函数
#[unsafe(export_name = "LOS_RegErrHandle")]
pub extern "C" fn reg_err_handle(func: Option<ErrorHandleFunc>) {
    unsafe {
        ERROR_HANDLE_FUNC = func;
    }
}
