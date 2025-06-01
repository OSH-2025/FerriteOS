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
        if self.stack_name.is_null() {
            return "unknown";
        }

        unsafe {
            let mut len = 0;
            let mut ptr = self.stack_name;

            // 计算C字符串长度
            while *ptr != 0 {
                len += 1;
                ptr = ptr.add(1);
            }

            // 转换为Rust字符串
            let bytes = core::slice::from_raw_parts(self.stack_name, len);
            core::str::from_utf8_unchecked(bytes)
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
