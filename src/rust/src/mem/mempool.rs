#[cfg(feature = "task_static_allocation")]
use super::memstat::Memstat;

#[repr(C)]
pub struct LosMemPoolInfo {
    pub pool: *mut core::ffi::c_void,
    pub pool_size: u32,
    #[cfg(feature = "task_static_allocation")]
    pub stat: Memstat,
}

/// 内存池状态信息
#[repr(C)]
// #[derive(Debug, Copy, Clone)]
pub struct LosMemPoolStatus {
    pub total_used_size: u32,
    pub total_free_size: u32,
    pub max_free_node_size: u32,
    pub used_node_num: u32,
    pub free_node_num: u32,
    #[cfg(feature = "task_static_allocation")]
    pub usage_water_line: u32,
}

impl Default for LosMemPoolStatus {
    fn default() -> Self {
        {
            LosMemPoolStatus {
                total_used_size: 0,
                total_free_size: 0,
                max_free_node_size: 0,
                used_node_num: 0,
                free_node_num: 0,
                #[cfg(feature = "task_static_allocation")]
                usage_water_line: 0,
            }
        }
    }
}
