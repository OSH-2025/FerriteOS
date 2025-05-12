use core::ffi::c_char;

use crate::{
    LOS_OK, container_of,
    mem::{
        defs::m_aucSysMem0,
        memory::{los_mem_alloc, los_mem_free},
    },
    offset_of,
    percpu::os_percpu_get,
    task::{TaskEntryFunc, TaskInitParam, los_task_create},
    utils::{
        list::LinkedList,
        sortlink::{SortLinkList, os_add_to_sort_link, os_delete_sort_link, os_sort_link_init},
    },
};

const KERNEL_SWTMR_LIMIT: u16 = 1024;
const OS_SWTMR_MAX_TIMERID: u16 = (u16::MAX / KERNEL_SWTMR_LIMIT) * KERNEL_SWTMR_LIMIT;
const LOS_WAIT_FOREVER: u32 = u32::MAX;
const KERNEL_TSK_SWTMR_STACK_SIZE: u32 = 24576;
const LOS_TASK_STATUS_DETACHED: u32 = 0x0100;
const OS_SWTMR_HANDLE_QUEUE_SIZE: u16 = KERNEL_SWTMR_LIMIT;

pub const LOS_ERRNO_SWTMR_NO_MEMORY: u32 = 0x02000307;
pub const LOS_ERRNO_SWTMR_QUEUE_CREATE_FAILED: u32 = 0x0200030b;
pub const LOS_ERRNO_SWTMR_TASK_CREATE_FAILED: u32 = 0x0200030c;
pub const LOS_ERRNO_SWTMR_SORTLINK_CREATE_FAILED: u32 = 0x02000311;

pub type SwtmrProcFunc = Option<unsafe extern "C" fn(arg: usize) -> ()>;

#[repr(C)]
pub struct SwtmrHandlerItem {
    pub handler: SwtmrProcFunc,
    pub arg: usize,
}

pub enum SwtmrState {
    Unused = 0,
    Created = 1,
    Ticking = 2,
}

impl SwtmrState {
    #[allow(dead_code)]
    fn from_u8(value: u8) -> Option<Self> {
        match value {
            0 => Some(SwtmrState::Unused),
            1 => Some(SwtmrState::Created),
            2 => Some(SwtmrState::Ticking),
            _ => None,
        }
    }
}

pub enum SwtmrMode {
    Once = 0,
    Period = 1,
    NoSelfDelete = 2,
}

impl SwtmrMode {
    fn from_u8(value: u8) -> Option<Self> {
        match value {
            0 => Some(SwtmrMode::Once),
            1 => Some(SwtmrMode::Period),
            2 => Some(SwtmrMode::NoSelfDelete),
            _ => None,
        }
    }
}

/// 软件定时器控制块
#[repr(C)]
pub struct LosSwtmrCB {
    /// 排序链表节点
    pub sort_list: SortLinkList,
    /// 软件定时器状态
    pub state: u8,
    /// 软件定时器模式
    pub mode: u8,
    /// 软件定时器重复计时次数
    pub overrun: u8,
    /// 软件定时器ID
    pub timer_id: u16,
    /// 周期性软件定时器的超时间隔(单位:tick)
    pub interval: u32,
    /// 一次性软件定时器的超时间隔(单位:tick)
    pub expiry: u32,
    /// 回调函数调用时传入的参数
    pub arg: usize,
    /// 软件定时器超时处理回调函数
    pub handler: SwtmrProcFunc,
}

/// Free list of Software Timers
#[unsafe(export_name = "g_swtmrFreeList")]
pub static mut SWTMR_FREE_LIST: LinkedList = LinkedList {
    prev: core::ptr::null_mut(),
    next: core::ptr::null_mut(),
};

#[unsafe(export_name = "g_swtmrCBArray")]
pub static mut SWTMR_CB_ARRAY: *mut LosSwtmrCB = core::ptr::null_mut();

unsafe extern "C" {
    #[link_name = "LOS_QueueReadCopy"]
    unsafe fn los_queue_read_copy(
        queue_id: u32,
        buffer_addr: *mut core::ffi::c_void,
        buffer_size: *mut u32,
        timeout: u32,
    ) -> u32;

    #[link_name = "LOS_QueueCreate"]
    unsafe fn los_queue_create(
        queue_name: *const c_char,
        len: u16,
        queue_id: *mut u32,
        flags: u32,
        max_msg_size: u16,
    ) -> u32;

    #[link_name = "LOS_QueueWriteCopy"]
    unsafe fn los_queue_write_copy(
        queue_id: u32,
        buffer_addr: *const core::ffi::c_void,
        buffer_size: u32,
        timeout: u32,
    ) -> u32;

    #[link_name = "OS_TCB_FROM_TID_WRAPPER"]
    unsafe fn os_tcb_from_tid(task_id: u32);
}

// TODO 删除export_name
#[unsafe(export_name = "OsSwtmrStart")]
pub extern "C" fn os_swtmr_start(swtmr: &mut LosSwtmrCB) {
    // 根据定时器类型和重复次数选择合适的过期时间
    let timeout = if (swtmr.overrun == 0)
        && ((swtmr.mode == SwtmrMode::Once as u8) || (swtmr.mode == SwtmrMode::NoSelfDelete as u8))
    {
        swtmr.expiry
    } else {
        swtmr.interval
    };
    // 设置排序链表的值为选定的过期时间
    swtmr.sort_list.idx_roll_num = timeout;

    // 获取当前CPU的软件定时器排序链表，并添加定时器
    os_add_to_sort_link(&os_percpu_get().swtmr_sort_link, &mut swtmr.sort_list);
    // 更新定时器状态为正在计时
    swtmr.state = SwtmrState::Ticking as u8;
}

// TODO 删除export_name
#[unsafe(export_name = "OsSwtmrDelete")]
pub extern "C" fn os_swtmr_delete(swtmr: &mut LosSwtmrCB) {
    // 将定时器的排序链表节点插入到空闲链表尾部
    LinkedList::tail_insert(
        &raw mut SWTMR_FREE_LIST,
        &mut swtmr.sort_list.sort_link_node,
    );
    // 更新定时器状态为未使用
    swtmr.state = SwtmrState::Unused as u8;
}

fn os_swtmr_update(swtmr: &mut LosSwtmrCB) {
    match SwtmrMode::from_u8(swtmr.mode) {
        Some(SwtmrMode::Once) => {
            os_swtmr_delete(swtmr);
            if swtmr.timer_id < (OS_SWTMR_MAX_TIMERID - KERNEL_SWTMR_LIMIT) {
                swtmr.timer_id += KERNEL_SWTMR_LIMIT;
            } else {
                swtmr.timer_id %= KERNEL_SWTMR_LIMIT;
            }
        }
        Some(SwtmrMode::Period) => {
            swtmr.overrun += 1;
            os_swtmr_start(swtmr);
        }
        Some(SwtmrMode::NoSelfDelete) => {
            swtmr.state = SwtmrState::Created as u8;
        }
        None => {}
    }
}

#[cfg(not(feature = "swtmr_in_isr"))]
fn os_swtmr_task() {
    // 读取大小设置为指针大小
    const READ_SIZE: u32 = core::mem::size_of::<*const SwtmrHandlerItem>() as u32;

    let mut read_size = READ_SIZE;
    // 获取当前CPU的软件定时器队列
    let swtmr_handler_queue = os_percpu_get().swtmr_handler_queue;
    let mut swtmr_handler: *mut SwtmrHandlerItem = core::ptr::null_mut();

    // 无限循环处理软件定时器回调
    loop {
        // 从队列中读取定时器处理项
        let ret = unsafe {
            los_queue_read_copy(
                swtmr_handler_queue,
                &mut swtmr_handler as *mut _ as *mut core::ffi::c_void,
                &mut read_size,
                LOS_WAIT_FOREVER,
            )
        };

        // 检查读取结果和读取大小
        if ret == LOS_OK && read_size == READ_SIZE {
            unsafe {
                // 从处理项中提取处理函数和参数
                let handler = (*swtmr_handler).handler;
                let arg = (*swtmr_handler).arg;

                // 释放处理项内存
                los_mem_free(
                    m_aucSysMem0 as *mut core::ffi::c_void,
                    swtmr_handler as *mut core::ffi::c_void,
                );

                // 如果处理函数有效，则执行处理函数
                if let Some(handler_fn) = handler {
                    handler_fn(arg);
                }
            }
        }
    }
}

#[cfg(not(feature = "swtmr_in_isr"))]
pub extern "C" fn os_swtmr_task_create() -> u32 {
    let mut swtmr_task_id: u32 = 0;

    // 创建任务参数结构
    let mut swtmr_task = TaskInitParam {
        pfn_task_entry: unsafe { core::mem::transmute::<_, TaskEntryFunc>(os_swtmr_task as usize) },
        stack_size: KERNEL_TSK_SWTMR_STACK_SIZE,
        name: b"Swt_Task\0".as_ptr(),
        task_prio: 0,
        resved: LOS_TASK_STATUS_DETACHED,
        p_args: core::ptr::null_mut(),
    };

    // 创建任务
    let ret = los_task_create(&mut swtmr_task_id, &mut swtmr_task);
    // 如果创建成功，设置任务属性
    if ret == LOS_OK {
        os_percpu_get().swtmr_task_id = swtmr_task_id;
        unsafe {
            // 设置系统任务标志
            os_tcb_from_tid(swtmr_task_id);
        }
    }
    ret
}

#[unsafe(export_name = "OsSwtmrInit")]
pub extern "C" fn os_swtmr_init() -> u32 {
    // 获取当前CPU ID
    // 计算内存大小并分配内存
    let size = core::mem::size_of::<LosSwtmrCB>() * KERNEL_SWTMR_LIMIT as usize;

    let swtmr_ptr = los_mem_alloc(
        unsafe { m_aucSysMem0 } as *mut core::ffi::c_void,
        size as u32,
    ) as *mut LosSwtmrCB;

    // 检查内存分配结果
    if swtmr_ptr.is_null() {
        return LOS_ERRNO_SWTMR_NO_MEMORY;
    }

    // 设置全局控制块数组指针
    unsafe { SWTMR_CB_ARRAY = swtmr_ptr };

    // 初始化空闲链表
    LinkedList::init(&raw mut SWTMR_FREE_LIST);

    // 初始化每个定时器控制块并添加到空闲链表
    for index in 0..KERNEL_SWTMR_LIMIT {
        let swtmr = unsafe { &mut *swtmr_ptr.add(index as usize) };
        swtmr.timer_id = index;
        LinkedList::tail_insert(
            &raw mut SWTMR_FREE_LIST,
            &mut swtmr.sort_list.sort_link_node,
        );
    }

    // 非ISR模式下的初始化
    #[cfg(not(feature = "swtmr_in_isr"))]
    {
        // 创建定时器处理队列
        let ret = unsafe {
            los_queue_create(
                core::ptr::null_mut(),
                OS_SWTMR_HANDLE_QUEUE_SIZE,
                &mut os_percpu_get().swtmr_handler_queue,
                0,
                core::mem::size_of::<*mut SwtmrHandlerItem>() as u16,
            )
        };

        if ret != LOS_OK {
            return LOS_ERRNO_SWTMR_QUEUE_CREATE_FAILED;
        }

        // 创建定时器任务
        let ret = os_swtmr_task_create();
        if ret != LOS_OK {
            return LOS_ERRNO_SWTMR_TASK_CREATE_FAILED;
        }
    }

    // 初始化排序链表
    let ret = os_sort_link_init(&mut os_percpu_get().swtmr_sort_link);

    if ret != LOS_OK {
        return LOS_ERRNO_SWTMR_SORTLINK_CREATE_FAILED;
    }

    LOS_OK
}

#[unsafe(export_name = "OsSwtmrScan")]
pub extern "C" fn os_swtmr_scan() {
    // 获取当前CPU的软件定时器排序链表
    let swtmr_sort_link = &mut os_percpu_get().swtmr_sort_link;

    // 更新游标并获取当前链表对象
    swtmr_sort_link.advance_cursor();
    let list_object = swtmr_sort_link.list_at_cursor();

    // 如果链表为空，返回
    if LinkedList::is_empty(list_object) {
        return;
    }

    unsafe {
        // 获取第一个节点并减少轮数
        let mut sort_list = container_of!((*list_object).next, SortLinkList, sort_link_node);
        (*sort_list).roll_num_dec();

        // 处理所有轮数为0的节点
        while (*sort_list).get_roll_num() == 0 {
            // 获取链表的第一个节点
            sort_list = container_of!((*list_object).next, SortLinkList, sort_link_node);

            // 从链表中删除节点
            LinkedList::remove(&mut (*sort_list).sort_link_node);

            // 获取对应的定时器控制块
            let swtmr = container_of!(sort_list, LosSwtmrCB, sort_list);
            let swtmr = &mut *swtmr;

            #[cfg(feature = "swtmr_in_isr")]
            {
                // 保存处理函数和参数
                let handler = swtmr.handler;
                let arg = swtmr.arg;

                // 更新定时器
                os_swtmr_update(swtmr);

                // 如果处理函数非空
                if let Some(handler_fn) = handler {
                    // 执行回调
                    handler_fn(arg);
                }
            }

            // 根据编译选项选择不同的处理方式
            #[cfg(not(feature = "swtmr_in_isr"))]
            {
                // 分配处理项内存
                let swtmr_handler = los_mem_alloc(
                    m_aucSysMem0 as *mut core::ffi::c_void,
                    core::mem::size_of::<SwtmrHandlerItem>() as u32,
                ) as *mut SwtmrHandlerItem;

                if !swtmr_handler.is_null() {
                    // 设置处理项数据
                    (*swtmr_handler).handler = swtmr.handler;
                    (*swtmr_handler).arg = swtmr.arg;

                    // 写入队列
                    if los_queue_write_copy(
                        os_percpu_get().swtmr_handler_queue,
                        &swtmr_handler as *const _ as *const core::ffi::c_void,
                        core::mem::size_of::<*const SwtmrHandlerItem>() as u32,
                        0,
                    ) != LOS_OK
                    {
                        // 写入失败，释放内存
                        los_mem_free(
                            m_aucSysMem0 as *mut core::ffi::c_void,
                            swtmr_handler as *mut core::ffi::c_void,
                        );
                    }
                }

                // 更新定时器
                os_swtmr_update(swtmr);
            }

            // 检查链表是否为空
            if LinkedList::is_empty(list_object) {
                break;
            }

            // 获取下一个节点
            sort_list = container_of!((*list_object).next, SortLinkList, sort_link_node);
        }
    }
}

fn os_swtmr_stop(swtmr: &mut LosSwtmrCB) {
    let sort_link_header = &os_percpu_get().swtmr_sort_link;

    // 从排序链表中删除定时器
    os_delete_sort_link(sort_link_header, &mut swtmr.sort_list);

    // 更新定时器状态为已创建
    swtmr.state = SwtmrState::Created as u8;
    // 重置重复计数
    swtmr.overrun = 0;
}
