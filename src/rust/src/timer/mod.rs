mod api;
mod error;
mod global;
mod init;
mod internal;
mod scan;
mod types;

pub use api::{timer_create, timer_delete, timer_start, timer_stop, timer_time_get};
pub use error::TimerError;
pub use init::timer_init;
pub use scan::timer_scan;
pub use types::{TimerHandler, TimerMode};
