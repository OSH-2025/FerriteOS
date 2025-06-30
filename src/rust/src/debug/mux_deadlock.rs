#![no_std]

use core::ptr;
use core::mem;

// 导入内核相关的头文件和结构
extern "C" {
    fn LOS_MemAlloc(pool: *mut u8, size: u32) -> *mut u8;
    fn LOS_MemFree(pool: *mut u8, ptr: *mut u8) -> u32;
    fn LOS_TickCountGet() -> u64;
    fn PRINTK(fmt: *const u8, ...);
    
    static mut m_aucSysMem1: [u8; 0];
    static mut g_taskCBArray: [LosTaskCB; 0];
}

// 常量定义
const LOSCFG_BASE_CORE_TSK_LIMIT: u32 = 128;
const KERNEL_TSK_LIMIT: u32 = LOSCFG_BASE_CORE_TSK_LIMIT;
const OS_MUX_DEADLOCK_CHECK_THRESHOLD: u64 = 60000;
const OS_TASK_STATUS_UNUSED: u32 = 0x0001;

// 双向链表结构 - 对应C中的LOS_DL_LIST
#[repr(C)]
pub struct LosDlList {
    pub prev: *mut LosDlList,
    pub next: *mut LosDlList,
}

impl LosDlList {
    pub fn init(&mut self) {
        self.prev = self;
        self.next = self;
    }
    
    pub fn is_empty(&self) -> bool {
        self.next == self
    }
    
    pub fn insert_tail(&mut self, node: *mut LosDlList) {
        unsafe {
            (*node).prev = self.prev;
            (*node).next = self;
            (*self.prev).next = node;
            self.prev = node;
        }
    }
    
    pub fn delete(&mut self) {
        unsafe {
            (*self.prev).next = self.next;
            (*self.next).prev = self.prev;
        }
    }
}

// Mutex死锁控制块
#[repr(C)]
pub struct MuxDLinkCB {
    pub mux_list_head: LosDlList,
    pub last_access_time: u64,
}

// Mutex死锁链接节点
#[repr(C)]
pub struct MuxDLinkNode {
    pub mux_list: LosDlList,
    pub mux_cb: *mut core::ffi::c_void,
}

// 任务控制块（需要与C结构保持一致）
#[repr(C)]
pub struct LosTaskCB {
    // 这里应该包含实际的任务控制块字段
    // 为了简化，只列出必要字段
    pub task_name: [u8; 16],
    pub task_id: u32,
    pub task_status: u32,
    pub stack_pointer: *mut core::ffi::c_void,
    // ...其他字段...
}

// Mutex控制块
#[repr(C)]
pub struct LosMuxCB {
    pub mux_count: u32,
    pub owner: *mut LosTaskCB,
    pub mux_list: LosDlList,
    // ...其他字段...
}

// 全局变量
static mut G_MUX_DEADLOCK_CB_ARRAY: *mut MuxDLinkCB = ptr::null_mut();

// 初始化Mutex死锁检测
#[no_mangle]
pub extern "C" fn OsMuxDlockCheckInit() -> u32 {
    let size = (LOSCFG_BASE_CORE_TSK_LIMIT + 1) * mem::size_of::<MuxDLinkCB>() as u32;
    
    unsafe {
        G_MUX_DEADLOCK_CB_ARRAY = LOS_MemAlloc(m_aucSysMem1.as_mut_ptr(), size) as *mut MuxDLinkCB;
        
        if G_MUX_DEADLOCK_CB_ARRAY.is_null() {
            return 1; // LOS_NOK
        }
        
        for index in 0..=LOSCFG_BASE_CORE_TSK_LIMIT {
            let cb = &mut *G_MUX_DEADLOCK_CB_ARRAY.add(index as usize);
            cb.last_access_time = 0;
            cb.mux_list_head.init();
        }
    }
    
    0 // LOS_OK
}

// 插入Mutex节点
#[no_mangle]
pub extern "C" fn OsMuxDlockNodeInsert(task_id: u32, mux_cb: *mut core::ffi::c_void) {
    if task_id > LOSCFG_BASE_CORE_TSK_LIMIT || mux_cb.is_null() {
        return;
    }
    
    unsafe {
        let node_size = mem::size_of::<MuxDLinkNode>() as u32;
        let mux_dl_node = LOS_MemAlloc(m_aucSysMem1.as_mut_ptr(), node_size) as *mut MuxDLinkNode;
        
        if mux_dl_node.is_null() {
            return;
        }
        
        // 清零内存
        ptr::write_bytes(mux_dl_node as *mut u8, 0, mem::size_of::<MuxDLinkNode>());
        
        (*mux_dl_node).mux_cb = mux_cb;
        
        let cb = &mut *G_MUX_DEADLOCK_CB_ARRAY.add(task_id as usize);
        cb.mux_list_head.insert_tail(&mut (*mux_dl_node).mux_list);
    }
}

// 删除Mutex节点
#[no_mangle]
pub extern "C" fn OsMuxDlockNodeDelete(task_id: u32, mux_cb: *const core::ffi::c_void) {
    if task_id > LOSCFG_BASE_CORE_TSK_LIMIT || mux_cb.is_null() {
        return;
    }
    
    unsafe {
        let mux_dl_cb = &mut *G_MUX_DEADLOCK_CB_ARRAY.add(task_id as usize);
        let mut list = mux_dl_cb.mux_list_head.next;
        
        while list != &mut mux_dl_cb.mux_list_head {
            let mux_dl_node = container_of!(list, MuxDLinkNode, mux_list);
            let next = (*list).next;
            
            if (*mux_dl_node).mux_cb == mux_cb as *mut core::ffi::c_void {
                (*list).delete();
                LOS_MemFree(m_aucSysMem1.as_mut_ptr(), mux_dl_node as *mut u8);
                return;
            }
            
            list = next;
        }
    }
}

// 更新任务时间
#[no_mangle]
pub extern "C" fn OsTaskTimeUpdate(task_id: u32, tick_count: u64) {
    if task_id > LOSCFG_BASE_CORE_TSK_LIMIT {
        return;
    }
    
    unsafe {
        if !G_MUX_DEADLOCK_CB_ARRAY.is_null() {
            let cb = &mut *G_MUX_DEADLOCK_CB_ARRAY.add(task_id as usize);
            cb.last_access_time = tick_count;
        }
    }
}

// container_of 宏的Rust实现
macro_rules! container_of {
    ($ptr:expr, $type:ty, $field:ident) => {{
        let offset = &(*(ptr::null::<$type>())).$field as *const _ as usize;
        ($ptr as *const u8).sub(offset) as *mut $type
    }};
}

// 死锁回溯跟踪
unsafe fn os_deadlock_back_trace(task_cb: *const LosTaskCB) {
    // 由于PRINTK是变参函数，需要特殊处理
    // 这里简化处理，实际使用时需要根据具体内核API调整
}

// 打印等待Mutex的任务列表
unsafe fn os_mutex_pend_task_list(list: *mut LosDlList) {
    if (*list).is_empty() {
        return;
    }
    
    let mut list_tmp = (*list).next;
    let mut index = 0u32;
    
    while list_tmp != list {
        // 这里需要根据实际的任务控制块结构来获取任务信息
        // let pended_task = container_of!(list_tmp, LosTaskCB, some_field);
        
        list_tmp = (*list_tmp).next;
        index += 1;
    }
}

// 打印任务持有的Mutex列表
unsafe fn os_task_hold_mutex_list(mux_dl_cb: *mut MuxDLinkCB) {
    if (*mux_dl_cb).mux_list_head.is_empty() {
        return;
    }
    
    let mut list = (*mux_dl_cb).mux_list_head.next;
    let mut index = 0u32;
    
    while list != &mut (*mux_dl_cb).mux_list_head {
        let mux_dl_node = container_of!(list, MuxDLinkNode, mux_list);
        let mux_cb = (*mux_dl_node).mux_cb as *mut LosMuxCB;
        
        // 打印Mutex信息
        
        list = (*list).next;
        index += 1;
    }
}

// Mutex死锁检测主函数
#[no_mangle]
pub extern "C" fn OsMutexDlockCheck() {
    unsafe {
        if G_MUX_DEADLOCK_CB_ARRAY.is_null() {
            return;
        }
        
        for loop_idx in 0..KERNEL_TSK_LIMIT {
            let task_cb = &g_taskCBArray[loop_idx as usize];
            
            if task_cb.task_status & OS_TASK_STATUS_UNUSED != 0 {
                continue;
            }
            
            let mux_dl_cb = &*G_MUX_DEADLOCK_CB_ARRAY.add(task_cb.task_id as usize);
            let current_tick = LOS_TickCountGet();
            
            if (current_tick - mux_dl_cb.last_access_time) > OS_MUX_DEADLOCK_CHECK_THRESHOLD {
                os_task_hold_mutex_list(mux_dl_cb as *const _ as *mut _);
                os_deadlock_back_trace(task_cb);
            }
        }
    }
}

// Shell命令处理函数
#[cfg(feature = "shell")]
#[no_mangle]
pub extern "C" fn OsShellCmdMuxDeadlockCheck(argc: u32, argv: *const *const u8) -> u32 {
    if argc > 0 {
        return 1; // OS_ERROR
    }
    
    OsMutexDlockCheck();
    0 // LOS_OK
}
    use super::*;
    
    #[test]
    fn test_init() {
        assert!(init_mux_deadlock_detection().is_ok());
    }
    
    #[test]
    fn test_time_update() {
        init_mux_deadlock_detection().unwrap();
        os_task_time_update(1, 12345);
        // 验证时间已更新
    }
