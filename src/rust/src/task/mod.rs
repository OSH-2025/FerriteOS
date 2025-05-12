/// 任务入口函数类型
pub type TaskEntryFunc = Option<fn(param: *mut core::ffi::c_void) -> *mut core::ffi::c_void>;

/// 任务初始化参数结构体
#[repr(C)]
pub struct TaskInitParam {
    /// 任务入口函数
    pub pfn_task_entry: TaskEntryFunc,

    /// 任务优先级
    pub task_prio: u16,

    /// 任务参数
    pub p_args: *mut core::ffi::c_void,

    /// 任务栈大小
    pub stack_size: u32,

    /// 任务名称
    pub name: *const u8,

    /// 保留字段，用于指定任务是否自动删除
    pub resved: u32,
}

impl Default for TaskInitParam {
    fn default() -> Self {
        Self {
            pfn_task_entry: None,
            task_prio: 0,
            p_args: core::ptr::null_mut(),
            stack_size: 0,
            name: core::ptr::null(),
            resved: 0,
        }
    }
}

unsafe extern "C" {
    #[link_name = "LOS_TaskCreate"]
    fn los_task_create_wrapper(task_id: &mut u32, init_param: &mut TaskInitParam) -> u32;
}

pub fn los_task_create(task_id: &mut u32, init_param: &mut TaskInitParam) -> u32 {
    unsafe { los_task_create_wrapper(task_id, init_param) }
}
