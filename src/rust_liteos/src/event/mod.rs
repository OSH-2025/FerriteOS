use crate::utils::dl_list::DlList;
// use crate::utils::printf::dprintf;

#[repr(C)]
pub struct EventControlBlock {
    pub event_id: u32,      // 事事件ID，每一位标识一种事件类型
    pub event_list: DlList, // 读取事件的任务链表
}

const LOS_OK: u32 = 0; // 成功
const LOS_ERRNO_EVENT_SETBIT_INVALID: u32 = 0x02001c00;
const LOS_ERRNO_EVENT_EVENTMASK_INVALID: u32 = 0x02001c02;
// const LOS_ERRNO_EVENT_READ_IN_INTERRUPT: u32 = 0x02001c03;
const LOS_ERRNO_EVENT_FLAGS_INVALID: u32 = 0x02001c04;
const LOS_ERRNO_EVENT_PTR_NULL: u32 = 0x02001c06;
const LOS_WAITMODE_CLR: u32 = 0x1;
const LOS_WAITMODE_OR: u32 = 0x2;
const LOS_WAITMODE_AND: u32 = 0x4;
const LOS_ERRTYPE_ERROR: u32 = 0x2 << 24;
// const OS_TASK_FLAG_SYSTEM: u32 = 0x0002;

unsafe extern "C" {
    #[link_name = "LOS_IntLock_Wrapper"]
    pub unsafe fn int_lock() -> u32;
    #[link_name = "LOS_IntRestore_Wrapper"]
    pub unsafe fn int_restore(int_save: u32);
}

#[unsafe(export_name = "LOS_EventInit")]
pub extern "C" fn event_init(event_control_block_ptr: *mut EventControlBlock) -> u32 {
    // TODO LOS_TRACE(EVENT_CREATE, (UINTPTR)eventCB);
    if event_control_block_ptr.is_null() {
        return LOS_ERRNO_EVENT_PTR_NULL;
    }
    // 获取中断锁
    let int_save = unsafe { int_lock() };
    let event_control_block = unsafe { &mut *event_control_block_ptr };
    event_control_block.event_id = 0;
    // 初始化链表
    event_control_block.event_list.init();
    // 释放中断锁
    unsafe { int_restore(int_save) };
    LOS_OK
}

#[unsafe(export_name = "OsEventParamCheck")]
pub fn os_event_param_check(ptr: *const (), event_mask: u32, mode: u32) -> u32 {
    // 检查指针是否为空
    if ptr.is_null() {
        return LOS_ERRNO_EVENT_PTR_NULL;
    }
    // 检查事件掩码是否为 0
    if event_mask == 0 {
        return LOS_ERRNO_EVENT_EVENTMASK_INVALID;
    }
    // 检查事件掩码是否包含无效位
    if event_mask & LOS_ERRTYPE_ERROR != 0 {
        return LOS_ERRNO_EVENT_SETBIT_INVALID;
    }
    // 检查 mode 参数的有效性
    if ((mode & LOS_WAITMODE_OR != 0) && (mode & LOS_WAITMODE_AND != 0))
        || (mode & !(LOS_WAITMODE_OR | LOS_WAITMODE_AND | LOS_WAITMODE_CLR) != 0)
        || (mode & (LOS_WAITMODE_OR | LOS_WAITMODE_AND) == 0)
    {
        return LOS_ERRNO_EVENT_FLAGS_INVALID;
    }
    LOS_OK
}

#[unsafe(export_name = "OsEventPoll")]
pub fn os_event_poll(event_id: &mut u32, event_mask: u32, mode: u32) -> u32 {
    let mut ret = 0;
    // TODO debug_assert!
    // LOS_ASSERT(ArchIntLocked());
    // LOS_ASSERT(LOS_SpinHeld(&g_taskSpin));
    if (mode & LOS_WAITMODE_OR) != 0 {
        if (*event_id & event_mask) != 0 {
            ret = *event_id & event_mask;
        }
    } else if (mode & LOS_WAITMODE_AND) != 0 {
        if (event_mask != 0) && (event_mask == (*event_id & event_mask)) {
            ret = *event_id & event_mask;
        }
    }
    if ret != 0 && (mode & LOS_WAITMODE_CLR) != 0 {
        *event_id &= !ret;
    }
    ret
}

// TODO: 需要实现
// #[unsafe(export_name = "OsEventReadCheck")]
// pub fn os_event_read_check(event_cb: *const EventControlBlock, event_mask: u32, mode: u32) -> u32 {
//     // 检查参数有效性
//     let ret = os_event_param_check(event_cb as *const (), event_mask, mode);
//     if ret != LOS_OK {
//         return ret;
//     }
//     // 检查是否在中断上下文中
//     if os_int_active() {
//         return LOS_ERRNO_EVENT_READ_IN_INTERRUPT;
//     }
//     // 获取当前任务
//     let run_task = os_curr_task_get();
//     if run_task.task_flags & OS_TASK_FLAG_SYSTEM != 0 {
//         unsafe {
//             dprintf(b"Warning: DO NOT recommend to use LOS_EventRead or OsEventReadOnce in system tasks.\n\0" as *const u8 );
//         };
//     }
//     LOS_OK
// }
