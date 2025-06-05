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
mod mutex;
mod percpu;
mod queue;
mod result;
mod semaphore;
mod stack;
mod swtmr;
mod task;
mod tick;
mod utils;

#[unsafe(export_name = "HelloRust")]
pub extern "C" fn hello_rust() {
    println_release!("Hello Rust!");
}
