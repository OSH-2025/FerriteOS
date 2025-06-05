use crate::{
    config::OK,
    container_of,
    interrupt::{disable_interrupts, restore_interrupt_state},
    mem::{
        defs::m_aucSysMem0,
        memory::{los_mem_alloc, los_mem_free},
    },
    percpu::os_percpu_get,
    task::{
        global::get_tcb_from_id,
        manager::create::task_create,
        types::{TaskAttr, TaskEntryFunc, TaskInitParam},
    },
    utils::{
        list::LinkedList,
        sortlink::{
            SortLinkList, add_to_sort_link, delete_from_sort_link,
            os_sort_link_get_target_expire_time, os_sort_link_init,
        },
    },
};

use core::mem::transmute;

const KERNEL_SWTMR_LIMIT: u16 = 1024;
const OS_SWTMR_MAX_TIMERID: u16 = (u16::MAX / KERNEL_SWTMR_LIMIT) * KERNEL_SWTMR_LIMIT;
const KERNEL_TSK_SWTMR_STACK_SIZE: u32 = 24576;
const OS_SWTMR_HANDLE_QUEUE_SIZE: u16 = KERNEL_SWTMR_LIMIT;

pub const LOS_ERRNO_SWTMR_PTR_NULL: u32 = 0x02000300;
pub const LOS_ERRNO_SWTMR_INTERVAL_NOT_SUITED: u32 = 0x02000301;
pub const LOS_ERRNO_SWTMR_MODE_INVALID: u32 = 0x02000302;
pub const LOS_ERRNO_SWTMR_MAXSIZE: u32 = 0x02000304;
pub const LOS_ERRNO_SWTMR_ID_INVALID: u32 = 0x02000305;
pub const LOS_ERRNO_SWTMR_NOT_CREATED: u32 = 0x02000306;
pub const LOS_ERRNO_SWTMR_NO_MEMORY: u32 = 0x02000307;
pub const LOS_ERRNO_SWTMR_QUEUE_CREATE_FAILED: u32 = 0x0200030b;
pub const LOS_ERRNO_SWTMR_TASK_CREATE_FAILED: u32 = 0x0200030c;
// pub const LOS_ERRNO_SWTMR_SORTLINK_CREATE_FAILED: u32 = 0x02000311;
pub const LOS_ERRNO_SWTMR_NOT_STARTED: u32 = 0x0200030d;
pub const LOS_ERRNO_SWTMR_STATUS_INVALID: u32 = 0x0200030e;

pub type SwtmrProcFunc = Option<unsafe extern "C" fn(arg: usize) -> ()>;

#[repr(C)]
pub struct SwtmrHandlerItem {
    pub handler: SwtmrProcFunc,
    pub arg: usize,
}

#[repr(u8)]
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

#[repr(u8)]
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

fn os_swtmr_start(swtmr: &mut LosSwtmrCB) {
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
    add_to_sort_link(&mut os_percpu_get().swtmr_sort_link, &mut swtmr.sort_list);
    // 更新定时器状态为正在计时
    swtmr.state = SwtmrState::Ticking as u8;
}

fn os_swtmr_delete(swtmr: &mut LosSwtmrCB) {
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

#[cfg(not(feature = "software_timer_in_isr"))]
fn os_swtmr_task() {
    // 读取大小设置为指针大小
    const READ_SIZE: u32 = core::mem::size_of::<*const SwtmrHandlerItem>() as u32;

    let mut read_size = READ_SIZE;
    // 获取当前CPU的软件定时器队列
    let swtmr_handler_queue = os_percpu_get().swtmr_handler_queue;
    let mut swtmr_handler: *mut SwtmrHandlerItem = core::ptr::null_mut();

    // 无限循环处理软件定时器回调
    loop {
        use core::u32;

        // 从队列中读取定时器处理项
        use crate::queue::operation::queue_read;
        let ret = queue_read(
            swtmr_handler_queue.into(),
            &mut swtmr_handler as *mut _ as *mut core::ffi::c_void,
            &mut read_size,
            u32::MAX,
        );

        // 检查读取结果和读取大小
        if ret.is_ok() && read_size == READ_SIZE {
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

#[cfg(not(feature = "software_timer_in_isr"))]
pub extern "C" fn os_swtmr_task_create() -> u32 {
    let mut swtmr_task_id: u32 = 0;

    // 创建任务参数结构
    let mut swtmr_task = TaskInitParam {
        task_entry: unsafe { transmute::<_, TaskEntryFunc>(os_swtmr_task as usize) },
        stack_size: KERNEL_TSK_SWTMR_STACK_SIZE,
        name: b"Swt_Task\0".as_ptr(),
        priority: 0,
        task_attr: TaskAttr::DETACHED,
        args: core::ptr::null_mut(),
    };

    // 创建任务
    match task_create(&mut swtmr_task_id, &mut swtmr_task) {
        Ok(_) => {
            os_percpu_get().swtmr_task_id = swtmr_task_id;
            // 设置系统任务标志
            get_tcb_from_id(swtmr_task_id).set_system_task();
            return OK;
        }
        Err(err) => {
            // 任务创建失败，返回错误码
            return err.into();
        }
    }
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
    #[cfg(not(feature = "software_timer_in_isr"))]
    {
        // 创建定时器处理队列
        use crate::queue::management::create_queue;
        match create_queue(
            OS_SWTMR_HANDLE_QUEUE_SIZE,
            core::mem::size_of::<*mut SwtmrHandlerItem>() as u16,
        ) {
            Ok(queue) => os_percpu_get().swtmr_handler_queue = queue.into(),
            Err(_) => return LOS_ERRNO_SWTMR_QUEUE_CREATE_FAILED,
        }

        // 创建定时器任务
        let ret = os_swtmr_task_create();
        if ret != OK {
            return LOS_ERRNO_SWTMR_TASK_CREATE_FAILED;
        }
    }

    // 初始化排序链表
    os_sort_link_init(&mut os_percpu_get().swtmr_sort_link);

    OK
}

pub fn swtmr_scan() {
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

            #[cfg(feature = "software_timer_in_isr")]
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
            #[cfg(not(feature = "software_timer_in_isr"))]
            {
                // 分配处理项内存
                let swtmr_handler = los_mem_alloc(
                    m_aucSysMem0 as *mut core::ffi::c_void,
                    core::mem::size_of::<SwtmrHandlerItem>() as u32,
                ) as *mut SwtmrHandlerItem;

                if !swtmr_handler.is_null() {
                    use crate::queue::operation::queue_write;

                    // 设置处理项数据
                    (*swtmr_handler).handler = swtmr.handler;
                    (*swtmr_handler).arg = swtmr.arg;

                    // 写入队列
                    if queue_write(
                        os_percpu_get().swtmr_handler_queue.into(),
                        &swtmr_handler as *const _ as *const core::ffi::c_void,
                        core::mem::size_of::<*const SwtmrHandlerItem>() as u32,
                        0,
                    )
                    .is_err()
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

#[inline]
fn os_swtmr_stop(swtmr: &mut LosSwtmrCB) {
    let sort_link_header = &mut os_percpu_get().swtmr_sort_link;

    // 从排序链表中删除定时器
    delete_from_sort_link(sort_link_header, &mut swtmr.sort_list);

    // 更新定时器状态为已创建
    swtmr.state = SwtmrState::Created as u8;

    // 重置重复计数
    swtmr.overrun = 0;
}

#[inline]
fn os_swtmr_time_get(swtmr: &LosSwtmrCB) -> u32 {
    let sort_link_header = &mut os_percpu_get().swtmr_sort_link;
    // 获取目标过期时间
    os_sort_link_get_target_expire_time(sort_link_header, &swtmr.sort_list)
}

#[unsafe(export_name = "LOS_SwtmrCreate")]
pub extern "C" fn los_swtmr_create(
    interval: u32,
    mode: u8,
    handler: SwtmrProcFunc,
    swtmr_id: &mut u16,
    arg: usize,
) -> u32 {
    // 参数验证
    if interval == 0 {
        return LOS_ERRNO_SWTMR_INTERVAL_NOT_SUITED;
    }

    // 验证模式是否有效
    if mode != SwtmrMode::Once as u8
        && mode != SwtmrMode::Period as u8
        && mode != SwtmrMode::NoSelfDelete as u8
    {
        return LOS_ERRNO_SWTMR_MODE_INVALID;
    }

    // 验证回调函数指针
    if handler.is_none() {
        return LOS_ERRNO_SWTMR_PTR_NULL;
    }

    let int_save = disable_interrupts();

    // 检查空闲列表是否为空
    if LinkedList::is_empty(&raw mut SWTMR_FREE_LIST) {
        restore_interrupt_state(int_save);
        return LOS_ERRNO_SWTMR_MAXSIZE;
    }

    let free_node = unsafe { SWTMR_FREE_LIST.next };

    // 从空闲列表中获取一个定时器控制块
    let sort_list = container_of!(free_node, SortLinkList, sort_link_node);

    let swtmr = container_of!(sort_list, LosSwtmrCB, sort_list);

    // 从空闲列表中删除该节点
    LinkedList::remove(free_node);
    restore_interrupt_state(int_save);

    let swtmr = unsafe { &mut *swtmr };
    swtmr.handler = handler;
    swtmr.mode = mode;
    swtmr.overrun = 0;
    swtmr.interval = interval;
    swtmr.expiry = interval;
    swtmr.arg = arg;
    swtmr.state = SwtmrState::Created as u8;

    // 设置排序链表值为0
    swtmr.sort_list.idx_roll_num = 0;

    // 设置输出参数
    *swtmr_id = swtmr.timer_id;

    OK
}

#[unsafe(export_name = "LOS_SwtmrStart")]
pub extern "C" fn los_swtmr_start(swtmr_id: u16) -> u32 {
    // 检查定时器ID是否超出范围
    if swtmr_id >= OS_SWTMR_MAX_TIMERID {
        return LOS_ERRNO_SWTMR_ID_INVALID;
    }

    // 加锁保护访问
    let int_save = disable_interrupts();

    // 计算实际定时器索引
    let swtmr_cb_id = swtmr_id % KERNEL_SWTMR_LIMIT;

    // 获取对应的定时器控制块
    let swtmr = unsafe { SWTMR_CB_ARRAY.add(swtmr_cb_id as usize) };
    let swtmr = unsafe { &mut *swtmr };

    // 二次检查定时器ID是否有效
    if swtmr.timer_id != swtmr_id {
        restore_interrupt_state(int_save);
        return LOS_ERRNO_SWTMR_ID_INVALID;
    }

    // 根据定时器状态执行不同操作
    let ret = match swtmr.state {
        // 未使用状态
        x if x == SwtmrState::Unused as u8 => LOS_ERRNO_SWTMR_NOT_CREATED,

        // 正在计时状态，停止定时器
        x if x == SwtmrState::Ticking as u8 => {
            os_swtmr_stop(swtmr);
            os_swtmr_start(swtmr);
            OK
        }

        // 已创建但未启动状态
        x if x == SwtmrState::Created as u8 => {
            os_swtmr_start(swtmr);
            OK
        }

        // 其他状态视为无效
        _ => LOS_ERRNO_SWTMR_STATUS_INVALID,
    };
    // 解锁
    restore_interrupt_state(int_save);

    ret
}

#[unsafe(export_name = "LOS_SwtmrStop")]
pub extern "C" fn los_swtmr_stop(swtmr_id: u16) -> u32 {
    // 检查定时器ID是否有效
    if swtmr_id >= OS_SWTMR_MAX_TIMERID {
        return LOS_ERRNO_SWTMR_ID_INVALID;
    }

    // 加锁保护访问
    let int_save = disable_interrupts();

    // 计算实际定时器索引
    let swtmr_cb_id = swtmr_id % KERNEL_SWTMR_LIMIT;

    // 获取对应的定时器控制块
    let swtmr = unsafe { SWTMR_CB_ARRAY.add(swtmr_cb_id as usize) };
    let swtmr = unsafe { &mut *swtmr };

    // 再次验证定时器ID
    if swtmr.timer_id != swtmr_id {
        restore_interrupt_state(int_save);
        return LOS_ERRNO_SWTMR_ID_INVALID;
    }

    // 根据定时器状态执行不同操作
    let ret = match swtmr.state {
        // 未使用状态
        x if x == SwtmrState::Unused as u8 => LOS_ERRNO_SWTMR_NOT_CREATED,

        // 已创建但未启动状态
        x if x == SwtmrState::Created as u8 => LOS_ERRNO_SWTMR_NOT_STARTED,

        // 正在计时状态，停止定时器
        x if x == SwtmrState::Ticking as u8 => {
            os_swtmr_stop(&mut *swtmr);
            OK
        }

        // 其他状态视为无效
        _ => LOS_ERRNO_SWTMR_STATUS_INVALID,
    };

    // 解锁
    restore_interrupt_state(int_save);

    ret
}

#[unsafe(export_name = "LOS_SwtmrTimeGet")]
pub extern "C" fn los_swtmr_time_get(swtmr_id: u16, tick: &mut u32) -> u32 {
    // 检查定时器ID是否超出范围
    if swtmr_id >= OS_SWTMR_MAX_TIMERID {
        return LOS_ERRNO_SWTMR_ID_INVALID;
    }

    // 加锁保护访问
    let int_save = disable_interrupts();

    // 计算实际定时器索引
    let swtmr_cb_id = swtmr_id % KERNEL_SWTMR_LIMIT;

    // 获取对应的定时器控制块
    let swtmr = unsafe { SWTMR_CB_ARRAY.add(swtmr_cb_id as usize) };
    let swtmr = unsafe { &*swtmr };

    // 二次检查定时器ID是否有效
    if swtmr.timer_id != swtmr_id {
        restore_interrupt_state(int_save);
        return LOS_ERRNO_SWTMR_ID_INVALID;
    }

    // 根据定时器状态执行不同操作
    let ret = match swtmr.state {
        // 未使用状态
        x if x == SwtmrState::Unused as u8 => LOS_ERRNO_SWTMR_NOT_CREATED,

        // 已创建但未启动状态
        x if x == SwtmrState::Created as u8 => LOS_ERRNO_SWTMR_NOT_STARTED,

        // 正在计时状态，获取剩余时间
        x if x == SwtmrState::Ticking as u8 => {
            *tick = os_swtmr_time_get(swtmr);
            OK
        }

        // 其他状态视为无效
        _ => LOS_ERRNO_SWTMR_STATUS_INVALID,
    };

    // 解锁
    restore_interrupt_state(int_save);

    ret
}

#[unsafe(export_name = "LOS_SwtmrDelete")]
pub extern "C" fn los_swtmr_delete(swtmr_id: u16) -> u32 {
    // 检查定时器ID是否超出范围
    if swtmr_id >= OS_SWTMR_MAX_TIMERID {
        return LOS_ERRNO_SWTMR_ID_INVALID;
    }

    // 加锁保护访问
    let int_save = disable_interrupts();

    // 计算实际定时器索引
    let swtmr_cb_id = swtmr_id % KERNEL_SWTMR_LIMIT;

    // 获取对应的定时器控制块
    let swtmr = unsafe { SWTMR_CB_ARRAY.add(swtmr_cb_id as usize) };
    let swtmr = unsafe { &mut *swtmr };

    // 二次检查定时器ID是否有效
    if swtmr.timer_id != swtmr_id {
        restore_interrupt_state(int_save);
        return LOS_ERRNO_SWTMR_ID_INVALID;
    }

    // 根据定时器状态执行不同操作
    let ret = match swtmr.state {
        // 未使用状态
        x if x == SwtmrState::Unused as u8 => LOS_ERRNO_SWTMR_NOT_CREATED,

        // 正在计时状态，先停止再删除
        x if x == SwtmrState::Ticking as u8 => {
            os_swtmr_stop(swtmr);
            os_swtmr_delete(swtmr);
            OK
        }

        // 已创建状态，直接删除
        x if x == SwtmrState::Created as u8 => {
            os_swtmr_delete(swtmr);
            OK
        }

        // 其他状态视为无效
        _ => LOS_ERRNO_SWTMR_STATUS_INVALID,
    };

    // 解锁
    restore_interrupt_state(int_save);

    ret
}
