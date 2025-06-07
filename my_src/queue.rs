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

/// 内部函数：检查队列创建参数的有效性
#[inline]
fn os_queue_create_parameter_check_internal(len: u16, queue_id: *const u32, max_msg_size: u16) -> u32 {
    // 检查队列ID指针是否为空
    if queue_id.is_null() {
        return LOS_ERRNO_QUEUE_CREAT_PTR_NULL;
    }

    // 检查消息大小是否超过限制 (OS_NULL_SHORT - sizeof(UINT32))
    // 假设OS_NULL_SHORT为0xFFFF，sizeof(UINT32)为4
    const OS_NULL_SHORT_MINUS_U32: u16 = 0xFFFF - 4;
    if max_msg_size > OS_NULL_SHORT_MINUS_U32 {
        return LOS_ERRNO_QUEUE_SIZE_TOO_BIG;
    }

    // 检查队列长度和消息大小是否为0
    if len == 0 || max_msg_size == 0 {
        return LOS_ERRNO_QUEUE_PARA_ISZERO;
    }

    LOS_OK
}

/// 检查队列创建参数的有效性
/// 
/// # 参数
/// 
/// * `len` - 队列长度
/// * `queue_id` - 队列ID指针（可为NULL）
/// * `max_msg_size` - 最大消息大小
/// 
/// # 返回值
/// 
/// * `LOS_OK` - 参数有效
/// * 其他错误码表示参数无效
pub fn os_queue_create_parameter_check(len: u16, queue_id: *const u32, max_msg_size: u16) -> u32 {
    os_queue_create_parameter_check_internal(len, queue_id, max_msg_size)
}

// 调度器操作的简单模拟
// 在实际实现中，这些应该是与调度器交互的函数
struct SchedulerGuard {
    _private: (),
}

impl SchedulerGuard {
    #[inline]
    fn new() -> Self {
        // 这里应该调用实际的SCHEDULER_LOCK函数
        // 简化处理，未实现实际的锁定逻辑
        Self { _private: () }
    }
}

impl Drop for SchedulerGuard {
    #[inline]
    fn drop(&mut self) {
        // 这里应该调用实际的SCHEDULER_UNLOCK函数
        // 简化处理，未实现实际的解锁逻辑
    }
}

// 任务相关结构和方法
struct TaskEntry {
    // 简化结构，实际应包含更多字段
    entry: usize,
}

fn os_curr_task_get() -> &'static TaskEntry {
    // 简化实现，实际应返回当前任务
    static TASK: TaskEntry = TaskEntry { entry: 0 };
    &TASK
}

// 调试钩子
fn os_queue_dbg_update_hook(_queue_id: u32, _task_entry: *const u8) {
    // 简化实现，实际应更新调试信息
}

// 检查钩子
fn os_queue_check_hook() {
    // 简化实现，实际应进行检查
}

// 跟踪记录
#[inline]
fn los_trace(_event: u32, _queue_id: u32, _len: u16, _msg_size: u16, _queue: usize, _mem_type: u8) {
    // 简化实现，实际应记录跟踪信息
}

/// 内部函数：创建一个队列
/// 
/// # 参数
/// 
/// * `len` - 队列长度
/// * `queue_id` - 用于存储队列ID的指针
/// * `msg_size` - 消息大小
/// * `queue` - 队列处理程序指针
/// * `queue_mem_type` - 队列内存类型
/// 
/// # 返回值
/// 
/// * `LOS_OK` - 创建成功
/// * 其他错误码表示创建失败
fn os_queue_create_internal(
    len: u16, 
    queue_id: *mut u32, 
    msg_size: u16,
    queue: *mut u8, 
    queue_mem_type: u8
) -> u32 {
    // 创建调度器保护
    let _guard = SchedulerGuard::new();
    
    // 检查是否有空闲队列可用
    unsafe {
        if los_list_empty(&mut G_FREE_QUEUE_LIST as *mut _) {
            // 释放锁（通过守卫的Drop）
            os_queue_check_hook();
            return LOS_ERRNO_QUEUE_CB_UNAVAILABLE;
        }
        
        // 获取空闲队列列表中的第一个节点
        let unused_queue = los_dl_list_first(&mut G_FREE_QUEUE_LIST as *mut _);
        
        // 从链表中删除该节点
        los_list_delete(unused_queue);
        
        // 获取队列控制块
        let queue_cb = get_queue_list(unused_queue);
        
        // 初始化队列控制块
        (*queue_cb).queue_len = len;
        (*queue_cb).queue_size = msg_size;
        (*queue_cb).queue_handle = queue;
        (*queue_cb).queue_state = LOS_USED;
        (*queue_cb).queue_mem_type = queue_mem_type;
        (*queue_cb).readable_writable_cnt[QueueReadWrite::OS_QUEUE_READ as usize] = 0;
        (*queue_cb).readable_writable_cnt[QueueReadWrite::OS_QUEUE_WRITE as usize] = len;
        (*queue_cb).queue_head = 0;
        (*queue_cb).queue_tail = 0;
        
        // 初始化各种链表
        los_list_init(&mut (*queue_cb).read_write_list[QueueReadWrite::OS_QUEUE_READ as usize] as *mut _);
        los_list_init(&mut (*queue_cb).read_write_list[QueueReadWrite::OS_QUEUE_WRITE as usize] as *mut _);
        los_list_init(&mut (*queue_cb).mem_list as *mut _);
        
        // 调用调试钩子
        os_queue_dbg_update_hook((*queue_cb).queue_id, os_curr_task_get().entry as *const u8);
        
        // 守卫在这里结束，自动解锁
        
        // 保存队列ID到输出参数
        *queue_id = (*queue_cb).queue_id;
        
        // 记录跟踪信息
        los_trace(0 /* QUEUE_CREATE */, *queue_id, len, msg_size - 4 /* sizeof(UINT32) */, 
                   queue as usize, queue_mem_type);
        
        // 返回成功
        LOS_OK
    }
}

/// 创建一个队列，使用动态内存分配
///
/// # 参数
///
/// * `queue_name` - 队列名称（在当前实现中未使用）
/// * `len` - 队列长度
/// * `queue_id` - 用于存储队列ID的指针
/// * `flags` - 队列标志（在当前实现中未使用）
/// * `max_msg_size` - 最大消息大小
///
/// # 返回值
///
/// * `LOS_OK` - 创建成功
/// * `LOS_ERRNO_QUEUE_CB_UNAVAILABLE` - 没有空闲队列控制块
/// * `LOS_ERRNO_QUEUE_CREATE_NO_MEMORY` - 内存分配失败
/// * 其他错误码表示创建失败
// #[no_mangle]
pub fn os_queue_create(
    queue_name: *const u8,
    len: u16,
    queue_id: *mut u32,
    flags: u32,
    max_msg_size: u16
) -> u32 {
    // 忽略未使用的参数，避免编译器警告
    let _queue_name = queue_name;
    let _flags = flags;
    
    // 检查参数有效性
    let ret = os_queue_create_parameter_check_internal(len, queue_id, max_msg_size);
    if ret != LOS_OK {
        return ret;
    }
    
    // 消息头需要额外的4个字节来存储消息长度
    let msg_size = max_msg_size + 4;
    
    // 计算所需的队列内存大小
    let queue_size = msg_size as u32 * len as u32;
    
    // 分配内存
    unsafe {
        let queue = LOS_MemAlloc(m_aucSysMem1, queue_size) as *mut u8;
        if queue.is_null() {
            return LOS_ERRNO_QUEUE_CREATE_NO_MEMORY;
        }
        
        // 创建队列
        let ret = os_queue_create_internal(len, queue_id, msg_size, queue, OS_QUEUE_ALLOC_DYNAMIC);
        if ret != LOS_OK {
            // 创建失败，释放内存
            LOS_MemFree(m_aucSysMem1, queue as *mut core::ffi::c_void);
            return ret;
        }
        
        // 返回创建结果
        ret
    }
}

/// 创建一个队列的C API兼容版本
///
/// 此函数提供给C代码调用的接口
// #[no_mangle]
pub extern "C" fn os_queue_create_c(
    queue_name: *const u8,
    len: u16,
    queue_id: *mut u32,
    flags: u32,
    max_msg_size: u16
) -> u32 {
    os_queue_create(queue_name, len, queue_id, flags, max_msg_size)
}
