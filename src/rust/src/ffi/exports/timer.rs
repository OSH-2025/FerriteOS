use crate::{
    config::OK,
    timer::{timer_create, timer_delete, timer_init, timer_start, timer_stop, timer_time_get, TimerError, TimerHandler, TimerMode},
};

#[unsafe(export_name = "OsSwtmrInit")]
pub fn os_swtmr_init() -> u32 {
    match timer_init() {
        Ok(_) => OK,
        Err(e) => e.into(),
    }
}

#[unsafe(export_name = "LOS_SwtmrCreate")]
pub fn los_swtmr_create(timeout: u32, mode: u8, handler: TimerHandler, id: *mut u32) -> u32 {
    // 检查指针是否为空
    if id.is_null() {
        return TimerError::RetPtrNull.into();
    }

    let mode = match TimerMode::try_from(mode) {
        Ok(m) => m,
        Err(_) => return TimerError::ModeInvalid.into(), // 模式转换失败
    };

    // 调用内部实现创建定时器
    match timer_create(timeout, mode, handler) {
        Ok(timer_id) => {
            unsafe { *id = timer_id.into() };
            OK
        }
        Err(e) => e.into(), // 错误转换为对应的错误码
    }
}

#[unsafe(export_name = "LOS_SwtmrDelete")]
pub fn los_swtmr_delete(timer_id: u32) -> u32 {
    // 调用内部实现删除定时器
    match timer_delete(timer_id.into()) {
        Ok(_) => OK,
        Err(e) => e.into(), // 错误转换为对应的错误码
    }
}

#[unsafe(export_name = "LOS_SwtmrStart")]
pub fn los_swtmr_start(timer_id: u32) -> u32 {
    // 调用内部实现启动定时器
    match timer_start(timer_id.into()) {
        Ok(_) => OK,
        Err(e) => e.into(), // 错误转换为对应的错误码
    }
}

#[unsafe(export_name = "LOS_SwtmrStop")]
pub fn los_swtmr_stop(timer_id: u32) -> u32 {
    // 调用内部实现停止定时器
    match timer_stop(timer_id.into()) {
        Ok(_) => OK,
        Err(e) => e.into(), // 错误转换为对应的错误码
    }
}

#[unsafe(export_name = "LOS_SwtmrTimeGet")]
pub fn los_swtmr_time_get(timer_id: u32, tick: *mut u32) -> u32 {
    // 检查指针是否为空
    if tick.is_null() {
        return TimerError::TickPtrNull.into();
    }

    // 调用内部实现获取定时器剩余时间
    match timer_time_get(timer_id.into()) {
        Ok(t) => {
            unsafe { *tick = t };
            OK
        }
        Err(e) => e.into(), // 错误转换为对应的错误码
    }
}
