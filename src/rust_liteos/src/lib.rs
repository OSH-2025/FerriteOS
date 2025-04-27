#![no_std]
#![no_main]

use core::panic::PanicInfo;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

unsafe extern "C" {
    // #[link_name = "dprintf"]
    pub unsafe fn dprintf(fmt: *const u8, ...) -> ();
}

#[unsafe(export_name = "LOS_HelloRust")]
pub extern "C" fn hello_rust() {
    unsafe {
        dprintf(b"Hello, Rust!\0" as *const u8);
    }
}

/// 定义无效位索引常量
pub const LOS_INVALID_BIT_INDEX: u16 = 32;

const OS_BITMAP_MASK: u16 = 0x1F;

/// 设置位图中的指定位置
#[unsafe(export_name = "LOS_BitmapSet")]
pub extern "C" fn bitmap_set(bitmap: *mut u32, pos: u16) {
    if bitmap.is_null() {
        return;
    }
    unsafe {
        *bitmap |= 1 << (pos & OS_BITMAP_MASK);
    }
}

/// 清除位图中的指定位置
#[unsafe(export_name = "LOS_BitmapClr")]
pub extern "C" fn bitmap_clr(bitmap: *mut u32, pos: u16) {
    if bitmap.is_null() {
        return;
    }
    unsafe {
        *bitmap &= !(1 << (pos & OS_BITMAP_MASK));
    }
}

/// 获取位图中最高有效位的位置
#[unsafe(export_name = "LOS_HighBitGet")]
pub extern "C" fn high_bit_get(bitmap: u32) -> u16 {
    if bitmap == 0 {
        return LOS_INVALID_BIT_INDEX;
    }
    OS_BITMAP_MASK - bitmap.leading_zeros() as u16
}

/// 获取位图中最低有效位的位置
#[unsafe(export_name = "LOS_LowBitGet")]
pub extern "C" fn low_bit_get(bitmap: u32) -> u16 {
    if bitmap == 0 {
        return LOS_INVALID_BIT_INDEX;
    }

    bitmap.trailing_zeros() as u16
}
