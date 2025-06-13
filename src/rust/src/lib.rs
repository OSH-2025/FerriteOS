#![no_std]
#![no_main]

use semihosting::println;
extern crate alloc;

mod config;
mod error;
mod event;
mod exception;
mod ffi;
mod interrupt;
mod memory;
mod mutex;
mod percpu;
mod queue;
mod result;
mod semaphore;
mod stack;
mod task;
mod tick;
mod timer;
mod utils;

#[unsafe(export_name = "HelloRust")]
pub extern "C" fn hello_rust() {
    println!("Hello Rust!");
}
