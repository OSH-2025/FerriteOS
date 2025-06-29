use core::alloc::Layout;
use core::ffi::c_void;
use core::ptr::{addr_of, null_mut};

use linked_list_allocator::LockedHeap;
use semihosting::println;

#[repr(C)]
struct MemHeader {
    size: usize,  // 用户请求的内存大小 (不包含 Header 本身)
    align: usize, // 用户请求的对齐边界
}

const HEADER_SIZE: usize = core::mem::size_of::<MemHeader>();
const MIN_HEADER_ALIGN: usize = core::mem::align_of::<MemHeader>();
const ALIGNED_HEADER_SIZE: usize = (HEADER_SIZE + MIN_HEADER_ALIGN - 1) & !(MIN_HEADER_ALIGN - 1);

#[unsafe(export_name = "g_sys_mem_addr_end")]
pub static mut G_SYS_MEM_ADDR_END: usize = 0;

unsafe extern "C" {
    pub static __heap_start: u8;
}

#[inline]
pub const fn os_sys_mem_addr() -> *mut c_void {
    addr_of!(__heap_start) as *mut c_void
}

#[inline]
pub fn os_sys_mem_size() -> usize {
    let sys_mem_end = unsafe { G_SYS_MEM_ADDR_END };
    let aligned_heap_start = ((os_sys_mem_addr() as usize) + (63)) & !(63);
    sys_mem_end - aligned_heap_start
}

// 全局分配器
#[global_allocator]
static ALLOCATOR: LockedHeap = LockedHeap::empty();

// 初始化分配器
#[unsafe(export_name = "OsMemSystemInit")]
pub extern "C" fn init_allocator() -> u32 {
    println!("Initializing allocator with static heap...");
    unsafe {
        ALLOCATOR
            .lock()
            .init(os_sys_mem_addr() as *mut u8, os_sys_mem_size());
    }
    0
}

// 分配内存的 C 接口
#[unsafe(export_name = "LOS_MemAlloc")]
pub extern "C" fn malloc(size: usize) -> *mut c_void {
    if size == 0 {
        return null_mut();
    }

    let requested_align = 8;
    let actual_align = core::cmp::max(MIN_HEADER_ALIGN, requested_align);

    // 总共需要分配的内存大小：Header 空间 + 用户数据空间
    let total_alloc_size = ALIGNED_HEADER_SIZE + size;

    let layout = match Layout::from_size_align(total_alloc_size, actual_align) {
        Ok(l) => l,
        Err(_) => return null_mut(),
    };

    unsafe {
        let alloc_ptr = alloc::alloc::alloc(layout);
        if alloc_ptr.is_null() {
            return null_mut(); // 分配失败
        }

        // 在分配的内存起始处写入 Header
        let header_ptr = alloc_ptr as *mut MemHeader;
        core::ptr::write(
            header_ptr,
            MemHeader {
                size: size,
                align: requested_align,
            },
        );

        // 返回给 C 的指针是跳过 Header 的地址
        alloc_ptr.add(ALIGNED_HEADER_SIZE) as *mut c_void
    }
}

#[unsafe(export_name = "LOS_MemAllocAlign")]
pub extern "C" fn memalign(size: usize, boundary: usize) -> *mut c_void {
    if size == 0 {
        return null_mut();
    }

    if !boundary.is_power_of_two() {
        return null_mut();
    }

    let requested_align = boundary;
    let actual_align = core::cmp::max(MIN_HEADER_ALIGN, requested_align);

    let total_alloc_size = ALIGNED_HEADER_SIZE + size;

    let layout = match Layout::from_size_align(total_alloc_size, actual_align) {
        Ok(l) => l,
        Err(_) => return null_mut(),
    };

    unsafe {
        let alloc_ptr = alloc::alloc::alloc(layout);
        if alloc_ptr.is_null() {
            return null_mut();
        }

        // 在分配的内存起始处写入 Header
        let header_ptr = alloc_ptr as *mut MemHeader;
        core::ptr::write(
            header_ptr,
            MemHeader {
                size: size,
                align: requested_align,
            },
        );

        // 返回给 C 的指针是跳过 Header 的地址
        alloc_ptr.add(ALIGNED_HEADER_SIZE) as *mut c_void
    }
}

// 释放内存的 C 接口
#[unsafe(export_name = "LOS_MemFree")]
pub extern "C" fn free(ptr: *mut c_void) {
    if ptr.is_null() {
        return;
    }
    unsafe {
        // 通过用户指针回溯到实际分配的起始地址 (包含 Header)
        let actual_alloc_ptr = (ptr as *mut u8).sub(ALIGNED_HEADER_SIZE);
        let header_ptr = actual_alloc_ptr as *mut MemHeader;

        // 读取 Header 信息以构造正确的 Layout
        let header = header_ptr.as_ref().unwrap();
        let original_user_size = header.size;
        let original_user_align = header.align;

        let total_size = ALIGNED_HEADER_SIZE + original_user_size;
        let total_align = core::cmp::max(MIN_HEADER_ALIGN, original_user_align);

        let layout = match Layout::from_size_align(total_size, total_align) {
            Ok(l) => l,
            Err(_) => return,
        };
        alloc::alloc::dealloc(actual_alloc_ptr, layout);
    }
}

#[unsafe(export_name = "LOS_MemTotalSizeGet")]
pub extern "C" fn get_total_size() -> usize {
    os_sys_mem_size()
}

#[unsafe(export_name = "LOS_MemUsedSizeGet")]
pub extern "C" fn get_used_size() -> usize {
    ALLOCATOR.lock().used()
}
