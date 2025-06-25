use crate::{
    config::OK,
    interrupt::{
        clear_interrupt, disable_interrupt, enable_interrupt, get_current_interrupt_number,
        get_interrupt_count, get_interrupt_handler, get_interrupt_nesting_count,
        get_interrupt_version,
        global::{irq_nesting_count_get, irq_nesting_count_set, register_interrupt_controller},
        handle_interrupt, initialize_interrupt, interrupt_entry, is_interrupt_registered,
        register_interrupt, set_interrupt_priority, trigger_interrupt,
        types::{InterruptController, InterruptHandler},
        unregister_interrupt,
    },
};
use core::{ffi::c_char, ptr::addr_of_mut};

#[unsafe(export_name = "OsHwiControllerReg")]
pub extern "C" fn os_hwi_controller_reg(ops: *mut InterruptController) {
    register_interrupt_controller(ops);
}

#[unsafe(export_name = "OsIrqNestingCntGet")]
pub extern "C" fn os_irq_nesting_cnt_get() -> u32 {
    irq_nesting_count_get()
}

#[unsafe(export_name = "OsIrqNestingCntSet")]
pub extern "C" fn os_irq_nesting_cnt_set(val: u32) {
    irq_nesting_count_set(val);
}

#[unsafe(export_name = "OsIntHandle")]
pub extern "C" fn os_int_handle(hwi_num: u32, hwi_form: &mut InterruptHandler) {
    handle_interrupt(hwi_num, hwi_form);
}

#[unsafe(export_name = "OsIntEntry")]
pub extern "C" fn os_int_entry() {
    interrupt_entry();
}

#[unsafe(export_name = "LOS_HwiCreate")]
pub extern "C" fn los_hwi_create(
    hwi_num: u32,
    hwi_prio: u8,
    hwi_handler: Option<extern "C" fn()>,
) -> u32 {
    match register_interrupt(hwi_num, hwi_prio, hwi_handler) {
        Ok(()) => OK,
        Err(err) => err.into(),
    }
}

#[unsafe(export_name = "LOS_HwiDelete")]
pub extern "C" fn los_hwi_delete(hwi_num: u32) -> u32 {
    match unregister_interrupt(hwi_num) {
        Ok(()) => OK,
        Err(err) => err.into(),
    }
}

#[unsafe(export_name = "LOS_HwiTrigger")]
pub extern "C" fn los_hwi_trigger(hwi_num: u32) -> u32 {
    match trigger_interrupt(hwi_num) {
        Ok(()) => OK,
        Err(err) => err.into(),
    }
}

#[unsafe(export_name = "LOS_HwiEnable")]
pub extern "C" fn los_hwi_enable(hwi_num: u32) -> u32 {
    match enable_interrupt(hwi_num) {
        Ok(()) => OK,
        Err(err) => err.into(),
    }
}

#[unsafe(export_name = "LOS_HwiDisable")]
pub extern "C" fn los_hwi_disable(hwi_num: u32) -> u32 {
    match disable_interrupt(hwi_num) {
        Ok(()) => OK,
        Err(err) => err.into(),
    }
}

#[unsafe(export_name = "LOS_HwiClear")]
pub extern "C" fn los_hwi_clear(hwi_num: u32) -> u32 {
    match clear_interrupt(hwi_num) {
        Ok(()) => OK,
        Err(err) => err.into(),
    }
}

#[unsafe(export_name = "LOS_HwiSetPriority")]
pub extern "C" fn los_hwi_set_priority(hwi_num: u32, priority: u8) -> u32 {
    match set_interrupt_priority(hwi_num, priority) {
        Ok(()) => OK,
        Err(err) => err.into(),
    }
}

#[unsafe(export_name = "IntActive")]
pub extern "C" fn int_active() -> u32 {
    get_interrupt_nesting_count()
}

#[unsafe(export_name = "OsHwiInit")]
pub extern "C" fn os_hwi_init() {
    initialize_interrupt();
}

#[unsafe(export_name = "OsGetHwiForm")]
pub extern "C" fn os_get_hwi_form(hwi_num: u32) -> *mut InterruptHandler {
    match get_interrupt_handler(hwi_num) {
        Ok(hwi_info) => addr_of_mut!(*hwi_info),
        Err(_) => core::ptr::null_mut(),
    }
}

#[unsafe(export_name = "OsGetHwiFormCnt")]
pub extern "C" fn os_get_hwi_form_cnt(hwi_num: u32) -> u32 {
    match get_interrupt_count(hwi_num) {
        Ok(count) => count,
        Err(err) => err.into(),
    }
}

#[unsafe(export_name = "OsIntNumGet")]
pub extern "C" fn os_int_num_get() -> u32 {
    match get_current_interrupt_number() {
        Ok(num) => num,
        Err(err) => err.into(),
    }
}

#[unsafe(export_name = "OsIntIsRegisted")]
pub extern "C" fn os_int_is_registed(num: u32) -> bool {
    is_interrupt_registered(num)
}

#[unsafe(export_name = "OsIntVersionGet")]
pub extern "C" fn os_int_version_get() -> *const c_char {
    get_interrupt_version().unwrap_or(core::ptr::null())
}
