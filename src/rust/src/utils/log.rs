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

#[doc(hidden)]
pub fn _print_internal(prefix: &str, args: core::fmt::Arguments) {
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
        _print_internal(prefix, args);
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
