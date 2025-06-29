//! RAMFS错误类型定义

use crate::result::SystemError;

/// RAMFS错误类型
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum RamfsError {
    /// 空指针错误
    NullPointer,
    /// 文件不存在
    FileNotFound,
    /// 不是目录
    NotDirectory,
    /// 是目录
    IsDirectory,
    /// 文件已存在
    FileExists,
    /// 没有内存
    NoMemory,
    /// 名称太长
    NameTooLong,
    /// 文件忙
    FileBusy,
    /// 无效参数
    InvalidArgument,
    /// 权限不足
    PermissionDenied,
    /// 不支持的操作
    NotSupported,
}

impl From<RamfsError> for SystemError {
    fn from(error: RamfsError) -> Self {
        match error {
            RamfsError::NullPointer => SystemError::InvalidParameter,
            RamfsError::FileNotFound => SystemError::NotFound,
            RamfsError::NotDirectory => SystemError::InvalidParameter,
            RamfsError::IsDirectory => SystemError::InvalidParameter,
            RamfsError::FileExists => SystemError::AlreadyExists,
            RamfsError::NoMemory => SystemError::OutOfMemory,
            RamfsError::NameTooLong => SystemError::InvalidParameter,
            RamfsError::FileBusy => SystemError::Busy,
            RamfsError::InvalidArgument => SystemError::InvalidParameter,
            RamfsError::PermissionDenied => SystemError::PermissionDenied,
            RamfsError::NotSupported => SystemError::NotSupported,
        }
    }
}