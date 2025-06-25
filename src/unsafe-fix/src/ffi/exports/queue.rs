use core::ffi::c_void;

use crate::{
    config::OK,
    queue::{
        error::QueueError,
        info::get_queue_info,
        management::{create_queue, delete_queue, init_queue_system},
        operation::{queue_read, queue_write, queue_write_head},
        types::{QueueId, QueueInfo},
    },
};

#[unsafe(export_name = "OsQueueInit")]
pub fn os_queue_init() {
    // 初始化队列系统
    init_queue_system();
}

#[unsafe(export_name = "LOS_QueueCreate")]
pub extern "C" fn los_queue_create(len: u16, queue_id: *mut u32, max_msg_size: u16) -> u32 {
    // 检查指针是否为空
    if queue_id.is_null() {
        return QueueError::CreatePtrNull.into();
    }

    // 调用内部实现创建队列
    match create_queue(len as usize, max_msg_size as usize) {
        Ok(id) => {
            // 创建成功，将ID写入输出参数
            unsafe { *queue_id = id.into() };
            OK
        }
        Err(e) => e.into(), // 错误转换为对应的错误码
    }
}

/// 删除队列的FFI导出函数
#[unsafe(export_name = "LOS_QueueDelete")]
pub extern "C" fn los_queue_delete(queue_id: u32) -> u32 {
    match delete_queue(QueueId(queue_id)) {
        Ok(_) => OK,
        Err(e) => e.into(),
    }
}

/// 获取队列信息的FFI导出函数
#[unsafe(export_name = "LOS_QueueInfoGet")]
pub extern "C" fn los_queue_info_get(queue_id: u32, queue_info: *mut QueueInfo) -> u32 {
    // 检查队列信息指针是否为空
    if queue_info.is_null() {
        return QueueError::PtrNull.into();
    }
    let queue_info = unsafe { &mut *queue_info };

    // 调用内部实现获取队列信息
    match get_queue_info(queue_id.into(), queue_info) {
        Ok(_) => OK,
        Err(e) => e.into(),
    }
}

/// 队列读取FFI函数
#[unsafe(export_name = "LOS_QueueRead")]
pub extern "C" fn los_queue_read(
    queue_id: u32,
    buffer_addr: *mut c_void,
    buffer_size: *mut u32,
    timeout: u32,
) -> u32 {
    if buffer_addr.is_null() || buffer_size.is_null() {
        return QueueError::ReadPtrNull.into();
    }
    unsafe {
        // 将buffer转为切片
        let buffer_slice =
            core::slice::from_raw_parts_mut(buffer_addr as *mut u8, *buffer_size as usize);
        match queue_read(QueueId(queue_id), buffer_slice, timeout) {
            Ok(read_size) => {
                // 更新输出参数
                *buffer_size = read_size as u32;
                OK
            }
            Err(e) => e.into(),
        }
    }
}

/// 队列头部写入FFI函数
#[unsafe(export_name = "LOS_QueueWriteHead")]
pub extern "C" fn los_queue_write_head(
    queue_id: u32,
    buffer_addr: *mut c_void,
    buffer_size: u32,
    timeout: u32,
) -> u32 {
    if buffer_addr.is_null() {
        return QueueError::WritePtrNull.into();
    }
    unsafe {
        let buffer_slice =
            core::slice::from_raw_parts_mut(buffer_addr as *mut u8, buffer_size as usize);
        match queue_write_head(QueueId(queue_id), buffer_slice, timeout) {
            Ok(_) => OK,
            Err(e) => e.into(),
        }
    }
}

/// 队列尾部写入FFI函数
#[unsafe(export_name = "LOS_QueueWrite")]
pub extern "C" fn los_queue_write(
    queue_id: u32,
    buffer_addr: *mut c_void,
    buffer_size: u32,
    timeout: u32,
) -> u32 {
    if buffer_addr.is_null() {
        return QueueError::WritePtrNull.into();
    }
    unsafe {
        // 将buffer_addr转换为切片
        let buffer_slice =
            core::slice::from_raw_parts_mut(buffer_addr as *mut u8, buffer_size as usize);
        match queue_write(QueueId(queue_id), buffer_slice, timeout) {
            Ok(_) => OK,
            Err(e) => e.into(),
        }
    }
}
