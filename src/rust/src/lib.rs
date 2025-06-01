#![no_std]
#![no_main]

use semihosting::println;

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
    println!("Hello Rust!");
}
