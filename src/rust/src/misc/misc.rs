//! 杂项函数模块 - LiteOS 杂项功能的 Rust 实现
//! 
//! 本模块包含系统的实用工具函数，如内存对齐、休眠、内存转储和数组排序等功能。

use core::ffi::c_void;
use core::ptr::{self, null_mut};

use crate::print_exc_info;
// use crate::task::OsTaskDelay;
// use crate::task::LOS_MS2Tick;

/// 用于条件编译的系统配置
#[cfg(feature = "lib_configurable")]
use crate::arch::BOOL;

/// 外部函数声明
unsafe extern "C" {
    fn PrintkWrapper(fmt: *const u8, ...);
    
    #[cfg(feature = "shell_excinfo_dump")]
    fn WriteExcInfoToBuf(fmt: *const u8, ...);

    fn LOS_MS2Tick(millisec: u32) -> u32;
    fn LOS_TaskDelay(tick: u32) -> u32;
}

// /// 导入在Shell异常信息转储时需要的函数
// #[cfg(feature = "shell_excinfo_dump")]
// use crate::exc::WriteExcInfoToBuf;

/// 可配置系统参数
#[cfg(feature = "lib_configurable")]
pub struct ConfigurableParams {
    pub os_sys_clock: u32,
    pub sem_limit: u32,
    pub mux_limit: u32,
    pub queue_limit: u32,
    pub swtmr_limit: u32,
    pub task_limit: u32,
    pub minus_one_tick_per_second: u32,
    pub task_min_stk_size: u32,
    pub task_idle_stk_size: u32,
    pub task_swtmr_stk_size: u32,
    pub task_dflt_stk_size: u32,
    pub time_slice_time_out: u32,
    pub nx_enabled: BOOL,
    pub dl_nx_heap_base: usize,
    pub dl_nx_heap_size: u32,
}

/// 全局可配置参数
#[cfg(feature = "lib_configurable")]
#[unsafe(no_mangle)]
pub static mut G_CONFIGURABLE_PARAMS: ConfigurableParams = ConfigurableParams {
    os_sys_clock: 0,
    sem_limit: 0,
    mux_limit: 0,
    queue_limit: 0,
    swtmr_limit: 0,
    task_limit: 0,
    minus_one_tick_per_second: 0,
    task_min_stk_size: 0,
    task_idle_stk_size: 0,
    task_swtmr_stk_size: 0,
    task_dflt_stk_size: 0,
    time_slice_time_out: 0,
    nx_enabled: 0,
    dl_nx_heap_base: 0,
    dl_nx_heap_size: 0,
};

/// 内核跟踪函数钩子类型
#[cfg(feature = "kernel_trace")]
pub type TraceEventHook = Option<unsafe extern "C" fn(hook_type: u32, identity: u32, param1: usize, param2: usize, param3: usize)>;

/// 内核跟踪转储钩子类型
#[cfg(feature = "kernel_trace")]
pub type TraceDumpHook = Option<unsafe extern "C" fn()>;

/// 内核跟踪事件钩子
#[cfg(feature = "kernel_trace")]
#[unsafe(no_mangle)]
pub static mut G_TRACE_EVENT_HOOK: TraceEventHook = None;

/// 内核跟踪转储钩子
#[cfg(feature = "kernel_trace")]
#[unsafe(no_mangle)]
pub static mut G_TRACE_DUMP_HOOK: TraceDumpHook = None;

// 添加钩子函数类型定义
/// 内存监控初始化钩子类型
#[cfg(feature = "kernel_lms")]
pub type LmsInitHook = Option<unsafe extern "C" fn()>;

/// 内存监控函数钩子类型
#[cfg(feature = "kernel_lms")]
pub type LmsFuncHook = Option<unsafe extern "C" fn(ptr: *mut c_void) -> i32>;

/// 内存监控初始化钩子
#[cfg(feature = "kernel_lms")]
#[unsafe(no_mangle)]
pub static mut g_lmsMemInitHook: LmsInitHook = None;

/// 内存监控分配钩子
#[cfg(feature = "kernel_lms")]
#[unsafe(no_mangle)]
pub static mut g_lmsMallocHook: LmsFuncHook = None;

/// 内存监控释放钩子
#[cfg(feature = "kernel_lms")]
#[unsafe(no_mangle)]
pub static mut g_lmsFreeHook: LmsFuncHook = None;

/// 地址对齐
///
/// # 参数
///
/// * `addr` - 待对齐的地址
/// * `boundary` - 对齐边界
///
/// # 返回值
///
/// * 对齐后的地址
#[unsafe(no_mangle)]
pub extern "C" fn LOS_Align(addr: usize, boundary: u32) -> usize {
    (addr + boundary as usize - 1) & !((boundary as usize) - 1)
}

/// 毫秒级休眠
///
/// # 参数
///
/// * `msecs` - 休眠毫秒数
#[unsafe(no_mangle)]
pub extern "C" fn LOS_Msleep(msecs: u32) {
    let mut interval; // 改为mut以允许修改
    
    if msecs == 0 {
        interval = 0; // 值为0表示直接调度
    } else {
        unsafe {
            interval = LOS_MS2Tick(msecs);
        }
        // 添加一个tick补偿不准确的tick计数
        if interval < u32::MAX {
            interval += 1;  // 使用+=赋值，而不是返回表达式结果
        }
        // 不需要else块，因为interval已经有值
    }
    
    // 使用外部声明的LOS_TaskDelay函数
    unsafe {
        let _ = LOS_TaskDelay(interval);
    }
}

/// 字节为单位转储内存内容
///
/// # 参数
///
/// * `length` - 要转储的内存长度
/// * `addr` - 内存起始地址
#[unsafe(no_mangle)]
pub extern "C" fn OsDumpMemByte(length: usize, addr: usize) {
    const SIZE_OF_UINTPTR: usize = core::mem::size_of::<usize>();
    const SIZE_OF_CHAR_PTR: usize = core::mem::size_of::<*const u8>();
    
    let data_len = LOS_Align(length, SIZE_OF_UINTPTR as u32); // ALIGN宏
    let align_addr = addr & !(SIZE_OF_UINTPTR - 1); // TRUNCATE宏
    
    if data_len == 0 || align_addr == 0 {
        return;
    }
    
    let mut count = 0;
    let mut current_addr = align_addr;
    let mut remaining = data_len;
    
    while remaining > 0 {
        // 使用IS_ALIGNED宏：((value) & ((alignSize) - 1)) == 0
        if ((count as usize) & (SIZE_OF_CHAR_PTR - 1)) == 0 {
            unsafe {
                PrintkWrapper(b"\n 0x%lx :\0".as_ptr(), current_addr as *mut usize);
                
                #[cfg(feature = "shell_excinfo_dump")]
                WriteExcInfoToBuf(b"\n 0x%lx :\0".as_ptr(), current_addr as *mut usize);
            }
        }
        
        unsafe {
            #[cfg(target_pointer_width = "64")]
            PrintkWrapper(b"%0+16lx \0".as_ptr(), *(current_addr as *const usize));
            
            #[cfg(target_pointer_width = "32")]
            PrintkWrapper(b"%0+8lx \0".as_ptr(), *(current_addr as *const usize));
            
            #[cfg(all(feature = "shell_excinfo_dump", target_pointer_width = "64"))]
            WriteExcInfoToBuf(b"0x%0+16x \0".as_ptr(), *(current_addr as *const usize));
            
            #[cfg(all(feature = "shell_excinfo_dump", target_pointer_width = "32"))]
            WriteExcInfoToBuf(b"0x%0+8x \0".as_ptr(), *(current_addr as *const usize));
        }
        
        current_addr += SIZE_OF_UINTPTR;
        remaining -= SIZE_OF_UINTPTR;
        count += 1;
    }
    
    unsafe {
        PrintkWrapper(b"\n\0".as_ptr());
        
        #[cfg(feature = "shell_excinfo_dump")]
        WriteExcInfoToBuf(b"\n\0".as_ptr());
    }
}

/// 调试特性下的排序参数
#[cfg(any(feature = "debug_semaphore", feature = "debug_mutex", feature = "debug_queue"))]
#[repr(C)]
pub struct SortParam {
    pub ctrl_blocks: *const c_void,
    pub sort_array: *mut u32,
    pub ctrl_block_cnt: u32,
}

/// 排序时的比较函数类型
#[cfg(any(feature = "debug_semaphore", feature = "debug_mutex", feature = "debug_queue"))]
pub type OsCompareFunc = unsafe extern "C" fn(sort_param: *const SortParam, left: u32, right: u32) -> bool;

/// 数组排序函数
///
/// # 参数
///
/// * `sort_array` - 待排序的数组
/// * `start` - 起始索引
/// * `end` - 结束索引
/// * `sort_param` - 排序参数
/// * `compare_func` - 比较函数
#[cfg(any(feature = "debug_semaphore", feature = "debug_mutex", feature = "debug_queue"))]
#[unsafe(no_mangle)]
pub extern "C" fn OsArraySort(
    sort_array: *mut u32,
    start: u32,
    end: u32,
    sort_param: *const SortParam,
    compare_func: OsCompareFunc,
) {
    let mut left = start;
    let mut right = end;
    let mut idx = start;
    let pivot = unsafe { *sort_array.add(start as usize) };
    
    while left < right {
        while left < right && 
              unsafe { *sort_array.add(right as usize) < (*sort_param).ctrl_block_cnt } && 
              pivot < unsafe { (*sort_param).ctrl_block_cnt } && 
              unsafe { compare_func(sort_param, *sort_array.add(right as usize), pivot) } {
            right -= 1;
        }
        
        if left < right {
            unsafe {
                *sort_array.add(left as usize) = *sort_array.add(right as usize);
            }
            idx = right;
            left += 1;
        }
        
        while left < right && 
              unsafe { *sort_array.add(left as usize) < (*sort_param).ctrl_block_cnt } && 
              pivot < unsafe { (*sort_param).ctrl_block_cnt } && 
              unsafe { compare_func(sort_param, pivot, *sort_array.add(left as usize)) } {
            left += 1;
        }
        
        if left < right {
            unsafe {
                *sort_array.add(right as usize) = *sort_array.add(left as usize);
            }
            idx = left;
            right -= 1;
        }
    }
    
    unsafe {
        *sort_array.add(idx as usize) = pivot;
    }
    
    if start < idx {
        OsArraySort(sort_array, start, idx - 1, sort_param, compare_func);
    }
    
    if idx < end {
        OsArraySort(sort_array, idx + 1, end, sort_param, compare_func);
    }
}