//! 排序算法实现

use core::ffi::c_void;

/// 比较函数类型
pub type CompareFunc = unsafe extern "C" fn(sort_param: *const SortParam, a: u32, b: u32) -> bool;

/// 排序参数结构体
#[repr(C)]
pub struct SortParam {
    pub ctrl_block_cnt: u32,
    pub sort_array: *mut u32,
    // 其他字段根据实际需要添加
}

/// 数组排序函数(快速排序)
/// 
/// # Arguments
/// * `sort_array` - 待排序数组
/// * `start` - 排序起始索引
/// * `end` - 排序结束索引
/// * `sort_param` - 排序参数
/// * `compare_func` - 比较函数
pub fn array_sort(
    sort_array: &mut [u32],
    start: u32,
    end: u32,
    sort_param: &SortParam,
    compare_func: CompareFunc,
) {
    if start >= end || start as usize >= sort_array.len() || end as usize >= sort_array.len() {
        return;
    }
    
    let mut left = start;
    let mut right = end;
    let mut idx = start;
    let pivot = sort_array[start as usize];
    
    unsafe {
        while left < right {
            // 从右向左找小于等于pivot的元素
            while left < right && 
                  sort_array[right as usize] < sort_param.ctrl_block_cnt && 
                  pivot < sort_param.ctrl_block_cnt && 
                  compare_func(sort_param, sort_array[right as usize], pivot) {
                right -= 1;
            }
            
            if left < right {
                sort_array[left as usize] = sort_array[right as usize];
                idx = right;
                left += 1;
            }
            
            // 从左向右找大于pivot的元素
            while left < right && 
                  sort_array[left as usize] < sort_param.ctrl_block_cnt && 
                  pivot < sort_param.ctrl_block_cnt && 
                  compare_func(sort_param, pivot, sort_array[left as usize]) {
                left += 1;
            }
            
            if left < right {
                sort_array[right as usize] = sort_array[left as usize];
                idx = left;
                right -= 1;
            }
        }
    }
    
    sort_array[idx as usize] = pivot;
    
    // 递归排序左半部分
    if start < idx {
        array_sort(sort_array, start, idx - 1, sort_param, compare_func);
    }
    
    // 递归排序右半部分
    if idx < end {
        array_sort(sort_array, idx + 1, end, sort_param, compare_func);
    }
}

/// C兼容的数组排序函数
/// 
/// # Safety
/// 调用者需要确保所有指针参数有效，并且数组索引在有效范围内
#[cfg(any(
    feature = "debug_semaphore",
    feature = "debug_mutex",
    feature = "debug_queue"
))]
#[no_mangle]
pub unsafe extern "C" fn OsArraySort(
    sort_array: *mut u32,
    start: u32,
    end: u32,
    sort_param: *const SortParam,
    compare_func: CompareFunc,
) {
    if sort_array.is_null() || sort_param.is_null() {
        return;
    }
    
    // 计算数组长度（假设end是有效的）
    let len = end as usize + 1;
    
    // 创建切片引用
    let array_slice = core::slice::from_raw_parts_mut(sort_array, len);
    let param_ref = &*sort_param;
    
    // 调用安全的Rust实现
    array_sort(array_slice, start, end, param_ref, compare_func);
}

/// 泛型排序函数
/// 
/// # Arguments
/// * `items` - 待排序的切片
/// * `compare` - 比较函数
pub fn quick_sort<T, F>(items: &mut [T], compare: F)
where
    F: Fn(&T, &T) -> core::cmp::Ordering,
{
    if items.len() <= 1 {
        return;
    }
    
    let (pivot, rest) = items.split_first_mut().unwrap();
    
    let mut left = 0;
    let mut right = rest.len();
    
    // 分区
    for i in 0..right {
        if compare(&rest[i], pivot) == core::cmp::Ordering::Less {
            rest.swap(i, left);
            left += 1;
        }
    }
    
    // 将pivot放到正确位置
    items.swap(0, left);
    
    // 递归排序子数组
    let (left_part, right_part) = items.split_at_mut(left);
    quick_sort(left_part, &compare);
    quick_sort(&mut right_part[1..], &compare);
}

#[cfg(test)]
mod tests {
    use super::*;
    
    #[test]
    fn test_quick_sort() {
        let mut numbers = [5, 2, 9, 1, 5, 6];
        quick_sort(&mut numbers, |a, b| a.cmp(b));
        assert_eq!(numbers, [1, 2, 5, 5, 6, 9]);
        
        let mut strings = ["apple", "banana", "cherry", "date"];
        quick_sort(&mut strings, |a, b| a.cmp(b));
        assert_eq!(strings, ["apple", "banana", "cherry", "date"]);
    }
}