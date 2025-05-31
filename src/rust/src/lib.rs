#![no_std]
#![no_main]

use semihosting::println;

mod config;
mod err;
mod error;
mod event;
mod exc;
mod ffi;
mod interrupt;
mod mem;
mod percpu;
mod queue;
mod swtmr;
mod task;
mod utils;

#[unsafe(export_name = "HelloRust")]
pub extern "C" fn hello_rust() {
    println!("Hello Rust!");
}
