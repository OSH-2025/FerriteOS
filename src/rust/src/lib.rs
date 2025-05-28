#![no_std]
#![no_main]

use semihosting::println;

mod config;
mod event;
mod ffi;
mod hwi;
mod mem;
mod percpu;
mod swtmr;
mod task;
mod utils;
mod errno;

#[unsafe(export_name = "HelloRust")]
pub extern "C" fn hello_rust() {
    println!("Hello Rust!");
}
