#![no_std]

extern crate alloc;

use core::fmt;
use alloc::vec::Vec;
use alloc::string::String;
use alloc::format;

/// 调试级别枚举
#[derive(Debug, Clone, Copy, PartialEq, Eq, PartialOrd, Ord)]
pub enum DebugLevel {
    Error = 0,
    Warn = 1,
    Info = 2,
    Debug = 3,
    Trace = 4,
}

/// 调试通道结构体
#[derive(Debug, Clone)]
pub struct DebugChannel {
    pub name: String,
    pub level: DebugLevel,
    pub enabled: bool,
}

impl DebugChannel {
    pub fn new(name: &str, level: DebugLevel) -> Self {
        Self {
            name: String::from(name),
            level,
            enabled: true,
        }
    }
}

/// 多路复用调试器
pub struct MuxDebugger {
    channels: Vec<DebugChannel>,
    global_level: DebugLevel,
}

impl MuxDebugger {
    /// 创建新的多路复用调试器
    pub fn new() -> Self {
        Self {
            channels: Vec::new(),
            global_level: DebugLevel::Info,
        }
    }

    /// 添加调试通道
    pub fn add_channel(&mut self, channel: DebugChannel) {
        self.channels.push(channel);
    }

    /// 设置全局调试级别
    pub fn set_global_level(&mut self, level: DebugLevel) {
        self.global_level = level;
    }

    /// 启用/禁用指定通道
    pub fn set_channel_enabled(&mut self, name: &str, enabled: bool) {
        if let Some(channel) = self.channels.iter_mut().find(|ch| ch.name == name) {
            channel.enabled = enabled;
        }
    }

    /// 输出调试信息
    pub fn debug(&self, channel_name: &str, level: DebugLevel, message: &str) {
        if level > self.global_level {
            return;
        }

        if let Some(channel) = self.channels.iter().find(|ch| ch.name == channel_name) {
            if !channel.enabled || level > channel.level {
                return;
            }

            self.output(channel_name, level, message);
        }
    }

    /// 格式化输出
    fn output(&self, channel: &str, level: DebugLevel, message: &str) {
        let level_str = match level {
            DebugLevel::Error => "ERROR",
            DebugLevel::Warn => "WARN ",
            DebugLevel::Info => "INFO ",
            DebugLevel::Debug => "DEBUG",
            DebugLevel::Trace => "TRACE",
        };

        // 这里可以根据实际需求输出到不同的目标
        // 例如：串口、内存缓冲区、文件等
        self.print_to_console(&format!("[{}][{}] {}", level_str, channel, message));
    }

    /// 输出到控制台（需要根据实际硬件实现）
    fn print_to_console(&self, message: &str) {
        // 这里应该调用实际的控制台输出函数
        // 例如：uart_print, vga_print 等
        // 暂时使用 panic 输出作为示例
        #[cfg(feature = "std")]
        println!("{}", message);
        
        #[cfg(not(feature = "std"))]
        {
            // 在无标准库环境下的输出实现
            // 这里需要根据具体的硬件平台实现
        }
    }

    /// 获取所有通道信息
    pub fn get_channels(&self) -> &Vec<DebugChannel> {
        &self.channels
    }
}

/// 全局调试器实例
static mut GLOBAL_DEBUGGER: Option<MuxDebugger> = None;

/// 初始化全局调试器
pub fn init_debug() {
    unsafe {
        let mut debugger = MuxDebugger::new();
        
        // 添加默认通道
        debugger.add_channel(DebugChannel::new("KERNEL", DebugLevel::Debug));
        debugger.add_channel(DebugChannel::new("MM", DebugLevel::Info));
        debugger.add_channel(DebugChannel::new("FS", DebugLevel::Info));
        debugger.add_channel(DebugChannel::new("NET", DebugLevel::Warn));
        
        GLOBAL_DEBUGGER = Some(debugger);
    }
}

/// 获取全局调试器引用
pub fn get_debugger() -> Option<&'static MuxDebugger> {
    unsafe { GLOBAL_DEBUGGER.as_ref() }
}

/// 获取全局调试器可变引用
pub fn get_debugger_mut() -> Option<&'static mut MuxDebugger> {
    unsafe { GLOBAL_DEBUGGER.as_mut() }
}

/// 便捷宏定义
#[macro_export]
macro_rules! debug_error {
    ($channel:expr, $($arg:tt)*) => {
        if let Some(debugger) = crate::debug::mux_debug::get_debugger() {
            debugger.debug($channel, crate::debug::mux_debug::DebugLevel::Error, &format!($($arg)*));
        }
    };
}

#[macro_export]
macro_rules! debug_warn {
    ($channel:expr, $($arg:tt)*) => {
        if let Some(debugger) = crate::debug::mux_debug::get_debugger() {
            debugger.debug($channel, crate::debug::mux_debug::DebugLevel::Warn, &format!($($arg)*));
        }
    };
}

#[macro_export]
macro_rules! debug_info {
    ($channel:expr, $($arg:tt)*) => {
        if let Some(debugger) = crate::debug::mux_debug::get_debugger() {
            debugger.debug($channel, crate::debug::mux_debug::DebugLevel::Info, &format!($($arg)*));
        }
    };
}

#[macro_export]
macro_rules! debug_trace {
    ($channel:expr, $($arg:tt)*) => {
        if let Some(debugger) = crate::debug::mux_debug::get_debugger() {
            debugger.debug($channel, crate::debug::mux_debug::DebugLevel::Trace, &format!($($arg)*));
        }
    };
}
