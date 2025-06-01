use crate::utils::bitmap::{clear_bit, get_highest_bit, get_lowest_bit, set_bit};

/// 设置位图中的指定位置
#[unsafe(export_name = "LOS_BitmapSet")]
pub extern "C" fn los_bitmap_set(bitmap: *mut u32, pos: u16) {
    match unsafe { bitmap.as_mut() } {
        Some(b) => set_bit(b, pos),
        None => {}
    }
}

/// 清除位图中的指定位置
#[unsafe(export_name = "LOS_BitmapClr")]
pub extern "C" fn los_bitmap_clr(bitmap: *mut u32, pos: u16) {
    match unsafe { bitmap.as_mut() } {
        Some(b) => {
            clear_bit(b, pos);
        }
        None => {}
    }
}

/// 获取位图中最高有效位的位置
#[unsafe(export_name = "LOS_HighBitGet")]
pub extern "C" fn los_high_bit_get(bitmap: u32) -> u16 {
    get_highest_bit(bitmap)
}

/// 获取位图中最低有效位的位置
#[unsafe(export_name = "LOS_LowBitGet")]
pub extern "C" fn los_low_bit_get(bitmap: u32) -> u16 {
    get_lowest_bit(bitmap)
}
