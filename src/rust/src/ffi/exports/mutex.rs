use crate::{
    config::OK,
    mutex::{
        core::{mutex_create, mutex_delete, mutex_init, mutex_pend, mutex_post},
        error::MutexError,
    },
};

#[unsafe(export_name = "OsMuxInit")]
pub extern "C" fn os_mux_init()  {
    mutex_init();
}

#[unsafe(export_name = "LOS_MuxCreate")]
pub extern "C" fn los_mux_create(mux_handle: *mut u32) -> u32 {
    if mux_handle.is_null() {
        return MutexError::PtrNull.into();
    }

    match mutex_create() {
        Ok(handle) => {
            unsafe {
                *mux_handle = handle.into();
            }
            OK
        }
        Err(e) => e.into(),
    }
}

#[unsafe(export_name = "LOS_MuxDelete")]
pub extern "C" fn los_mux_delete(mux_handle: u32) -> u32 {
    match mutex_delete(mux_handle.into()) {
        Ok(()) => OK,
        Err(e) => e.into(),
    }
}

#[unsafe(export_name = "LOS_MuxPend")]
pub extern "C" fn los_mux_pend(mux_handle: u32, timeout: u32) -> u32 {
    match mutex_pend(mux_handle.into(), timeout) {
        Ok(()) => OK,
        Err(e) => e.into(),
    }
}

#[unsafe(export_name = "LOS_MuxPost")]
pub extern "C" fn los_mux_post(mux_handle: u32) -> u32 {
    match mutex_post(mux_handle.into()) {
        Ok(()) => OK,
        Err(e) => e.into(),
    }
}
