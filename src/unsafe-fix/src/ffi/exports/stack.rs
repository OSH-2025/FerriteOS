use crate::{
    config::{NOK, OK},
    stack::{
        get_stack_waterline,
        global::{get_stack_info, register_stack_info},
        types::StackInfo,
    },
};

#[unsafe(export_name = "OsStackWaterLineGet")]
pub extern "C" fn os_stack_water_line_get(
    stack_bottom: *const usize,
    stack_top: *const usize,
    peak_used: *mut u32,
) -> u32 {
    const INVALID_WATERLINE: u32 = u32::MAX;
    let pead_used = unsafe { &mut *peak_used };

    let bottom = unsafe { &*stack_bottom };
    let top = unsafe { &*stack_top };

    match get_stack_waterline(top, bottom) {
        Ok(used) => {
            *pead_used = used;
            OK
        }
        Err(_) => {
            *pead_used = INVALID_WATERLINE;
            NOK
        }
    }
}

/// C兼容的栈信息注册函数
#[unsafe(export_name = "OsExcStackInfoReg")]
pub extern "C" fn os_exc_stack_info_reg(stack_info: *const StackInfo, stack_num: u32) {
    if !stack_info.is_null() && stack_num > 0 {
        let info_slice = unsafe { core::slice::from_raw_parts(stack_info, stack_num as usize) };
        register_stack_info(info_slice);
    }
}

/// C兼容的栈信息获取函数
#[unsafe(export_name = "OsGetStackInfo")]
pub extern "C" fn os_get_stack_info(stack_info: *mut *const StackInfo, stack_num: *mut u32) {
    if stack_info.is_null() || stack_num.is_null() {
        return;
    }

    let (info, num) = get_stack_info();
    unsafe {
        *stack_info = if let Some(slice) = info {
            slice.as_ptr()
        } else {
            core::ptr::null()
        };
        *stack_num = num;
    }
}
