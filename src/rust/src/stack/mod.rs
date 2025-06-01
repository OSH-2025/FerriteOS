//! 栈水位线检测功能
pub mod global;
pub mod types;

use crate::{
    config::{STACK_INIT_PATTERN, STACK_MAGIC_WORD},
    error::{StackError, SystemError, SystemResult},
};

/// 获取栈的水位线（最大使用量）
///
/// # Arguments
/// * `stack_top` - 栈顶地址
/// * `stack_bottom` - 栈底地址
///
/// # Returns
/// 栈使用情况的检测结果
pub fn get_stack_waterline(stack_top: &usize, stack_bottom: &usize) -> SystemResult<u32> {
    // 检查栈魔数
    if *stack_top != STACK_MAGIC_WORD {
        return Err(SystemError::Stack(StackError::Corrupted));
    }

    // 从栈顶开始搜索未初始化的区域
    let stack_top_ptr = stack_top as *const usize;
    let stack_bottom_ptr = stack_bottom as *const usize;

    unsafe {
        let mut current_ptr = stack_top_ptr.add(1); // 跳过魔数

        // 搜索第一个非初始化模式的位置
        while current_ptr < stack_bottom_ptr && *current_ptr == STACK_INIT_PATTERN {
            current_ptr = current_ptr.add(1);
        }

        // 计算使用的字节数
        let used_words = stack_bottom_ptr.offset_from(current_ptr);

        let used_bytes = if used_words == 0 {
            0
        } else {
            (used_words as usize * core::mem::size_of::<usize>())
                + core::mem::size_of::<*const u8>()
        };

        Ok(used_bytes as u32)
    }
}
