#![no_std]
#![no_main]

use semihosting::println;

use crate::interrupt::{disable_interrupts, restore_interrupt_state};
extern crate alloc;

mod config;
mod event;
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
mod ramfs;

#[unsafe(export_name = "HelloRust")]
pub extern "C" fn hello_rust() {
    println!("Hello Rust!");
}

struct MyCriticalSection;
critical_section::set_impl!(MyCriticalSection);

unsafe impl critical_section::Impl for MyCriticalSection {
    unsafe fn acquire() -> critical_section::RawRestoreState {
        let state = disable_interrupts();
        state
    }

    unsafe fn release(restore_state: critical_section::RawRestoreState) {
        restore_interrupt_state(restore_state);
    }
}
