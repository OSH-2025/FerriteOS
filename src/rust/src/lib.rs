#![no_std]
#![no_main]

use semihosting::println;

mod config;
mod errno;
mod event;
mod ffi;
mod hwi;
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
