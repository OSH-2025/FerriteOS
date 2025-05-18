//! 事件管理模块 - LiteOS 事件机制的 Rust 实现

use core::ptr::null_mut;
use core::ffi::c_void;

use crate::trace::event_trace;
use crate::task::LosTaskCB;

/// 双向链表结构体
#[repr(C)]
pub struct LOS_DL_LIST {
    pub pstPrev: *mut LOS_DL_LIST,
    pub pstNext: *mut LOS_DL_LIST,
}

/// 事件控制块结构
#[repr(C)]
pub struct EventCB {
    /// 事件ID，每个位表示一个事件类型
    pub event_id: u32,
    /// 等待该事件的任务列表
    pub event_list: LOS_DL_LIST,
}

#[macro_export]
macro_rules! container_of {
    ($ptr:expr, $container:ty, $field:ident) => {
        ($ptr as usize - core::mem::offset_of!($container, $field)) as *mut $container
    };
}

/// 等待模式：任一事件触发
pub const LOS_WAITMODE_OR: u32 = 0x01;
/// 等待模式：所有事件都触发
pub const LOS_WAITMODE_AND: u32 = 0x02;
/// 等待模式：触发后清除事件
pub const LOS_WAITMODE_CLR: u32 = 0x04;

/// 错误码：事件指针为空
pub const LOS_ERRNO_EVENT_PTR_NULL: u32 = 0x02001400;
/// 错误码：事件掩码无效
pub const LOS_ERRNO_EVENT_EVENTMASK_INVALID: u32 = 0x02001401;
/// 错误码：事件设置位无效
pub const LOS_ERRNO_EVENT_SETBIT_INVALID: u32 = 0x02001402;
/// 错误码：事件读取中断
pub const LOS_ERRNO_EVENT_READ_IN_INTERRUPT: u32 = 0x02001403;
/// 错误码：在锁中读取事件
pub const LOS_ERRNO_EVENT_READ_IN_LOCK: u32 = 0x02001404;
/// 错误码：无效的标志
pub const LOS_ERRNO_EVENT_FLAGS_INVALID: u32 = 0x02001405;
/// 错误码：事件读取超时
pub const LOS_ERRNO_EVENT_READ_TIMEOUT: u32 = 0x02001407;
/// 错误码：不应销毁事件
pub const LOS_ERRNO_EVENT_SHOULD_NOT_DESTORY: u32 = 0x02001408;

/// 错误类型掩码
pub const LOS_ERRTYPE_ERROR: u32 = 0x80000000;

/// 成功返回码
pub const LOS_OK: u32 = 0;

/// 条件事件结构
#[repr(C)]
pub struct EventCond {
    pub real_value: *const u32,
    pub value: u32,
    pub clear_event: u32,
}

// 外部函数声明
unsafe extern "C" {
    fn OsTaskWait(list: *mut c_void, status: u32, timeout: u32);
    fn OsTaskWake(task: *mut c_void, status: u32);
    fn OsSchedResched();
    fn OsSchedPreempt();
    fn IntActive() -> usize;
}

use crate::arch::{ArchIntLock, ArchIntRestore, OsCurrTaskGet, ArchCurrCpuid};

#[repr(C)]
pub struct Percpu {
    // 注意：需要添加所有字段，或者至少添加到 schedFlag 字段
    // 这里只是简化示例，实际结构可能更复杂
    pub task_lock_cnt: u32,
    pub sched_flag: u32,
    // 其他字段...
}

pub const INT_PEND_RESCH: u32 = 1; // 对应 SchedFlag 枚举中的值

// 宏定义替代
const OS_TASK_STATUS_PEND: u32 = 0x0008;
const OS_TASK_STATUS_TIMEOUT: u32 = 0x0010;
const OS_TASK_FLAG_SYSTEM: u32 = 0x0200;
const OS_MP_CPU_ALL: u32 = 0xFFFFFFFF;


/// 初始化双向链表
#[unsafe(no_mangle)]
pub extern "C" fn LOS_ListInit(list: *mut c_void) {
    let list = list as *mut LOS_DL_LIST;
    unsafe {
        (*list).pstNext = list;
        (*list).pstPrev = list;
    }
}

/// 锁定中断并返回之前的中断状态
#[unsafe(no_mangle)]
pub extern "C" fn LOS_IntLock() -> u32 {
    // 调用架构特定的中断锁定函数
    unsafe { ArchIntLock() }
}

/// 恢复中断状态
#[unsafe(no_mangle)]
pub extern "C" fn LOS_IntRestore(intSave: u32) {
    // 调用架构特定的中断恢复函数
    unsafe { ArchIntRestore(intSave) }
}

/// 检查链表是否为空
#[unsafe(no_mangle)]
pub extern "C" fn LOS_ListEmpty(list: *const c_void) -> bool {
    let list = list as *const LOS_DL_LIST;
    unsafe { (*list).pstNext as *const _ == list }
}

/// 向链表中删除节点
#[unsafe(no_mangle)]
pub extern "C" fn LOS_ListDelete(node: *mut c_void) -> u32 {
    let node = node as *mut LOS_DL_LIST;
    
    unsafe {
        (*(*node).pstPrev).pstNext = (*node).pstNext;
        (*(*node).pstNext).pstPrev = (*node).pstPrev;
        (*node).pstNext = node;
        (*node).pstPrev = node;
    }
    
    0 // 返回成功
}

/// 清除事件标志位
#[unsafe(no_mangle)]
pub extern "C" fn LOS_EventClear(eventCB: *mut EventCB, events: u32) -> u32 {
    if eventCB.is_null() {
        return LOS_ERRNO_EVENT_PTR_NULL;
    }

    let int_save = LOS_IntLock();
    unsafe { (*eventCB).event_id &= events };
    LOS_IntRestore(int_save);

    LOS_OK
}


/// 初始化事件控制块
///
/// # 参数
///
/// * `event_cb` - 事件控制块指针
///
/// # 返回值
///
/// * `LOS_OK` - 初始化成功
/// * `LOS_ERRNO_EVENT_PTR_NULL` - 事件控制块指针为空
#[unsafe(export_name = "LOS_EventInit")]
pub extern "C" fn event_init(event_cb: *mut EventCB) -> u32 {
    if event_cb.is_null() {
        return LOS_ERRNO_EVENT_PTR_NULL;
    }

    let int_save = LOS_IntLock();
    unsafe {
        (*event_cb).event_id = 0;
        LOS_ListInit(&mut (*event_cb).event_list as *mut _ as *mut c_void);
    }
    LOS_IntRestore(int_save);

    // 添加跟踪调用
    event_trace::trace_event_create(event_cb);

    LOS_OK
}

/// 检查事件参数是否有效
fn event_param_check(ptr: *const c_void, event_mask: u32, mode: u32) -> u32 {
    if ptr.is_null() {
        return LOS_ERRNO_EVENT_PTR_NULL;
    }

    if event_mask == 0 {
        return LOS_ERRNO_EVENT_EVENTMASK_INVALID;
    }

    if event_mask & LOS_ERRTYPE_ERROR != 0 {
        return LOS_ERRNO_EVENT_SETBIT_INVALID;
    }

    if ((mode & LOS_WAITMODE_OR != 0) && (mode & LOS_WAITMODE_AND != 0)) ||
       (mode & !(LOS_WAITMODE_OR | LOS_WAITMODE_AND | LOS_WAITMODE_CLR) != 0) ||
       (mode & (LOS_WAITMODE_OR | LOS_WAITMODE_AND) == 0) {
        return LOS_ERRNO_EVENT_FLAGS_INVALID;
    }

    LOS_OK
}

/// 轮询事件
fn event_poll(event_id: *mut u32, event_mask: u32, mode: u32) -> u32 {
    let mut ret = 0;
    let event_val = unsafe { *event_id };

    if mode & LOS_WAITMODE_OR != 0 {
        if event_val & event_mask != 0 {
            ret = event_val & event_mask;
        }
    } else if event_mask != 0 && (event_val & event_mask) == event_mask {
        ret = event_val & event_mask;
    }

    if ret != 0 && (mode & LOS_WAITMODE_CLR) != 0 {
        unsafe { *event_id = event_val & !ret };
    }

    ret
}

/// 事件读取检查
fn event_read_check(event_cb: *const EventCB, event_mask: u32, mode: u32) -> u32 {
    let ret = event_param_check(event_cb as *const c_void, event_mask, mode);
    if ret != LOS_OK {
        return ret;
    }

    // 在中断中不能读取事件
    if unsafe { is_interrupt_active() } {
        return LOS_ERRNO_EVENT_READ_IN_INTERRUPT;
    }

    // 系统任务警告（这里只是打印警告，不返回错误）
    
    LOS_OK
}

/// 判断是否在中断上下文
#[inline]
unsafe fn is_interrupt_active() -> bool {
    // 调用外部函数获取中断状态
    IntActive() != 0
}

/// 检查当前上下文是否可进行抢占式调度
#[inline]
unsafe fn os_preemptable_in_sched() -> bool {
    let percpu = OsPercpuGet();
    let preemptable;
    
    // 根据是否启用SMP功能进行不同的检查
    #[cfg(feature = "kernel_smp")]
    {
        // 对于SMP系统，调度必须持有任务自旋锁，此计数器在那种情况下会增加1
        preemptable = (*percpu).task_lock_cnt == 1;
    }
    
    #[cfg(not(feature = "kernel_smp"))]
    {
        preemptable = (*percpu).task_lock_cnt == 0;
    }
    
    if !preemptable {
        // 如果禁用了抢占，则设置调度标志
        (*percpu).sched_flag = INT_PEND_RESCH;
    }
    
    preemptable
}

/// 获取当前CPU的Percpu结构
#[unsafe(no_mangle)]
pub extern "C" fn OsPercpuGet() -> *mut crate::event::Percpu {
    // 添加调试输出
    unsafe extern "C" {
        fn printf(fmt: *const u8, ...) -> i32;
    }
    
    unsafe {
        let msg = b"OsPercpuGet called\n\0";
        printf(msg.as_ptr());
    }

    unsafe extern "C" {
        #[link_name = "g_percpu"]
        static mut g_percpu: [crate::event::Percpu; 1]; // 注意使用static mut
    }
    
    unsafe {
        let cpu_id = ArchCurrCpuid();
        &mut g_percpu[cpu_id as usize] as *mut _
    }
}


/// 事件读取实现
unsafe fn event_read_imp(
    event_cb: *mut EventCB,
    event_mask: u32,
    mode: u32,
    timeout: u32,
    once: bool,
    int_save: *mut u32,
) -> u32 {
    let mut ret = 0;
    let run_task = OsCurrTaskGet() as *mut crate::task::LosTaskCB;

    if !once {
        ret = event_poll(&mut (*event_cb).event_id, event_mask, mode);
    }

    if ret == 0 {
        if timeout == 0 {
            return ret;
        }

        if !os_preemptable_in_sched() {
            return LOS_ERRNO_EVENT_READ_IN_LOCK;
        }

        // 设置任务等待事件信息
        (*run_task).event_mask = event_mask;
        (*run_task).event_mode = mode;
        
        // 添加任务到等待列表并挂起
        OsTaskWait(&mut (*event_cb).event_list as *mut _ as *mut c_void, OS_TASK_STATUS_PEND, timeout);
        
        // 重新调度
        OsSchedResched();

        // 解锁调度器并重新加锁，模拟 SCHEDULER_UNLOCK/LOCK
        LOS_IntRestore(*int_save);
        *int_save = LOS_IntLock();

        // 检查是否超时
        if (*run_task).task_status & OS_TASK_STATUS_TIMEOUT != 0 {
            // 清除超时状态
            (*run_task).task_status &= !OS_TASK_STATUS_TIMEOUT;
            return LOS_ERRNO_EVENT_READ_TIMEOUT;
        }

        ret = event_poll(&mut (*event_cb).event_id, event_mask, mode);
    }
    
    ret
}

/// 通用事件读取函数
unsafe fn event_read(
    event_cb: *mut EventCB,
    event_mask: u32,
    mode: u32,
    timeout: u32,
    once: bool,
) -> u32 {
    let ret = event_read_check(event_cb, event_mask, mode);
    if ret != LOS_OK {
        return ret;
    }

    let mut int_save = LOS_IntLock();
    let ret = event_read_imp(event_cb, event_mask, mode, timeout, once, &mut int_save);
    LOS_IntRestore(int_save);
    
    ret
}


unsafe extern "C" {
    fn printf(fmt: *const u8, ...) -> i32;
}

/// 调度函数
#[unsafe(no_mangle)]
pub extern "C" fn LOS_Schedule() {
    static mut counter: u32 = 0;

    unsafe {
        counter += 1;
        if counter > 100 {
            let msg = b"Detected possible infinite loop, pausing...\n\0";
            printf(msg.as_ptr());
            loop { /* 无限循环，暂停执行 */ }
        }
    }

    // 检查是否在中断上下文中
    if unsafe { IntActive() != 0 } {
        // 在中断上下文中，设置调度标志
        unsafe {
            // 直接访问结构体字段，与C代码保持一致
            (*OsPercpuGet()).sched_flag = INT_PEND_RESCH;
        }
        return;
    }

    // 在任务上下文中，直接触发调度
    unsafe {
        OsSchedPreempt();
    }
}

#[unsafe(no_mangle)]
pub extern "C" fn LOS_MpSchedule(target: u32) {
    // 在单处理器系统中，这个函数什么都不做
    let _ = target; // 消除未使用参数的警告
}

// // 设置调度标志的外部函数
// unsafe extern "C" {
//     fn OsSchedPreempt();
//     fn set_sched_flag();
// }

/// 事件写入函数
unsafe fn event_write(event_cb: *mut EventCB, events: u32, once: bool) -> u32 {
    if event_cb.is_null() {
        return LOS_ERRNO_EVENT_PTR_NULL;
    }

    if events & LOS_ERRTYPE_ERROR != 0 {
        return LOS_ERRNO_EVENT_SETBIT_INVALID;
    }

    let mut int_save = LOS_IntLock();
    
    // 设置事件
    (*event_cb).event_id |= events;
    
    let mut exit_flag = 0;
    
    // 唤醒等待的任务
    if !LOS_ListEmpty(&(*event_cb).event_list as *const _ as *const c_void) {
        // 遍历链表, 查找符合唤醒条件的任务
        let mut list_head = &(*event_cb).event_list as *const LOS_DL_LIST as *mut LOS_DL_LIST;
        let mut list_node = (*list_head).pstNext;
        
        while list_node != list_head {
            // 从链表节点获取任务控制块指针
            let task = container_of!(list_node, crate::task::LosTaskCB, pend_list) as *mut crate::task::LosTaskCB;
            list_node = (*list_node).pstNext;
            
            // 检查任务是否满足唤醒条件
            let event_id = (*event_cb).event_id;
            let task_event_mask = (*task).event_mask;
            let task_event_mode = (*task).event_mode;
            
            let need_wake = if task_event_mode & LOS_WAITMODE_OR != 0 {
                event_id & task_event_mask != 0
            } else {
                (event_id & task_event_mask) == task_event_mask
            };
            
            if need_wake {
                // 将任务从等待链表中移除并唤醒它
                OsTaskWake(task as *mut c_void, 0);
                exit_flag = 1;
                
                // 如果是一次性写入，每次只唤醒一个任务
                if once {
                    break;
                }
            }
        }
    }
    
    LOS_IntRestore(int_save);
    
    if exit_flag == 1 {
        LOS_MpSchedule(OS_MP_CPU_ALL);
        LOS_Schedule();
    }
    
    LOS_OK
}

/// 轮询事件
///
/// # 参数
///
/// * `event_id` - 事件ID指针
/// * `event_mask` - 事件掩码
/// * `mode` - 事件模式
///
/// # 返回值
///
/// * 匹配的事件位
/// * 错误码
#[unsafe(export_name = "LOS_EventPoll")]
pub extern "C" fn event_poll_api(event_id: *mut u32, event_mask: u32, mode: u32) -> u32 {
    let ret = event_param_check(event_id as *const c_void, event_mask, mode);
    if ret != LOS_OK {
        return ret;
    }

    let int_save = LOS_IntLock();
    let ret = unsafe { event_poll(event_id, event_mask, mode) };
    LOS_IntRestore(int_save);
    
    ret
}

/// 读取事件
///
/// # 参数
///
/// * `event_cb` - 事件控制块指针
/// * `event_mask` - 事件掩码
/// * `mode` - 事件模式
/// * `timeout` - 超时时间
///
/// # 返回值
///
/// * 匹配的事件位
/// * 错误码
#[unsafe(export_name = "LOS_EventRead")]
pub extern "C" fn event_read_api(
    event_cb: *mut EventCB,
    event_mask: u32,
    mode: u32,
    timeout: u32,
) -> u32 {
    // 添加跟踪调用
    event_trace::trace_event_read(event_cb, unsafe { (*event_cb).event_id }, event_mask, mode, timeout);
    
    unsafe { event_read(event_cb, event_mask, mode, timeout, false) }
}

/// 写入事件
///
/// # 参数
///
/// * `event_cb` - 事件控制块指针
/// * `events` - 要写入的事件位
///
/// # 返回值
///
/// * `LOS_OK` - 成功
/// * 错误码
#[unsafe(export_name = "LOS_EventWrite")]
pub extern "C" fn event_write_api(event_cb: *mut EventCB, events: u32) -> u32 {
    // 添加跟踪调用
    event_trace::trace_event_write(event_cb, unsafe { (*event_cb).event_id }, events);
    
    unsafe { event_write(event_cb, events, false) }
}

/// 销毁事件控制块
///
/// # 参数
///
/// * `event_cb` - 事件控制块指针
///
/// # 返回值
///
/// * `LOS_OK` - 成功
/// * 错误码
#[unsafe(export_name = "LOS_EventDestroy")]
pub extern "C" fn event_destroy(event_cb: *mut EventCB) -> u32 {
    if event_cb.is_null() {
        return LOS_ERRNO_EVENT_PTR_NULL;
    }

    let int_save = LOS_IntLock();
    let ret;
    
    unsafe {
        if !LOS_ListEmpty(&(*event_cb).event_list as *const _ as *const c_void) {
            ret = LOS_ERRNO_EVENT_SHOULD_NOT_DESTORY;
        } else {
            (*event_cb).event_id = 0;
            ret = LOS_OK;
        }
    }
    
    LOS_IntRestore(int_save);
    
    // 添加跟踪调用
    event_trace::trace_event_delete(event_cb, ret);

    ret
}

/// 一次性写入事件
///
/// # 参数
///
/// * `event_cb` - 事件控制块指针
/// * `events` - 要写入的事件位
///
/// # 返回值
///
/// * `LOS_OK` - 成功
/// * 错误码
#[unsafe(export_name = "OsEventWriteOnce")]
pub extern "C" fn event_write_once(event_cb: *mut EventCB, events: u32) -> u32 {
    unsafe { event_write(event_cb, events, true) }
}

/// 一次性读取事件
///
/// # 参数
///
/// * `event_cb` - 事件控制块指针
/// * `event_mask` - 事件掩码
/// * `mode` - 事件模式
/// * `timeout` - 超时时间
///
/// # 返回值
///
/// * 匹配的事件位
/// * 错误码
#[cfg(feature = "compat_posix")]
#[export_name = "OsEventReadOnce"]
pub extern "C" fn event_read_once(
    event_cb: *mut EventCB,
    event_mask: u32,
    mode: u32,
    timeout: u32,
) -> u32 {
    unsafe { event_read(event_cb, event_mask, mode, timeout, true) }
}

/// 条件读取事件
///
/// # 参数
///
/// * `cond` - 条件指针
/// * `event_cb` - 事件控制块指针
/// * `event_mask` - 事件掩码
/// * `mode` - 事件模式
/// * `timeout` - 超时时间
///
/// # 返回值
///
/// * 匹配的事件位
/// * 错误码
#[cfg(feature = "compat_posix")]
#[export_name = "OsEventReadWithCond"]
pub extern "C" fn event_read_with_cond(
    cond: *const EventCond,
    event_cb: *mut EventCB,
    event_mask: u32,
    mode: u32,
    timeout: u32,
) -> u32 {
    let ret = event_read_check(event_cb, event_mask, mode);
    if ret != LOS_OK {
        return ret;
    }

    let mut int_save = LOS_IntLock();
    
    unsafe {
        if *(*cond).real_value != (*cond).value {
            (*event_cb).event_id &= (*cond).clear_event;
            let result = LOS_OK;
            LOS_IntRestore(int_save);
            return result;
        }
        
        let ret = event_read_imp(event_cb, event_mask, mode, timeout, false, &mut int_save);
        LOS_IntRestore(int_save);
        ret
    }
}