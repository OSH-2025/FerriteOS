#![no_std]
#![no_main]
// 忽略命名规范警告
#![allow(non_snake_case)]
#![allow(non_camel_case_types)]

use core::panic::PanicInfo;
use core::ffi::c_void;

#[panic_handler]
fn panic(_info: &PanicInfo) -> ! {
    loop {}
}

// --- 常量定义 ---
pub const LOS_INVALID_BIT_INDEX: u16 = 32;
// 从 los_config.h 获取
pub const KERNEL_MUX_LIMIT: usize = 1024; // 这个值需要从实际配置文件确认
pub const LOS_UNUSED: u8 = 0;
pub const LOS_USED: u8 = 1;

// 错误码 - 从 los_mux.h 获取实际值
pub const LOS_OK: u32 = 0;
// LOS_ERRNO_OS_ERROR(LOS_MOD_MUX, 0x00) = 0x02001d00
pub const LOS_ERRNO_MUX_NO_MEMORY: u32 = 0x02001d00;
// LOS_ERRNO_OS_ERROR(LOS_MOD_MUX, 0x01) = 0x02001d01  
pub const LOS_ERRNO_MUX_INVALID: u32 = 0x02001d01;
// LOS_ERRNO_OS_ERROR(LOS_MOD_MUX, 0x02) = 0x02001d02
pub const LOS_ERRNO_MUX_PTR_NULL: u32 = 0x02001d02;
// LOS_ERRNO_OS_ERROR(LOS_MOD_MUX, 0x03) = 0x02001d03
pub const LOS_ERRNO_MUX_ALL_BUSY: u32 = 0x02001d03;
// LOS_ERRNO_OS_ERROR(LOS_MOD_MUX, 0x04) = 0x02001d04
pub const LOS_ERRNO_MUX_UNAVAILABLE: u32 = 0x02001d04;
// LOS_ERRNO_OS_ERROR(LOS_MOD_MUX, 0x05) = 0x02001d05
pub const LOS_ERRNO_MUX_PEND_INTERR: u32 = 0x02001d05;
// LOS_ERRNO_OS_ERROR(LOS_MOD_MUX, 0x06) = 0x02001d06
pub const LOS_ERRNO_MUX_PEND_IN_LOCK: u32 = 0x02001d06;
// LOS_ERRNO_OS_ERROR(LOS_MOD_MUX, 0x07) = 0x02001d07
pub const LOS_ERRNO_MUX_TIMEOUT: u32 = 0x02001d07;
// LOS_ERRNO_OS_ERROR(LOS_MOD_MUX, 0x09) = 0x02001d09
pub const LOS_ERRNO_MUX_PENDED: u32 = 0x02001d09;

// 任务标志和状态 - 从 los_task_pri.h 获取
pub const OS_TASK_FLAG_SYSTEM: u32 = 0x0002;
pub const OS_TASK_STATUS_PEND: u32 = 0x0008;
pub const OS_TASK_STATUS_TIMEOUT: u32 = 0x0040;

pub const LOS_WAIT_FOREVER: u32 = 0xFFFFFFFF;

// 从 los_mux_pri.h 获取
pub const MUX_SCHEDULE: u32 = 0x01;
pub const MUX_NO_SCHEDULE: u32 = 0x02;

// 互斥锁ID操作 - 从 los_mux_pri.h 获取
const MUX_SPLIT_BIT: u32 = 16;

// --- FFI类型定义 ---
#[repr(C)]
pub struct LOS_DL_LIST {
    pub pstPrev: *mut LOS_DL_LIST,
    pub pstNext: *mut LOS_DL_LIST,
}

impl LOS_DL_LIST {
    const fn new() -> Self {
        LOS_DL_LIST {
            pstPrev: core::ptr::null_mut(),
            pstNext: core::ptr::null_mut(),
        }
    }
}

#[repr(C)]
pub struct LosTaskCB {
    pub pendList: LOS_DL_LIST,
    pub taskId: u32,
    pub priority: u16,
    pub taskFlags: u32,
    pub taskStatus: u32,
    pub priBitMap: u32,
    pub taskMux: *mut c_void,
    pub taskEntry: Option<unsafe extern "C" fn() -> ()>,
}

#[repr(C)]
pub struct LosMuxCB {
    pub muxList: LOS_DL_LIST,
    pub muxStat: u8,
    pub muxCount: u16,
    pub muxId: u32,
    pub owner: *mut LosTaskCB,
}

// --- FFI声明 ---
unsafe extern "C" {
    static mut m_aucSysMem0: *mut c_void;

    // 内存分配
    pub fn LOS_MemAlloc(pool: *mut c_void, size: u32) -> *mut c_void;

    // 链表操作
    pub fn LOS_ListInit(list: *mut LOS_DL_LIST);
    pub fn LOS_ListEmpty(list: *const LOS_DL_LIST) -> bool;
    pub fn LOS_ListTailInsert(list: *mut LOS_DL_LIST, node: *mut LOS_DL_LIST);
    pub fn LOS_ListDelete(node: *mut LOS_DL_LIST);
    pub fn LOS_DL_LIST_FIRST(list: *const LOS_DL_LIST) -> *mut LOS_DL_LIST;
    pub fn LOS_DL_LIST_LAST(list: *const LOS_DL_LIST) -> *mut LOS_DL_LIST;

    // 位图操作
    pub fn LOS_BitmapSet(bitmap: *mut u32, pos: u16);
    pub fn LOS_BitmapClr(bitmap: *mut u32, pos: u16);
    pub fn LOS_HighBitGet(bitmap: u32) -> u16;
    pub fn LOS_LowBitGet(bitmap: u32) -> u16;

    // 调度器锁
    pub fn HalIntLock() -> u32;
    pub fn HalIntRestore(intSave: u32);
    pub fn LOS_SpinLockSave() -> u32;
    pub fn LOS_SpinUnlockRestore(intSave: u32);

    // 任务操作
    pub fn OsCurrTaskGet() -> *mut LosTaskCB;
    pub fn OsTaskPriModify(taskCB: *mut LosTaskCB, priority: u16);
    pub fn OsTaskWait(listNode: *mut LOS_DL_LIST, timeout: u32);
    pub fn OsSchedResched();
    pub fn OsTaskWake(taskCB: *mut LosTaskCB);
    pub fn OsPreemptableInSched() -> bool;

    // 调试钩子
    pub fn OsMuxDbgInit();
    pub fn OsMuxDbgUpdate(muxId: u32, entry: Option<unsafe extern "C" fn() -> ()>);
    pub fn OsMuxDbgTimeUpdate(muxId: u32);
    pub fn OsMuxDlockNodeInsert(taskId: u32, mux: *const LosMuxCB);
    pub fn OsMuxDlockNodeDelete(taskId: u32, mux: *const LosMuxCB);
    pub fn OsMutexCheck();

    // 其他
    pub fn OS_INT_ACTIVE() -> bool;
    pub fn OsBackTrace();
    pub fn LOS_Schedule();
    pub fn PRINT_DEBUG(format: *const u8, ...);
    pub fn PRINT_ERR(format: *const u8, ...);
}

// --- 全局变量 ---
#[unsafe(no_mangle)]
pub static mut g_allMux: [LosMuxCB; KERNEL_MUX_LIMIT] = [const { LosMuxCB {
    muxList: LOS_DL_LIST::new(),
    muxStat: LOS_UNUSED,
    muxCount: 0,
    muxId: 0,
    owner: core::ptr::null_mut(),
} }; KERNEL_MUX_LIMIT];

#[unsafe(no_mangle)]
pub static mut g_unusedMuxList: LOS_DL_LIST = LOS_DL_LIST::new();

// --- 辅助宏和函数 ---
// 从 los_task_pri.h 获取 SCHEDULER_LOCK/UNLOCK 的正确实现
macro_rules! SCHEDULER_LOCK {
    ($intSave:ident) => {
        let $intSave = LOS_SpinLockSave();
    };
}

macro_rules! SCHEDULER_UNLOCK {
    ($intSave:expr) => {
        LOS_SpinUnlockRestore($intSave);
    };
}

// 从 los_mux_pri.h 获取正确的宏定义
macro_rules! GET_MUX_INDEX {
    ($muxId:expr) => {
        (($muxId) & ((1u32 << MUX_SPLIT_BIT) - 1))
    };
}

macro_rules! GET_MUX {
    ($muxId:expr) => {
        &mut g_allMux[GET_MUX_INDEX!($muxId) as usize]
    };
}

macro_rules! GET_MUX_COUNT {
    ($muxId:expr) => {
        (($muxId) >> MUX_SPLIT_BIT)
    };
}

macro_rules! SET_MUX_ID {
    ($count:expr, $muxId:expr) => {
        (($count) << MUX_SPLIT_BIT) | ($muxId)
    };
}

// 从C代码看到使用了 OS_TCB_FROM_PENDLIST 宏
macro_rules! OS_TCB_FROM_PENDLIST {
    ($ptr:expr) => {
        ($ptr as *mut u8).sub(core::mem::offset_of!(LosTaskCB, pendList)) as *mut LosTaskCB
    };
}

// 添加缺失的错误处理宏
macro_rules! OS_GOTO_ERR_HANDLER {
    ($errNo:expr) => {
        return $errNo;
    };
}

macro_rules! OS_RETURN_ERROR {
    ($errNo:expr) => {
        return $errNo;
    };
}

// --- 互斥锁实现 ---

#[unsafe(no_mangle)]
pub unsafe extern "C" fn OsMuxInit() {
    LOS_ListInit(&mut g_unusedMuxList);

    for index in 0..KERNEL_MUX_LIMIT {
        let mux_node = &mut g_allMux[index];
        mux_node.muxId = index as u32;
        mux_node.owner = core::ptr::null_mut();
        mux_node.muxStat = LOS_UNUSED;
        LOS_ListTailInsert(&mut g_unusedMuxList, &mut mux_node.muxList);
    }

    #[cfg(feature = "LOSCFG_DEBUG_MUTEX")]
    OsMuxDbgInit();
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn LOS_MuxCreate(muxHandle: *mut u32) -> u32 {
    if muxHandle.is_null() {
        return LOS_ERRNO_MUX_PTR_NULL;
    }

    SCHEDULER_LOCK!(intSave);
    
    if LOS_ListEmpty(&g_unusedMuxList) {
        SCHEDULER_UNLOCK!(intSave);
        #[cfg(feature = "LOSCFG_DEBUG_MUTEX")]
        OsMutexCheck();
        return LOS_ERRNO_MUX_ALL_BUSY;
    }

    let unusedMux = LOS_DL_LIST_FIRST(&g_unusedMuxList);
    LOS_ListDelete(unusedMux);
    
    // 使用 LOS_DL_LIST_ENTRY 宏等价的操作
    let muxCreated = (unusedMux as *mut u8).sub(core::mem::offset_of!(LosMuxCB, muxList)) as *mut LosMuxCB;
    
    (*muxCreated).muxCount = 0;
    (*muxCreated).muxStat = LOS_USED;
    (*muxCreated).owner = core::ptr::null_mut();
    LOS_ListInit(&mut (*muxCreated).muxList);
    *muxHandle = (*muxCreated).muxId;

    #[cfg(feature = "LOSCFG_DEBUG_MUTEX")]
    OsMuxDbgUpdate((*muxCreated).muxId, (*OsCurrTaskGet()).taskEntry);

    SCHEDULER_UNLOCK!(intSave);
    return LOS_OK;
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn LOS_MuxDelete(muxHandle: u32) -> u32 {
    if GET_MUX_INDEX!(muxHandle) >= KERNEL_MUX_LIMIT as u32 {
        OS_GOTO_ERR_HANDLER!(LOS_ERRNO_MUX_INVALID);
    }

    let muxDeleted = GET_MUX!(muxHandle);

    SCHEDULER_LOCK!(intSave);
    
    if (muxDeleted.muxId != muxHandle) || (muxDeleted.muxStat == LOS_UNUSED) {
        SCHEDULER_UNLOCK!(intSave);
        OS_GOTO_ERR_HANDLER!(LOS_ERRNO_MUX_INVALID);
    }

    if !LOS_ListEmpty(&muxDeleted.muxList) || muxDeleted.muxCount != 0 {
        SCHEDULER_UNLOCK!(intSave);
        OS_GOTO_ERR_HANDLER!(LOS_ERRNO_MUX_PENDED);
    }

    LOS_ListTailInsert(&mut g_unusedMuxList, &mut muxDeleted.muxList);
    muxDeleted.muxStat = LOS_UNUSED;
    muxDeleted.muxId = SET_MUX_ID!(
        GET_MUX_COUNT!(muxDeleted.muxId) + 1,
        GET_MUX_INDEX!(muxDeleted.muxId)
    );

    #[cfg(feature = "LOSCFG_DEBUG_MUTEX")]
    OsMuxDbgUpdate(muxDeleted.muxId, None);

    SCHEDULER_UNLOCK!(intSave);
    return LOS_OK;
}

unsafe fn OsMuxParaCheck(muxCB: *const LosMuxCB, muxHandle: u32) -> u32 {
    if ((*muxCB).muxStat == LOS_UNUSED) || ((*muxCB).muxId != muxHandle) {
        OS_RETURN_ERROR!(LOS_ERRNO_MUX_INVALID);
    }

    #[cfg(feature = "LOSCFG_DEBUG_MUTEX")]
    OsMuxDbgTimeUpdate((*muxCB).muxId);

    if OS_INT_ACTIVE() {
        return LOS_ERRNO_MUX_PEND_INTERR;
    }
    return LOS_OK;
}

unsafe fn OsMuxBitmapSet(runTask: *const LosTaskCB, muxPended: *const LosMuxCB) {
    let owner = (*muxPended).owner;
    if !owner.is_null() && (*owner).priority > (*runTask).priority {
        LOS_BitmapSet(&mut (*owner).priBitMap, (*owner).priority);
        OsTaskPriModify(owner, (*runTask).priority);
    }
}

unsafe fn OsMuxBitmapRestore(runTask: *const LosTaskCB, owner: *mut LosTaskCB) {
    if owner.is_null() { return; }
    
    let bitMapPri: u16;
    if (*owner).priority >= (*runTask).priority {
        bitMapPri = LOS_LowBitGet((*owner).priBitMap);
        if bitMapPri != LOS_INVALID_BIT_INDEX {
            LOS_BitmapClr(&mut (*owner).priBitMap, bitMapPri);
            OsTaskPriModify(owner, bitMapPri);
        }
    } else {
        if LOS_HighBitGet((*owner).priBitMap) != (*runTask).priority {
            LOS_BitmapClr(&mut (*owner).priBitMap, (*runTask).priority);
        }
    }
}

#[cfg(feature = "LOSCFG_MUTEX_WAITMODE_PRIO")]
unsafe fn OsMuxPendFindPosSub(runTask: *const LosTaskCB, muxPended: *const LosMuxCB) -> *mut LOS_DL_LIST {
    let mut node: *mut LOS_DL_LIST = core::ptr::null_mut();
    
    // 使用 LOS_DL_LIST_FOR_EACH_ENTRY 宏的等价实现
    let mut current = (*muxPended).muxList.pstNext;
    while current != &(*muxPended).muxList as *const LOS_DL_LIST as *mut LOS_DL_LIST {
        let pendedTask = OS_TCB_FROM_PENDLIST!(current);
        
        if (*pendedTask).priority < (*runTask).priority {
            current = (*current).pstNext;
            continue;
        } else if (*pendedTask).priority >= (*runTask).priority {
            node = &mut (*pendedTask).pendList;
            break;
        } else {
            node = (*pendedTask).pendList.pstNext;
            break;
        }
    }
    
    return node;
}

unsafe fn OsMuxPendFindPos(runTask: *const LosTaskCB, muxPended: *mut LosMuxCB) -> *mut LOS_DL_LIST {
    let node: *mut LOS_DL_LIST;
    
    if LOS_ListEmpty(&(*muxPended).muxList) {
        node = &mut (*muxPended).muxList;
    } else {
        #[cfg(feature = "LOSCFG_MUTEX_WAITMODE_PRIO")]
        {
            let pendedTask1 = OS_TCB_FROM_PENDLIST!(LOS_DL_LIST_FIRST(&(*muxPended).muxList));
            let pendedTask2 = OS_TCB_FROM_PENDLIST!(LOS_DL_LIST_LAST(&(*muxPended).muxList));
                
            if (*pendedTask1).priority > (*runTask).priority {
                node = (*muxPended).muxList.pstNext;
            } else if (*pendedTask2).priority <= (*runTask).priority {
                node = &mut (*muxPended).muxList;
            } else {
                node = OsMuxPendFindPosSub(runTask, muxPended);
            }
        }
        #[cfg(not(feature = "LOSCFG_MUTEX_WAITMODE_PRIO"))]
        {
            node = &mut (*muxPended).muxList;
        }
    }
    return node;
}

unsafe fn OsMuxPendOp(runTask: *mut LosTaskCB, muxPended: *mut LosMuxCB, timeout: u32, intSave: *mut u32) -> u32 {
    let mut ret = LOS_OK;
    let owner = (*muxPended).owner;

    let node = OsMuxPendFindPos(runTask, muxPended);
    OsTaskWait(node, timeout);
    OsSchedResched();
    SCHEDULER_UNLOCK!(*intSave);
    SCHEDULER_LOCK!(newIntSave);
    *intSave = newIntSave;

    if ((*runTask).taskStatus & OS_TASK_STATUS_TIMEOUT) != 0 {
        (*runTask).taskStatus &= !OS_TASK_STATUS_TIMEOUT;
        ret = LOS_ERRNO_MUX_TIMEOUT;
    }

    if timeout != LOS_WAIT_FOREVER {
        OsMuxBitmapRestore(runTask, owner);
    }

    return ret;
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn LOS_MuxPend(muxHandle: u32, timeout: u32) -> u32 {
    if GET_MUX_INDEX!(muxHandle) >= KERNEL_MUX_LIMIT as u32 {
        OS_RETURN_ERROR!(LOS_ERRNO_MUX_INVALID);
    }

    let muxPended = GET_MUX!(muxHandle);
    
    let runTask = OsCurrTaskGet();
    if ((*runTask).taskFlags & OS_TASK_FLAG_SYSTEM) != 0 {
        PRINT_DEBUG(b"Warning: DO NOT recommend to use %s in system tasks.\n\0".as_ptr(), b"LOS_MuxPend\0".as_ptr());
    }

    SCHEDULER_LOCK!(intSave);

    let mut ret = OsMuxParaCheck(muxPended, muxHandle);
    if ret != LOS_OK {
        SCHEDULER_UNLOCK!(intSave);
        return ret;
    }

    if (*muxPended).muxCount == 0 {
        #[cfg(feature = "LOSCFG_DEBUG_MUTEX_DEADLOCK")]
        OsMuxDlockNodeInsert((*runTask).taskId, muxPended);
        
        (*muxPended).muxCount += 1;
        (*muxPended).owner = runTask;
        SCHEDULER_UNLOCK!(intSave);
        return LOS_OK;
    }

    if (*muxPended).owner == runTask {
        (*muxPended).muxCount += 1;
        SCHEDULER_UNLOCK!(intSave);
        return LOS_OK;
    }

    if timeout == 0 {
        ret = LOS_ERRNO_MUX_UNAVAILABLE;
        SCHEDULER_UNLOCK!(intSave);
        return ret;
    }

    if !OsPreemptableInSched() {
        ret = LOS_ERRNO_MUX_PEND_IN_LOCK;
        OsBackTrace();
        SCHEDULER_UNLOCK!(intSave);
        return ret;
    }

    OsMuxBitmapSet(runTask, muxPended);
    ret = OsMuxPendOp(runTask, muxPended, timeout, &intSave as *const u32 as *mut u32);

    SCHEDULER_UNLOCK!(intSave);
    if ret == LOS_ERRNO_MUX_PEND_IN_LOCK {
        PRINT_ERR(b"!!!LOS_ERRNO_MUX_PEND_IN_LOCK!!!\n\0".as_ptr());
    }
    return ret;
}

unsafe fn OsMuxPostOpSub(runTask: *mut LosTaskCB, muxPosted: *mut LosMuxCB) {
    if !LOS_ListEmpty(&(*muxPosted).muxList) {
        let bitMapPri = LOS_HighBitGet((*runTask).priBitMap);
        
        // 使用 LOS_DL_LIST_FOR_EACH_ENTRY 宏的等价实现
        let mut current = (*muxPosted).muxList.pstNext;
        while current != &(*muxPosted).muxList as *const LOS_DL_LIST as *mut LOS_DL_LIST {
            let pendedTask = OS_TCB_FROM_PENDLIST!(current);
            if bitMapPri != (*pendedTask).priority {
                LOS_BitmapClr(&mut (*runTask).priBitMap, (*pendedTask).priority);
            }
            current = (*current).pstNext;
        }
    }
    
    let bitMapPri = LOS_LowBitGet((*runTask).priBitMap);
    LOS_BitmapClr(&mut (*runTask).priBitMap, bitMapPri);
    OsTaskPriModify((*muxPosted).owner, bitMapPri);
}

unsafe fn OsMuxPostOp(runTask: *mut LosTaskCB, muxPosted: *mut LosMuxCB) -> u32 {
    if LOS_ListEmpty(&(*muxPosted).muxList) {
        (*muxPosted).owner = core::ptr::null_mut();
        #[cfg(feature = "LOSCFG_DEBUG_MUTEX_DEADLOCK")]
        OsMuxDlockNodeDelete((*runTask).taskId, muxPosted);
        return MUX_NO_SCHEDULE;
    }

    let resumedTask = OS_TCB_FROM_PENDLIST!(LOS_DL_LIST_FIRST(&(*muxPosted).muxList));

    #[cfg(feature = "LOSCFG_MUTEX_WAITMODE_PRIO")]
    {
        if (*resumedTask).priority > (*runTask).priority {
            if LOS_HighBitGet((*runTask).priBitMap) != (*resumedTask).priority {
                LOS_BitmapClr(&mut (*runTask).priBitMap, (*resumedTask).priority);
            }
        } else if (*runTask).priBitMap != 0 {
            OsMuxPostOpSub(runTask, muxPosted);
        }
    }
    #[cfg(not(feature = "LOSCFG_MUTEX_WAITMODE_PRIO"))]
    {
        if (*runTask).priBitMap != 0 {
            OsMuxPostOpSub(runTask, muxPosted);
        }
    }

    (*muxPosted).muxCount = 1;
    (*muxPosted).owner = resumedTask;
    
    #[cfg(feature = "LOSCFG_DEBUG_MUTEX_DEADLOCK")]
    {
        OsMuxDlockNodeDelete((*runTask).taskId, muxPosted);
        OsMuxDlockNodeInsert((*resumedTask).taskId, muxPosted);
    }

    OsTaskWake(resumedTask);

    return MUX_SCHEDULE;
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn LOS_MuxPost(muxHandle: u32) -> u32 {
    if GET_MUX_INDEX!(muxHandle) >= KERNEL_MUX_LIMIT as u32 {
        return LOS_ERRNO_MUX_INVALID;
    }

    let muxPosted = GET_MUX!(muxHandle);
    
    SCHEDULER_LOCK!(intSave);

    let mut ret = OsMuxParaCheck(muxPosted, muxHandle);
    if ret != LOS_OK {
        SCHEDULER_UNLOCK!(intSave);
        return ret;
    }

    let runTask = OsCurrTaskGet();
    if ((*muxPosted).muxCount == 0) || ((*muxPosted).owner != runTask) {
        SCHEDULER_UNLOCK!(intSave);
        return LOS_ERRNO_MUX_INVALID;
    }

    (*muxPosted).muxCount -= 1;
    if (*muxPosted).muxCount != 0 {
        SCHEDULER_UNLOCK!(intSave);
        return LOS_OK;
    }

    ret = OsMuxPostOp(runTask, muxPosted);
    SCHEDULER_UNLOCK!(intSave);
    
    if ret == MUX_SCHEDULE {
        LOS_Schedule();
    }

    return LOS_OK;
}
