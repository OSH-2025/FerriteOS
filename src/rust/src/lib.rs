#![no_std]
#![no_main]

mod config;
mod error;
#[cfg(feature = "ipc_event")]
mod event;
mod exception;
mod ffi;
mod interrupt;
mod mem;
mod percpu;
mod queue;
mod result;
mod stack;
mod swtmr;
mod task;
mod tick;
mod utils;

#[unsafe(export_name = "HelloRust")]
pub extern "C" fn hello_rust() {
    println_release!("Hello Rust!");
}

// #[macro_export]
// macro_rules! os_check_null_return {
//     ($param:expr) => {
//         if $param.is_null() {
//             crate::utils::printf::dprintf(b"Null pointer detected at\n\0".as_ptr());
//             return;
//         }
//     };
// }

// use core::ffi::c_void;
// use core::fmt;
