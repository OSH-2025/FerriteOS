use crate::{
    config::OK,
    task::{
        idle::idle_task_create,
        lifecycle::create::{
            task_create, task_create_only, task_create_only_static, task_create_static,
        },
        types::{TaskError, TaskInitParam},
    },
};
use core::ffi::{c_char, c_void};

#[repr(C)]
#[derive(Debug, Clone, Copy)]
pub struct CTaskInitParam {
    pub task_entry: Option<extern "C" fn(*mut c_void) -> *mut c_void>,
    pub priority: u16,
    pub args: *mut c_void,
    pub stack_size: u32,
    pub name: *const c_char,
    pub reserved: u32,
}

impl From<CTaskInitParam> for TaskInitParam {
    fn from(c_task_init_param: CTaskInitParam) -> Self {
        Self {
            task_entry: c_task_init_param.task_entry,
            priority: c_task_init_param.priority,
            args: c_task_init_param.args,
            stack_size: c_task_init_param.stack_size,
            name: c_task_init_param.name,
            task_attr: c_task_init_param.reserved.into(),
        }
    }
}

impl From<TaskInitParam> for CTaskInitParam {
    fn from(rust_param: TaskInitParam) -> Self {
        Self {
            task_entry: rust_param.task_entry,
            priority: rust_param.priority,
            args: rust_param.args,
            stack_size: rust_param.stack_size,
            name: rust_param.name,
            reserved: rust_param.task_attr.bits(),
        }
    }
}

/// C兼容的任务创建函数
#[unsafe(export_name = "LOS_TaskCreate")]
pub extern "C" fn los_task_create(task_id: *mut u32, c_init_param: *mut CTaskInitParam) -> u32 {
    if task_id.is_null() {
        return TaskError::InvalidId.into();
    }
    if c_init_param.is_null() {
        return TaskError::ParamNull.into();
    }
    unsafe {
        let task_id_ref = &mut *task_id;
        let c_param_ref = &*c_init_param;

        let mut init_param: TaskInitParam = (*c_param_ref).into();
        match task_create(task_id_ref, &mut init_param) {
            Ok(()) => OK,
            Err(err) => err.into(),
        }
    }
}

/// C兼容的任务创建（仅创建不启动）函数
#[unsafe(export_name = "LOS_TaskCreateOnly")]
pub extern "C" fn los_task_create_only(
    task_id: *mut u32,
    c_init_param: *mut CTaskInitParam,
) -> u32 {
    if task_id.is_null() {
        return TaskError::InvalidId.into();
    }
    if c_init_param.is_null() {
        return TaskError::ParamNull.into();
    }
    unsafe {
        let task_id_ref = &mut *task_id;
        let c_param_ref = &*c_init_param;

        let mut init_param: TaskInitParam = (*c_param_ref).into();
        match task_create_only(task_id_ref, &mut init_param) {
            Ok(()) => OK,
            Err(err) => err.into(),
        }
    }
}

/// C兼容的静态任务创建函数
#[cfg(feature = "task_static_allocation")]
#[unsafe(export_name = "LOS_TaskCreateStatic")]
pub extern "C" fn los_task_create_static(
    task_id: *mut u32,
    c_init_param: *mut CTaskInitParam,
    top_stack: *mut c_void,
) -> u32 {
    if task_id.is_null() {
        return TaskError::InvalidId.into();
    }
    if c_init_param.is_null() {
        return TaskError::ParamNull.into();
    }
    unsafe {
        let task_id_ref = &mut *task_id;
        let c_param_ref = &*c_init_param;

        let mut init_param: TaskInitParam = (*c_param_ref).into();
        match task_create_static(task_id_ref, &mut init_param, top_stack) {
            Ok(()) => OK,
            Err(err) => err.into(),
        }
    }
}

/// C兼容的静态任务创建（仅创建不启动）函数
#[cfg(feature = "task_static_allocation")]
#[unsafe(export_name = "LOS_TaskCreateOnlyStatic")]
pub extern "C" fn los_task_create_only_static(
    task_id: *mut u32,
    c_init_param: *mut CTaskInitParam,
    top_stack: *mut c_void,
) -> u32 {
    if task_id.is_null() {
        return TaskError::InvalidId.into();
    }
    if c_init_param.is_null() {
        return TaskError::ParamNull.into();
    }
    unsafe {
        let task_id_ref = &mut *task_id;
        let c_param_ref = &*c_init_param;

        let mut init_param: TaskInitParam = (*c_param_ref).into();
        match task_create_only_static(task_id_ref, &mut init_param, top_stack) {
            Ok(()) => OK,
            Err(err) => err.into(),
        }
    }
}

#[unsafe(export_name = "OsIdleTaskCreate")]
pub extern "C" fn os_idle_task_create() -> u32 {
    match idle_task_create() {
        Ok(()) => OK,
        Err(err) => err.into(),
    }
}
