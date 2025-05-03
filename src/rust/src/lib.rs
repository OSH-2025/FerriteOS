#![no_std]
#![no_main]

use panic_halt as _;

// #[cfg(feature = "event")]
// pub mod event;
// #[cfg(feature = "mem")]
pub mod mem;
pub mod utils;
pub mod bindings;
