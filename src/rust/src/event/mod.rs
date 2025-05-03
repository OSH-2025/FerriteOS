use crate::utils::{dl_list::DlList, printf::dprintf};

#[repr(C)]
pub struct EventControlBlock {
    pub event_id: u32,      // 事事件ID，每一位标识一种事件类型
    pub event_list: DlList, // 读取事件的任务链表
}

const LOS_OK: u32 = 0; // 成功
const LOS_ERRNO_EVENT_SETBIT_INVALID: u32 = 0x02001c00;
const LOS_ERRNO_EVENT_READ_TIMEOUT: u32 = 0x02001c01;
const LOS_ERRNO_EVENT_EVENTMASK_INVALID: u32 = 0x02001c02;
const LOS_ERRNO_EVENT_READ_IN_INTERRUPT: u32 = 0x02001c03;
const LOS_ERRNO_EVENT_FLAGS_INVALID: u32 = 0x02001c04;
const LOS_ERRNO_EVENT_READ_IN_LOCK: u32 = 0x02001c05;
const LOS_ERRNO_EVENT_PTR_NULL: u32 = 0x02001c06;
const LOS_WAITMODE_CLR: u32 = 0x1;
const LOS_WAITMODE_OR: u32 = 0x2;
const LOS_WAITMODE_AND: u32 = 0x4;
const LOS_ERRTYPE_ERROR: u32 = 0x2 << 24;
const OS_TASK_FLAG_SYSTEM: u32 = 0x0002;
const OS_TASK_STATUS_PEND: u16 = 0x0008;
const OS_TASK_STATUS_TIMEOUT: u16 = 0x0040;

unsafe extern "C" {
    #[link_name = "LOS_IntLock_Wrapper"]
    pub unsafe fn int_lock() -> u32;

    #[link_name = "LOS_IntRestore_Wrapper"]
    pub unsafe fn int_restore(int_save: u32);

    #[link_name = "IntActive"]
    pub unsafe fn int_active() -> u32;

    #[link_name = "Os_Preemptable_In_Sched_Wrapper"]
    pub unsafe fn os_preemptable_in_sched() -> u32;

    #[link_name = "Os_Get_Curr_Task_Flags_Wrapper"]
    pub unsafe fn os_curr_task_flags_get() -> u32;

    #[link_name = "Os_Get_Curr_Task_Status_Wrapper"]
    pub unsafe fn os_curr_task_status_get() -> u16;

    #[link_name = "Os_Set_Curr_Task_Event_Mask_Wrapper"]
    pub unsafe fn os_set_curr_task_event_mask(event_mask: u32);

    #[link_name = "Os_Set_Curr_Task_Event_Mode_Wrapper"]
    pub unsafe fn os_set_curr_task_event_mode(event_mode: u32);

    #[link_name = "Os_Set_Curr_Task_Status_Wrapper"]
    pub unsafe fn os_set_curr_task_event_status();

    #[link_name = "OsTaskWait"]
    pub unsafe fn os_task_wait(list: *mut DlList, task_status: u16, timeout: u32);

    #[link_name = "OsSchedResched"]
    pub unsafe fn os_sched_resched();

    #[link_name = "Scheduler_Lock_Wrapper"]
    pub unsafe fn scheduler_lock(int_save: *mut u32);

    #[link_name = "Scheduler_Unlock_Wrapper"]
    pub unsafe fn scheduler_unlock(int_save: *mut u32);
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
    if ptr.is_null() {
        return LOS_ERRNO_EVENT_PTR_NULL;
    }
    if event_mask == 0 {
        return LOS_ERRNO_EVENT_EVENTMASK_INVALID;
    }
    if event_mask & LOS_ERRTYPE_ERROR != 0 {
        return LOS_ERRNO_EVENT_SETBIT_INVALID;
    }
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
    // TODO LOS_ASSERT
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

#[unsafe(export_name = "OsEventReadCheck")]
pub fn os_event_read_check(event_cb: *const EventControlBlock, event_mask: u32, mode: u32) -> u32 {
    // 检查参数有效性
    let ret = os_event_param_check(event_cb as *const (), event_mask, mode);
    if ret != LOS_OK {
        return ret;
    }
    // 检查是否在中断上下文中
    if unsafe { int_active() } != 0 {
        return LOS_ERRNO_EVENT_READ_IN_INTERRUPT;
    }
    let task_flags = unsafe { os_curr_task_flags_get() };
    if task_flags & OS_TASK_FLAG_SYSTEM != 0 {
        // TODO PRINT_DEBUG
        unsafe {
            dprintf(b"Warning: DO NOT recommend to use LOS_EventRead or OsEventReadOnce in system tasks.\n\0" as *const u8 );
        };
    }
    LOS_OK
}

#[unsafe(export_name = "OsEventReadImp")]
pub unsafe fn os_event_read_imp(
    event_cb: *mut EventControlBlock,
    event_mask: u32,
    mode: u32,
    timeout: u32,
    once: u32,
    int_save: *mut u32,
) -> u32 {
    let mut ret: u32 = 0;
    let event_cb = unsafe { &mut *event_cb };
    if once == 0 {
        ret = os_event_poll(&mut event_cb.event_id, event_mask, mode);
    }
    if ret == 0 {
        if timeout == 0 {
            return ret;
        }
        if unsafe { os_preemptable_in_sched() } == 0 {
            return LOS_ERRNO_EVENT_READ_IN_LOCK;
        }
        unsafe {
            os_set_curr_task_event_mask(event_mask);
            os_set_curr_task_event_mode(mode);
            os_task_wait(&mut event_cb.event_list, OS_TASK_STATUS_PEND, timeout);
            os_sched_resched();
            scheduler_unlock(int_save);
            scheduler_lock(int_save);
        }
        let task_status = unsafe { os_curr_task_status_get() };
        if task_status & OS_TASK_STATUS_TIMEOUT /* OS_TASK_STATUS_TIMEOUT */ != 0 {
            unsafe {
                os_set_curr_task_event_status();
            }
            return LOS_ERRNO_EVENT_READ_TIMEOUT;
        }
        ret = os_event_poll(&mut event_cb.event_id, event_mask, mode);
    }
    ret
}
