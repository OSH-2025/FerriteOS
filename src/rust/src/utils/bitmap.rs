/// 定义无效位索引常量
const INVALID_BIT_INDEX: u16 = 32;
const BITMAP_MASK: u16 = 0x1F;

/// 设置位图中的指定位
///
/// # Arguments
/// * `bitmap` - 位图指针
/// * `pos` - 要设置的位位置(0-31)
pub fn set_bit(bitmap: &mut u32, pos: u16) {
    *bitmap |= 1u32 << (pos & BITMAP_MASK);
}

/// 清除位图中的指定位
///
/// # Arguments
/// * `bitmap` - 位图指针
/// * `pos` - 要清除的位位置(0-31)
pub fn clear_bit(bitmap: &mut u32, pos: u16) {
    *bitmap &= !(1u32 << (pos & BITMAP_MASK));
}

/// 获取位图中的最高有效位位置
///
/// # Arguments
/// * `bitmap` - 位图值
///
/// # Returns
/// 最高有效位的位置(0-31)，如果位图为0则返回INVALID_BIT_INDEX
pub fn get_highest_bit(bitmap: u32) -> u16 {
    if bitmap == 0 {
        return INVALID_BIT_INDEX;
    }
    BITMAP_MASK - bitmap.leading_zeros() as u16
}

/// 获取位图中的最低有效位位置
///
/// # Arguments
/// * `bitmap` - 位图值
///
/// # Returns
/// 最低有效位的位置(0-31)，如果位图为0则返回INVALID_BIT_INDEX
pub fn get_lowest_bit(bitmap: u32) -> u16 {
    if bitmap == 0 {
        return INVALID_BIT_INDEX;
    }
    bitmap.trailing_zeros() as u16
}
