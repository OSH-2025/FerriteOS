use core::mem;
use core::ptr;

// 假设这些是从其他模块导入的类型和常量
type TSK_ENTRY_FUNC = Option<fn()>;
type UINT32 = u32;
type UINT64 = u64;

const LOSCFG_BASE_IPC_QUEUE_LIMIT: usize = 1024; // 假设值，需要根据实际配置调整
const LOS_UNUSED: u32 = 0;
const LOS_USED: u32 = 1;
const OS_QUEUE_READ: usize = 0;
const OS_QUEUE_WRITE: usize = 1;
const OS_ERROR: u32 = 1;
const LOS_OK: u32 = 0;

#[repr(C)]
struct QueueDebugCB {
    creator: TSK_ENTRY_FUNC,  // The task entry who created this queue
    last_access_time: UINT64, // The last access time
}

#[repr(C)]
struct LosQueueCB {
    queue_id: UINT32,
    queue_len: UINT32,
    queue_state: UINT32,
    read_writeable_cnt: [UINT32; 2],
    read_write_list: [LOS_DL_LIST; 2],
}

#[repr(C)]
struct LOS_DL_LIST {
    prev: *mut LOS_DL_LIST,
    next: *mut LOS_DL_LIST,
}

static mut G_QUEUE_DEBUG_ARRAY: [QueueDebugCB; LOSCFG_BASE_IPC_QUEUE_LIMIT] = 
    [QueueDebugCB { creator: None, last_access_time: 0 }; LOSCFG_BASE_IPC_QUEUE_LIMIT];

// 外部函数声明 - 这些需要从C代码中链接
extern "C" {
    fn LOS_TickCountGet() -> UINT64;
    fn GET_QUEUE_INDEX(queue_id: UINT32) -> usize;
    fn GET_QUEUE_HANDLE(index: usize) -> *const LosQueueCB;
    fn SCHEDULER_LOCK(int_save: &mut UINT32);
    fn SCHEDULER_UNLOCK(int_save: UINT32);
    fn LOS_ListEmpty(list: *const LOS_DL_LIST) -> bool;
    fn PRINTK(format: *const i8, ...);
}

pub fn os_queue_dbg_init() {
    unsafe {
        for item in G_QUEUE_DEBUG_ARRAY.iter_mut() {
            item.creator = None;
            item.last_access_time = 0;
        }
    }
}

pub fn os_queue_dbg_time_update(queue_id: UINT32) {
    unsafe {
        let index = GET_QUEUE_INDEX(queue_id);
        if index < LOSCFG_BASE_IPC_QUEUE_LIMIT {
            let queue_debug = &mut G_QUEUE_DEBUG_ARRAY[index];
            queue_debug.last_access_time = LOS_TickCountGet();
        }
    }
}

pub fn os_queue_dbg_update(queue_id: UINT32, entry: TSK_ENTRY_FUNC) {
    unsafe {
        let index = GET_QUEUE_INDEX(queue_id);
        if index < LOSCFG_BASE_IPC_QUEUE_LIMIT {
            let queue_debug = &mut G_QUEUE_DEBUG_ARRAY[index];
            queue_debug.creator = entry;
            queue_debug.last_access_time = LOS_TickCountGet();
        }
    }
}

fn os_queue_info_output(node: &LosQueueCB) {
    unsafe {
        let format = b"Queue ID <0x%x> may leak, queue len is 0x%x, readable cnt:0x%x, writeable cnt:0x%x, \0".as_ptr() as *const i8;
        PRINTK(format, 
               node.queue_id, 
               node.queue_len, 
               node.read_writeable_cnt[OS_QUEUE_READ],
               node.read_writeable_cnt[OS_QUEUE_WRITE]);
    }
}

fn os_queue_ops_output(node: &QueueDebugCB) {
    unsafe {
        let format = b"TaskEntry of creator:0x%p, Latest operation time: 0x%llx\n\0".as_ptr() as *const i8;
        PRINTK(format, 
               node.creator.map_or(ptr::null(), |f| f as *const fn() as *const _),
               node.last_access_time);
    }
}

pub fn os_queue_check() {
    for index in 0..LOSCFG_BASE_IPC_QUEUE_LIMIT {
        let mut int_save: UINT32 = 0;
        
        unsafe {
            SCHEDULER_LOCK(&mut int_save);
            
            let queue_handle = GET_QUEUE_HANDLE(index);
            if queue_handle.is_null() {
                SCHEDULER_UNLOCK(int_save);
                continue;
            }
            
            let queue_node = ptr::read(queue_handle);
            let queue_debug_node = G_QUEUE_DEBUG_ARRAY[index];
            
            SCHEDULER_UNLOCK(int_save);
            
            if queue_node.queue_state == LOS_UNUSED || 
               (queue_node.queue_state == LOS_USED && queue_debug_node.creator.is_none()) {
                continue;
            }
            
            if queue_node.queue_state == LOS_USED &&
               queue_node.queue_len == queue_node.read_writeable_cnt[OS_QUEUE_WRITE] &&
               LOS_ListEmpty(&queue_node.read_write_list[OS_QUEUE_READ]) &&
               LOS_ListEmpty(&queue_node.read_write_list[OS_QUEUE_WRITE]) {
                
                let format = b"Queue ID <0x%x> may leak, No task uses it, QueueLen is 0x%x, \0".as_ptr() as *const i8;
                PRINTK(format, queue_node.queue_id, queue_node.queue_len);
                os_queue_ops_output(&queue_debug_node);
            } else {
                os_queue_info_output(&queue_node);
                os_queue_ops_output(&queue_debug_node);
            }
        }
    }
}

#[cfg(feature = "shell")]
pub fn os_shell_cmd_queue_info_get(argc: UINT32, _argv: *const *const i8) -> UINT32 {
    if argc > 0 {
        unsafe {
            let format = b"\nUsage: queue\n\0".as_ptr() as *const i8;
            PRINTK(format);
        }
        return OS_ERROR;
    }
    
    unsafe {
        let format = b"used queues information: \n\0".as_ptr() as *const i8;
        PRINTK(format);
    }
    
    os_queue_check();
    LOS_OK
}

// Shell命令注册需要根据实际的shell系统来实现
#[cfg(feature = "shell")]
extern "C" {
    fn SHELLCMD_ENTRY(
        name: *const i8,
        cmd_type: u32,
        cmd_key: *const i8,
        param_num: u32,
        callback: extern "C" fn(UINT32, *const *const i8) -> UINT32,
    );
}

#[cfg(feature = "shell")]
#[no_mangle]
pub extern "C" fn queue_info_shell_wrapper(argc: UINT32, argv: *const *const i8) -> UINT32 {
    os_shell_cmd_queue_info_get(argc, argv)
}

#[cfg(feature = "shell")]
pub fn register_queue_shell_command() {
    unsafe {
        const CMD_TYPE_EX: u32 = 1; // 假设值
        let cmd_name = b"queue\0".as_ptr() as *const i8;
        let cmd_key = b"queue\0".as_ptr() as *const i8;
        SHELLCMD_ENTRY(cmd_name, CMD_TYPE_EX, cmd_key, 0, queue_info_shell_wrapper);
    }
}
