//! 地址对齐工具模块

/// 将地址按指定边界对齐
#[inline]
pub fn align_up(addr: u32, boundary: u32) -> u32 {
    (addr + boundary - 1) & !(boundary - 1)
}

/// 将地址向下对齐到指定边界
#[inline]
#[allow(dead_code)]
pub fn align_down(addr: u32, boundary: u32) -> u32 {
    addr & !(boundary - 1)
}

/// 检查地址是否已对齐
#[inline]
pub fn is_aligned(addr: u32, boundary: u32) -> bool {
    (addr & (boundary - 1)) == 0
}

/// 获取地址对齐所需的填充字节数
#[inline]
#[allow(dead_code)]
pub fn get_align_padding(addr: u32, boundary: u32) -> u32 {
    let aligned = align_up(addr, boundary);
    aligned - addr
}
