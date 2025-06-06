//! 信号量外部接口函数 - 新实现

use crate::{
    config::OK,
    semaphore1::{
        core::{
            create_binary_semaphore, create_semaphore, delete_semaphore, init_semaphore_system,
            semaphore_pend, semaphore_post,
        },
        error::SemaphoreError,
    },
};

/// 初始化信号量模块
///
/// 对应C函数: OsSemInit
#[unsafe(export_name = "OsSemInit")]
pub extern "C" fn os_sem_init1() {
    init_semaphore_system();
}

/// 创建信号量
///
/// # 参数
/// * `count` - 初始计数值
/// * `sem_handle` - 用于存储创建的信号量句柄的指针
///
/// # 返回值
/// * `LOS_OK` - 成功
/// * 其他错误码 - 失败原因
///
/// 对应C函数: LOS_SemCreate
#[unsafe(export_name = "LOS_SemCreate")]
pub extern "C" fn los_sem_create1(count: u16, sem_handle: *mut u32) -> u32 {
    if sem_handle.is_null() {
        return SemaphoreError::PtrNull.into();
    }

    match create_semaphore(count) {
        Ok(id) => {
            unsafe { *sem_handle = id.into() };
            OK
        }
        Err(e) => e.into(),
    }
}

/// 创建二进制信号量
///
/// # 参数
/// * `count` - 初始计数值(0或1)
/// * `sem_handle` - 用于存储创建的信号量句柄的指针
///
/// # 返回值
/// * `LOS_OK` - 成功
/// * 其他错误码 - 失败原因
///
/// 对应C函数: LOS_BinarySemCreate
#[unsafe(export_name = "LOS_BinarySemCreate")]
pub extern "C" fn los_binary_sem_create1(count: u16, sem_handle: *mut u32) -> u32 {
    if sem_handle.is_null() {
        return SemaphoreError::PtrNull.into();
    }

    match create_binary_semaphore(count) {
        Ok(id) => {
            unsafe { *sem_handle = id.into() };
            OK
        }
        Err(e) => e.into(),
    }
}

/// 删除信号量
///
/// # 参数
/// * `sem_handle` - 信号量句柄
///
/// # 返回值
/// * `LOS_OK` - 成功
/// * 其他错误码 - 失败原因
///
/// 对应C函数: LOS_SemDelete
#[unsafe(export_name = "LOS_SemDelete")]
pub extern "C" fn los_sem_delete1(sem_handle: u32) -> u32 {
    match delete_semaphore(sem_handle.into()) {
        Ok(_) => OK,
        Err(e) => e.into(),
    }
}

/// 等待信号量(P操作)
///
/// # 参数
/// * `sem_handle` - 信号量句柄
/// * `timeout` - 超时时间(单位:tick)
///
/// # 返回值
/// * `LOS_OK` - 成功
/// * 其他错误码 - 失败原因
///
/// 对应C函数: LOS_SemPend
#[unsafe(export_name = "LOS_SemPend")]
pub extern "C" fn los_sem_pend1(sem_handle: u32, timeout: u32) -> u32 {
    match semaphore_pend(sem_handle.into(), timeout) {
        Ok(_) => OK,
        Err(e) => e.into(),
    }
}

/// 释放信号量(V操作)
///
/// # 参数
/// * `sem_handle` - 信号量句柄
///
/// # 返回值
/// * `LOS_OK` - 成功
/// * 其他错误码 - 失败原因
///
/// 对应C函数: LOS_SemPost
#[unsafe(export_name = "LOS_SemPost")]
pub extern "C" fn los_sem_post1(sem_handle: u32) -> u32 {
    match semaphore_post(sem_handle.into()) {
        Ok(_) => OK,
        Err(e) => e.into(),
    }
}