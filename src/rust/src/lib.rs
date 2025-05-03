#![no_std]
#![no_main]

use panic_halt as _;

#[cfg(feature = "event")]
pub mod event;
// pub mod mem;
pub mod utils;
