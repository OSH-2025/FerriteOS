//! 栈信息结构体定义

use core::{ffi::c_char, fmt};

/// 栈信息结构体
#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub struct StackInfo {
    /// 栈的基地址
    pub stack_addr: *const usize,
    /// 栈的大小
    pub stack_size: u32,
    /// 栈的名称（以null结尾的C字符串）
    pub stack_name: *const c_char,
}

impl StackInfo {
    /// 获取栈名称的Rust字符串表示
    pub fn name(&self) -> &str {
        unsafe {
            core::ffi::CStr::from_ptr(self.stack_name)
                .to_str()
                .unwrap_or("unknown")
        }
    }
}

impl fmt::Display for StackInfo {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(
            f,
            "Stack '{}': addr=0x{:08x}, size={} bytes",
            self.name(),
            self.stack_addr as usize,
            self.stack_size
        )
    }
}
