//! 错误处理模块
use core::ffi::{c_char, c_void};

/// 错误处理函数类型定义
pub type ErrorHandlerFunc = Option<
    extern "C" fn(
        file_name: *const c_char,
        line_no: u32,
        error_no: u32,
        para_len: u32,
        para: *const c_void,
    ),
>;

/// 全局错误处理函数指针
static mut ERROR_HANDLER: ErrorHandlerFunc = None;

pub fn register_error_handler(handler: ErrorHandlerFunc) {
    unsafe { ERROR_HANDLER = handler };
}

pub fn handle_error(
    file_name: *const c_char,
    line_no: u32,
    error_no: u32,
    para_len: u32,
    para: *const c_void,
) {
    unsafe {
        if let Some(handler) = ERROR_HANDLER {
            handler(file_name, line_no, error_no, para_len, para);
        }
    }
}

