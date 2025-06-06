//! 信号量模块实现
//! 
//! 提供计数信号量和二进制信号量的功能，用于任务间同步。

// 子模块定义
pub mod bindings;
pub mod configs;
pub mod core;
pub mod error;
pub mod global;
pub mod macros;
pub mod types;

// 重新导出公共接口
pub use core::{
    create_binary_semaphore,
    create_semaphore,
    delete_semaphore,
    init_semaphore_system,
    semaphore_pend,
    semaphore_post,
};

pub use error::{SemaphoreError, SemaphoreResult};
pub use types::{SemaphoreId, SemaphoreType};

/* // 修改SystemError以支持新的信号量错误类型
#[cfg(not(test))]
mod result_extension {
    use crate::result::SystemError;
    use crate::semaphore1::error::SemaphoreError;
    
    impl From<SemaphoreError> for SystemError {
        fn from(err: SemaphoreError) -> Self {
            SystemError::Semaphore1(err)
        }
    }
} */