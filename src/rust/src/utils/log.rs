use crate::ffi::bindings::dprintf;
use core::fmt::{self, Write};

// 日志级别定义
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
#[repr(u8)]
#[allow(dead_code)]
pub enum LogLevel {
    Emergency = 0, // LOS_EMG_LEVEL
    Common = 1,    // LOS_COMMOM_LEVEL
    Error = 2,     // LOS_ERR_LEVEL
    Warning = 3,   // LOS_WARN_LEVEL
    Info = 4,      // LOS_INFO_LEVEL
    Debug = 5,     // LOS_DEBUG_LEVEL
}

// 编译时日志级别配置
#[allow(dead_code)]
pub const PRINT_LEVEL: LogLevel = {
    #[cfg(feature = "log-emergency")]
    {
        LogLevel::Emergency
    }
    #[cfg(feature = "log-common")]
    {
        LogLevel::Common
    }
    #[cfg(feature = "log-error")]
    {
        LogLevel::Error
    }
    #[cfg(feature = "log-warning")]
    {
        LogLevel::Warning
    }
    #[cfg(feature = "log-info")]
    {
        LogLevel::Info
    }
    #[cfg(feature = "log-debug")]
    {
        LogLevel::Debug
    }

    // 默认级别（如果没有指定任何feature）
    #[cfg(not(any(
        feature = "log-emergency",
        feature = "log-common",
        feature = "log-error",
        feature = "log-warning",
        feature = "log-info",
        feature = "log-debug"
    )))]
    {
        LogLevel::Error
    }
};

// 日志级别前缀
#[allow(dead_code)]
const LOG_PREFIXES: &[&str] = &["[EMG] ", "", "[ERR] ", "[WARN] ", "[INFO] ", "[DEBUG] "];

/// 内部辅助函数，用于将 Rust 格式化字符串和参数发送到 C dprintf
/// 在no_std环境中，使用固定大小的缓冲区和heapless字符串
#[doc(hidden)]
pub fn _dprintf_internal(prefix: &str, args: core::fmt::Arguments) {
    // 使用heapless::String避免堆分配
    let mut message = heapless::String::<512>::new();
    // 写入前缀和格式化消息
    if write!(message, "{}{}", prefix, args).is_ok() && message.push('\0').is_ok() {
        dprintf(message.as_ptr());
    } else {
        // 格式化失败，输出错误消息
        dprintf(b"Log message too long or format error\n\0".as_ptr());
    }
}

// 带前缀的日志打印函数
#[allow(dead_code)]
pub fn log_with_prefix(level: LogLevel, args: fmt::Arguments) {
    if level <= PRINT_LEVEL {
        let prefix = LOG_PREFIXES[level as usize];
        // 使用改进的内部函数
        _dprintf_internal(prefix, args);
    }
}

// 公共宏定义
#[macro_export]
macro_rules! print_emergency {
    ($($arg:tt)*) => {
        $crate::utils::log::log_with_prefix(
            $crate::utils::log::LogLevel::Emergency,
            format_args!($($arg)*)
        );
    };
}

#[macro_export]
macro_rules! print_common {
    ($($arg:tt)*) => {
        $crate::utils::log::log_with_prefix(
            $crate::utils::log::LogLevel::Common,
            format_args!($($arg)*)
        );
    };
}

#[macro_export]
macro_rules! print_error {
    ($($arg:tt)*) => {
        $crate::utils::log::log_with_prefix(
            $crate::utils::log::LogLevel::Error,
            format_args!($($arg)*)
        );
    };
}

#[macro_export]
macro_rules! print_warning {
    ($($arg:tt)*) => {
        $crate::utils::log::log_with_prefix(
            $crate::utils::log::LogLevel::Warning,
            format_args!($($arg)*)
        );
    };
}

#[macro_export]
macro_rules! print_info {
    ($($arg:tt)*) => {
        $crate::utils::log::log_with_prefix(
            $crate::utils::log::LogLevel::Info,
            format_args!($($arg)*)
        );
    };
}

#[macro_export]
macro_rules! print_debug {
    ($($arg:tt)*) => {
        $crate::utils::log::log_with_prefix(
            $crate::utils::log::LogLevel::Debug,
            format_args!($($arg)*)
        );
    };
}

#[macro_export]
macro_rules! println_emergency {
    () => {
        $crate::utils::log::log_with_prefix(
            $crate::utils::log::LogLevel::Emergency,
            format_args!("\n")
        );
    };
    ($($arg:tt)*) => {
        $crate::utils::log::log_with_prefix(
            $crate::utils::log::LogLevel::Emergency,
            format_args!("{}\n", format_args!($($arg)*))
        );
    };
}

#[macro_export]
macro_rules! println_common {
    () => {
        $crate::utils::log::log_with_prefix(
            $crate::utils::log::LogLevel::Common,
            format_args!("\n")
        );
    };
    ($($arg:tt)*) => {
        $crate::utils::log::log_with_prefix(
            $crate::utils::log::LogLevel::Common,
            format_args!("{}\n", format_args!($($arg)*))
        );
    };
}

#[macro_export]
macro_rules! println_error {
    () => {
        $crate::utils::log::log_with_prefix(
            $crate::utils::log::LogLevel::Error,
            format_args!("\n")
        );
    };
    ($($arg:tt)*) => {
        $crate::utils::log::log_with_prefix(
            $crate::utils::log::LogLevel::Error,
            format_args!("{}\n", format_args!($($arg)*))
        );
    };
}

#[macro_export]
macro_rules! println_warning {
    () => {
        $crate::utils::log::log_with_prefix(
            $crate::utils::log::LogLevel::Warning,
            format_args!("\n")
        );
    };
    ($($arg:tt)*) => {
        $crate::utils::log::log_with_prefix(
            $crate::utils::log::LogLevel::Warning,
            format_args!("{}\n", format_args!($($arg)*))
        );
    };
}

#[macro_export]
macro_rules! println_info {
    () => {
        $crate::utils::log::log_with_prefix(
            $crate::utils::log::LogLevel::Info,
            format_args!("\n")
        );
    };
    ($($arg:tt)*) => {
        $crate::utils::log::log_with_prefix(
            $crate::utils::log::LogLevel::Info,
            format_args!("{}\n", format_args!($($arg)*))
        );
    };
}

#[macro_export]
macro_rules! println_debug {
    () => {
        $crate::utils::log::log_with_prefix(
            $crate::utils::log::LogLevel::Debug,
            format_args!("\n")
        );
    };
    ($($arg:tt)*) => {
        $crate::utils::log::log_with_prefix(
            $crate::utils::log::LogLevel::Debug,
            format_args!("{}\n", format_args!($($arg)*))
        );
    };
}

pub struct DPrintfWriter {
    buffer: heapless::String<512>, // 增大缓冲区
}

#[allow(dead_code)]
impl DPrintfWriter {
    pub fn new() -> Self {
        Self {
            buffer: heapless::String::new(),
        }
    }

    pub fn flush(&mut self) {
        if !self.buffer.is_empty() {
            // 使用单独的null终止缓冲区
            self.flush_with_null_terminator();
            self.buffer.clear();
        }
    }

    /// 专门处理null终止符的刷新方法
    fn flush_with_null_terminator(&self) {
        // 创建包含null终止符的临时缓冲区
        let mut null_terminated = heapless::Vec::<u8, 513>::new(); // 512 + 1

        if null_terminated
            .extend_from_slice(self.buffer.as_bytes())
            .is_ok()
            && null_terminated.push(0).is_ok()
        {
            dprintf(null_terminated.as_ptr());
        } else {
            // 降级处理：如果无法创建null终止版本，强制截断
            let mut truncated = heapless::String::<512>::new();
            let max_len = self.buffer.len().min(511); // 为null终止符留一个字节

            if truncated.push_str(&self.buffer[..max_len]).is_ok() && truncated.push('\0').is_ok() {
                dprintf(truncated.as_ptr());
            }
        }
    }

    /// 处理大字符串，分批写入
    fn write_large_string(&mut self, s: &str) -> fmt::Result {
        const CHUNK_SIZE: usize = 512; // 使用完整的缓冲区大小

        for chunk in s.as_bytes().chunks(CHUNK_SIZE) {
            // 确保UTF-8边界正确
            let chunk_str = Self::safe_chunk_to_str_static(chunk);

            // 清空缓冲区并写入当前块
            self.buffer.clear();
            if self.buffer.push_str(chunk_str).is_err() {
                return Err(fmt::Error);
            }
            self.flush();
        }

        Ok(())
    }

    /// 安全地将字节块转换为字符串
    fn safe_chunk_to_str_static(chunk: &[u8]) -> &str {
        match core::str::from_utf8(chunk) {
            Ok(s) => s,
            Err(e) => {
                let valid_up_to = e.valid_up_to();
                unsafe { core::str::from_utf8_unchecked(&chunk[..valid_up_to]) }
            }
        }
    }
}

impl Write for DPrintfWriter {
    fn write_str(&mut self, s: &str) -> fmt::Result {
        if s.is_empty() {
            return Ok(());
        }

        // 尝试直接写入
        if self.buffer.push_str(s).is_ok() {
            return Ok(());
        }

        // 缓冲区满了，刷新并重试
        self.flush();

        // 如果字符串太大，分批处理
        if s.len() > self.buffer.capacity() {
            self.write_large_string(s)
        } else {
            self.buffer.push_str(s).map_err(|_| fmt::Error)
        }
    }
}

impl Drop for DPrintfWriter {
    fn drop(&mut self) {
        self.flush();
    }
}

#[macro_export]
macro_rules! print_release {
    ($($arg:tt)*) => {
        let mut writer = $crate::utils::log::DPrintfWriter::new();
        let _ = core::fmt::Write::write_fmt(&mut writer, format_args!($($arg)*));
        writer.flush();
    };
}

#[macro_export]
macro_rules! println_release {
    () => {
        let mut writer = $crate::utils::log::DPrintfWriter::new();
        let _ = core::fmt::Write::write_fmt(&mut writer, format_args!("{}"));
        writer.flush();
    };
    ($($arg:tt)*) => {
        let mut writer = $crate::utils::log::DPrintfWriter::new();
        let _ = core::fmt::Write::write_fmt(&mut writer, format_args!("{}\n", format_args!($($arg)*)));
        writer.flush();
    };
}
