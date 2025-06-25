use crate::{
    config::OK,
    event::{
        core::{event_clear, event_destroy, event_init, event_poll, event_read, event_write},
        error::EventError,
        types::EventCB,
    },
    result::SystemError,
};

// C兼容接口
#[unsafe(export_name = "LOS_EventInit")]
pub extern "C" fn los_event_init(event_cb: *mut EventCB) -> u32 {
    if event_cb.is_null() {
        return SystemError::Event(EventError::PtrNull).into();
    }
    unsafe {
        event_init(&mut *event_cb);
    }
    OK
}

#[unsafe(export_name = "LOS_EventDestroy")]
pub extern "C" fn los_event_destroy(event_cb: *mut EventCB) -> u32 {
    if event_cb.is_null() {
        return SystemError::Event(EventError::PtrNull).into();
    }

    unsafe {
        match event_destroy(&mut *event_cb) {
            Ok(()) => OK,
            Err(e) => e.into(),
        }
    }
}

#[unsafe(export_name = "LOS_EventRead")]
pub extern "C" fn los_event_read(
    event_cb: *mut EventCB,
    event_mask: u32,
    mode: u32,
    timeout: u32,
) -> u32 {
    if event_cb.is_null() {
        return SystemError::Event(EventError::PtrNull).into();
    }

    unsafe {
        match event_read(&mut *event_cb, event_mask, mode, timeout) {
            Ok(result) => result,
            Err(e) => e.into(),
        }
    }
}

#[unsafe(export_name = "LOS_EventWrite")]
pub extern "C" fn los_event_write(event_cb: *mut EventCB, events: u32) -> u32 {
    if event_cb.is_null() {
        return SystemError::Event(EventError::PtrNull).into();
    }

    unsafe {
        match event_write(&mut *event_cb, events) {
            Ok(()) => OK,
            Err(e) => e.into(),
        }
    }
}

#[unsafe(export_name = "LOS_EventPoll")]
pub extern "C" fn los_event_poll(event_id: *mut u32, event_mask: u32, mode: u32) -> u32 {
    if event_id.is_null() {
        return SystemError::Event(EventError::PtrNull).into();
    }

    unsafe {
        match event_poll(&mut *event_id, event_mask, mode) {
            Ok(result) => result,
            Err(e) => e.into(),
        }
    }
}

#[unsafe(export_name = "LOS_EventClear")]
pub extern "C" fn los_event_clear(event_cb: *mut EventCB, events: u32) -> u32 {
    if event_cb.is_null() {
        return SystemError::Event(EventError::PtrNull).into();
    }

    unsafe {
        event_clear(&mut *event_cb, events);
    }
    OK
}
