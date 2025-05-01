#![no_std]
#![no_main]

use panic_halt as _;

pub mod event;
pub mod utils;

use crate::utils::printf::dprintf;

#[unsafe(export_name = "LOS_HelloRust")]
pub extern "C" fn hello_rust() {
    unsafe {
        dprintf(b"Hello, Rust!\0" as *const u8);
    }
}
