//! 地址对齐工具模块

/// 将地址按指定边界对齐
#[inline]
pub(crate) fn align_up(addr: u32, boundary: u32) -> u32 {
    (addr + boundary - 1) & !(boundary - 1)
}

/// 将地址向下对齐到指定边界
// #[inline]
// pub(crate) fn align_down(addr: u32, boundary: u32) -> u32 {
//     addr & !(boundary - 1)
// }

/// 检查地址是否已对齐
#[inline]
pub(crate) fn is_aligned(addr: u32, boundary: u32) -> bool {
    (addr & (boundary - 1)) == 0
}
