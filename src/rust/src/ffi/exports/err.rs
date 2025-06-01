use core::ffi::{c_char, c_void};

use crate::error::{ErrorHandlerFunc, handle_error, register_error_handler};

/// 错误处理函数
#[unsafe(export_name = "LOS_ErrHandle")]
pub extern "C" fn los_err_handle(
    file_name: *const c_char,
    line_no: u32,
    error_no: u32,
    para_len: u32,
    para: *const c_void,
) -> u32 {
    handle_error(file_name, line_no, error_no, para_len, para);
    0 // LOS_OK
}

/// 注册错误处理函数
///
/// # 参数
///
/// * `func` - 错误处理函数
#[unsafe(export_name = "LOS_RegErrHandle")]
pub extern "C" fn los_reg_err_handle(handler: ErrorHandlerFunc) {
    register_error_handler(handler);
}
