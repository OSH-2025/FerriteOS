//! LiteOS 事件追踪模块的 Rust 实现

use core::ffi::c_void;

/// 追踪模块定义的事件类型掩码
pub mod trace_mask {
    pub const SYS_FLAG: u32 = 0x10;
    pub const HWI_FLAG: u32 = 0x20;
    pub const TASK_FLAG: u32 = 0x40;
    pub const SWTMR_FLAG: u32 = 0x80;
    pub const MEM_FLAG: u32 = 0x100;
    pub const QUE_FLAG: u32 = 0x200;
    pub const EVENT_FLAG: u32 = 0x400;
    pub const SEM_FLAG: u32 = 0x800;
    pub const MUX_FLAG: u32 = 0x1000;
    pub const MAX_FLAG: u32 = 0x80000000;
    pub const USER_DEFAULT_FLAG: u32 = 0xFFFFFFF0;
}

/// 追踪模块定义的事件类型
pub mod trace_event {
    use super::trace_mask::*;

    // 系统事件
    pub const SYS_ERROR: u32 = SYS_FLAG | 0;
    pub const SYS_START: u32 = SYS_FLAG | 1;
    pub const SYS_STOP: u32 = SYS_FLAG | 2;

    // 事件相关
    pub const EVENT_CREATE: u32 = EVENT_FLAG | 0;
    pub const EVENT_DELETE: u32 = EVENT_FLAG | 1;
    pub const EVENT_READ: u32 = EVENT_FLAG | 2;
    pub const EVENT_WRITE: u32 = EVENT_FLAG | 3;
    pub const EVENT_CLEAR: u32 = EVENT_FLAG | 4;
    
    // 这里可以添加其他事件类型
}

/// 事件钩子类型定义
pub type TraceEventHook = unsafe extern "C" fn(
    event_type: u32,
    identity: usize,
    params: *const usize,
    param_count: u16
);

/// 性能跟踪相关的外部函数
unsafe extern "C" {
    pub fn LOS_PerformanceCounterStart(type_id: u32);
    pub fn LOS_PerformanceCounterStop(type_id: u32);
    pub static mut g_traceEventHook: Option<TraceEventHook>;
}

/// 性能监测功能
#[inline]
pub fn los_perf(type_id: u32) {
    #[cfg(feature = "perf")]
    unsafe {
        LOS_PerformanceCounterStart(type_id);
        LOS_PerformanceCounterStop(type_id);
    }
}

/// 事件跟踪功能
///
/// # 参数
///
/// * `event_type` - 事件类型
/// * `identity` - 事件主体标识符
/// * `params` - 事件参数数组
///
/// # 安全性
///
/// 该函数调用不安全的 C 函数，调用者需要确保参数有效
pub fn los_trace(event_type: u32, identity: usize, params: &[usize]) {
    los_perf(event_type);
    
    #[cfg(feature = "kernel_trace")]
    unsafe {
        if let Some(hook) = g_traceEventHook {
            if params.is_empty() {
                hook(event_type, identity, core::ptr::null(), 0);
            } else {
                hook(event_type, identity, params.as_ptr(), params.len() as u16);
            }
        }
    }
}

/// 事件相关的跟踪辅助函数
pub mod event_trace {
    use super::*;
    use super::trace_event::*;
    use crate::event::EventCB;
    
    /// 追踪事件创建
    pub fn trace_event_create(event_cb: *const EventCB) {
        let params = [event_cb as usize];
        los_trace(EVENT_CREATE, event_cb as usize, &params);
    }
    
    /// 追踪事件删除
    pub fn trace_event_delete(event_cb: *const EventCB, del_ret_code: u32) {
        let params = [event_cb as usize, del_ret_code as usize];
        los_trace(EVENT_DELETE, event_cb as usize, &params);
    }
    
    /// 追踪事件读取
    pub fn trace_event_read(event_cb: *const EventCB, event_id: u32, mask: u32, mode: u32, timeout: u32) {
        let params = [
            event_cb as usize, 
            event_id as usize, 
            mask as usize, 
            mode as usize, 
            timeout as usize
        ];
        los_trace(EVENT_READ, event_cb as usize, &params);
    }
    
    /// 追踪事件写入
    pub fn trace_event_write(event_cb: *const EventCB, event_id: u32, events: u32) {
        let params = [event_cb as usize, event_id as usize, events as usize];
        los_trace(EVENT_WRITE, event_cb as usize, &params);
    }
    
    /// 追踪事件清除
    pub fn trace_event_clear(event_cb: *const EventCB, event_id: u32, events: u32) {
        let params = [event_cb as usize, event_id as usize, events as usize];
        los_trace(EVENT_CLEAR, event_cb as usize, &params);
    }
}