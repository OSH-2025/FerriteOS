#![no_std]
#![no_main]

use panic_halt as _;

pub mod hwi;
pub mod mem;
pub mod spinlock;
pub mod utils;
pub mod config;

pub use config::*;
