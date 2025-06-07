// #![allow(unsafe_code)]
#![allow(static_mut_refs)]
// #![allow(unsafe_op_in_unsafe_fn)]

use super::types::{LosQueueCB, LosDlList, QueueReadWrite, QueueInfoS};
use super::error::{
    LOS_OK, 
    // LOS_ERRNO_QUEUE_NO_MEMORY,
    LOS_ERRNO_QUEUE_CREAT_PTR_NULL,
    LOS_ERRNO_QUEUE_SIZE_TOO_BIG,
    LOS_ERRNO_QUEUE_PARA_ISZERO,
    LOS_ERRNO_QUEUE_CB_UNAVAILABLE,
    LOS_ERRNO_QUEUE_CREATE_NO_MEMORY,
    LOS_ERRNO_QUEUE_INVALID,
    LOS_ERRNO_QUEUE_READ_PTR_NULL,
    LOS_ERRNO_QUEUE_READSIZE_IS_INVALID,
    LOS_ERRNO_QUEUE_READ_IN_INTERRUPT,
    LOS_ERRNO_QUEUE_WRITE_PTR_NULL,
    LOS_ERRNO_QUEUE_WRITESIZE_ISZERO,
    LOS_ERRNO_QUEUE_WRITE_IN_INTERRUPT,
    LOS_USED,
    LOS_ERRNO_QUEUE_NOT_CREATE,
    LOS_ERRNO_QUEUE_READ_SIZE_TOO_SMALL,
    LOS_ERRNO_QUEUE_WRITE_SIZE_TOO_BIG,
    LOS_ERRNO_QUEUE_ISEMPTY,
    LOS_ERRNO_QUEUE_ISFULL,
    LOS_ERRNO_QUEUE_PEND_IN_LOCK,
    LOS_ERRNO_QUEUE_TIMEOUT,
    LOS_ERRNO_QUEUE_NOT_FOUND,
    LOS_ERRNO_QUEUE_IN_TSKUSE,
    LOS_ERRNO_QUEUE_IN_TSKWRITE,
    LOS_ERRNO_QUEUE_PTR_NULL,
};
use core::mem::MaybeUninit;

// 队列相关常量
pub const KERNEL_QUEUE_LIMIT: usize = 1024; // 假设为1024，实际值需根据C代码确定

// 队列内存类型
pub const OS_QUEUE_ALLOC_DYNAMIC: u8 = 0;
pub const OS_QUEUE_ALLOC_STATIC: u8 = 1;

// 超时相关常量
pub const LOS_NO_WAIT: u32 = 0;

// 全局变量
static mut QUEUE_ARRAY: MaybeUninit<[LosQueueCB; KERNEL_QUEUE_LIMIT]> = MaybeUninit::uninit();
static mut G_ALL_QUEUE: *mut LosQueueCB = core::ptr::null_mut();
static mut G_FREE_QUEUE_LIST: LosDlList = LosDlList {
    pst_prev: core::ptr::null_mut(),
    pst_next: core::ptr::null_mut(),
};

// 外部内存池和内存分配函数声明
unsafe extern "C" {
    static m_aucSysMem1: *mut u8;
    
    fn LOS_MemAlloc(pool: *mut u8, size: u32) -> *mut core::ffi::c_void;
    fn LOS_MemFree(pool: *mut u8, ptr: *mut core::ffi::c_void) -> u32;
}

// 引入日志宏（假设存在）
unsafe extern "C" {
    #[allow(dead_code)]
    fn PRINT_ERR(format: *const u8, ...);
    
    // 任务相关外部函数
    fn OsTaskWait(list: *mut LosDlList, task_status: u32, timeout: u32);
    fn OsSchedResched();
    fn OsCurrTaskGet() -> *mut TaskCB;
    fn OsTaskWake(task: *mut TaskCB, task_status: u32);
    fn OsPreemptableInSched() -> bool;
    fn LOS_MpSchedule(cpus: u32);
    fn LOS_Schedule();
    
    // 获取调度器锁的函数
    fn LOS_IntLock() -> u32;
    fn LOS_IntRestore(intSave: u32);
    // fn LOS_SpinLock(lock: *mut u32) -> u32;
    // fn LOS_SpinUnlock(lock: *mut u32, intSave: u32);
}

// 任务控制块结构体（简化版）
#[repr(C)]
struct TaskCB {
    task_status: u32,
    // 其他字段根据需要添加
}

// 打印错误信息的宏（简化实现，实际应使用正确的日志系统）
macro_rules! print_err {
    ($fmt:expr) => {
        unsafe {
            let c_string = concat!($fmt, "\0");
            PRINT_ERR(c_string.as_ptr());
        }
    };
    ($fmt:expr, $($arg:expr),*) => {
        unsafe {
            let c_string = concat!($fmt, "\0");
            PRINT_ERR(c_string.as_ptr(), $($arg),*);
        }
    };
}

// 链表操作函数
#[inline]
pub fn los_list_init(list: *mut LosDlList) {
    // 初始化双向链表，让节点指向自身
    unsafe {
    (*list).pst_next = list;
    (*list).pst_prev = list;
    }
}

#[inline]
pub fn los_list_tail_insert(list: *mut LosDlList, node: *mut LosDlList) {
    // 在链表尾部插入节点
    unsafe {
    (*node).pst_next = list;
    (*node).pst_prev = (*list).pst_prev;
    (*(*list).pst_prev).pst_next = node;
    (*list).pst_prev = node;
    }
}

/// 获取链表中的第一个节点
#[inline]
fn los_dl_list_first(list: *mut LosDlList) -> *mut LosDlList {
    unsafe {
        (*list).pst_next
    }
}

/// 检查链表是否为空
#[inline]
fn los_list_empty(list: *mut LosDlList) -> bool {
    unsafe {
        (*list).pst_next == list
    }
}

/// 从链表中删除一个节点
#[inline]
fn los_list_delete(node: *mut LosDlList) {
    unsafe {
        (*(*node).pst_prev).pst_next = (*node).pst_next;
        (*(*node).pst_next).pst_prev = (*node).pst_prev;
        (*node).pst_next = core::ptr::null_mut();
        (*node).pst_prev = core::ptr::null_mut();
    }
}

/// 从链表节点获取包含它的队列控制块
#[inline]
unsafe fn get_queue_list(list_node: *mut LosDlList) -> *mut LosQueueCB {
    // 假设readWriteList[OS_QUEUE_WRITE]的偏移量是已知的
    // 这里简化处理，实际上应该使用container_of宏的等效实现
    let offset = core::mem::size_of::<LosDlList>() * QueueReadWrite::OS_QUEUE_WRITE as usize;
    let qcb_addr = (list_node as usize - offset) as *mut LosQueueCB;
    qcb_addr
}

/// 队列系统初始化函数
/// 
/// 初始化全局队列数组和空闲队列链表
/// 
/// # 返回值
/// 
/// * `LOS_OK` - 初始化成功
pub fn os_queue_init() -> u32 {
    unsafe {
        // 获取数组指针
        let queue_array_ptr = QUEUE_ARRAY.as_mut_ptr() as *mut LosQueueCB;
        G_ALL_QUEUE = queue_array_ptr;
        
        // 初始化每个队列控制块
        for i in 0..KERNEL_QUEUE_LIMIT {
            let queue = &mut *queue_array_ptr.add(i);
            queue.queue_handle = core::ptr::null_mut();
            queue.queue_state = 0;
            queue.queue_mem_type = 0;
            queue.queue_len = 0;
            queue.queue_size = 0;
            queue.queue_id = i as u32;
            queue.queue_head = 0;
            queue.queue_tail = 0;
            queue.readable_writable_cnt = [0, 0];
            
            // 初始化链表
            queue.read_write_list[0].pst_prev = &mut queue.read_write_list[0];
            queue.read_write_list[0].pst_next = &mut queue.read_write_list[0];
            queue.read_write_list[1].pst_prev = &mut queue.read_write_list[1];
            queue.read_write_list[1].pst_next = &mut queue.read_write_list[1];
            queue.mem_list.pst_prev = &mut queue.mem_list;
            queue.mem_list.pst_next = &mut queue.mem_list;
        }
        
        // 初始化空闲队列链表
        los_list_init(&mut G_FREE_QUEUE_LIST as *mut _);
        
        // 将所有队列添加到空闲链表
        for i in 0..KERNEL_QUEUE_LIMIT {
            let queue = &mut *queue_array_ptr.add(i);
            let list_ptr = &mut queue.read_write_list[QueueReadWrite::OS_QUEUE_WRITE as usize] as *mut LosDlList;
            los_list_tail_insert(&mut G_FREE_QUEUE_LIST as *mut _, list_ptr);
        }
        
        // 返回成功
        LOS_OK
    }
}
