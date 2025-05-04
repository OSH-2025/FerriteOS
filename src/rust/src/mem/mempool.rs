use super::memstat::Memstat;

#[repr(C)]
pub struct LosMemPoolInfo {
    /// Starting address of a memory pool
    pub pool: *mut core::ffi::c_void,
    /// Memory pool size
    pub pool_size: u32,
    /// Memory statistics (enabled with LOSCFG_MEM_TASK_STAT)
    pub stat: Memstat,
}

/// 内存池状态信息
#[repr(C)]
// #[derive(Debug, Copy, Clone)]
pub struct LosMemPoolStatus {
    /// 总已使用内存大小
    pub total_used_size: u32,
    /// 总空闲内存大小
    pub total_free_size: u32,
    /// 最大空闲节点大小
    pub max_free_node_size: u32,
    /// 已使用节点数量
    pub used_node_num: u32,
    /// 空闲节点数量
    pub free_node_num: u32,
    /// 内存使用水线
    pub usage_water_line: u32,
}
