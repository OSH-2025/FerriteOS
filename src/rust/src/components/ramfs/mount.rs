//! RAMFS挂载管理

use alloc::{boxed::Box, string::String};
use core::mem;

use crate::{
    mem::memory::{MemInit, MemoryPool},
    result::SystemResult,
    vfs::types::{FileSystem, MountPoint},
};

use super::{
    error::RamfsError,
    ops::RAMFS_FILE_OPS,
    types::{RamfsElement, RamfsMountPoint},
};

/// RAMFS文件系统结构
static mut RAMFS_FILESYSTEM: FileSystem = FileSystem {
    name: "ramfs",
    ops: &RAMFS_FILE_OPS,
    mount_data: core::ptr::null_mut(),
    flags: 0,
};

/// RAMFS挂载管理器
pub struct RamfsMountManager;

impl RamfsMountManager {
    /// 挂载RAMFS文件系统
    /// 
    /// # 参数
    /// * `path` - 挂载路径
    /// * `block_size` - 内存块大小
    /// 
    /// # 返回值
    /// 成功返回Ok(())，失败返回错误码
    pub fn mount(path: &str, block_size: usize) -> SystemResult<()> {
        // 检查路径长度
        if path.len() >= 256 {
            return Err(RamfsError::NameTooLong.into());
        }

        // 分配挂载点内存
        let mount_point = Box::new(RamfsMountPoint::new(path, core::ptr::null_mut()));
        let mount_ptr = Box::into_raw(mount_point);

        // 分配内存池
        let memory = unsafe {
            let layout = core::alloc::Layout::from_size_align(block_size, 8)
                .map_err(|_| RamfsError::InvalidArgument)?;
            alloc::alloc::alloc(layout)
        };

        if memory.is_null() {
            unsafe {
                let _ = Box::from_raw(mount_ptr);
            }
            return Err(RamfsError::NoMemory.into());
        }

        // 初始化内存池
        unsafe {
            (*mount_ptr).memory = Some(memory);
            if MemoryPool::init(memory, block_size).is_err() {
                alloc::alloc::dealloc(
                    memory,
                    core::alloc::Layout::from_size_align(block_size, 8).unwrap(),
                );
                let _ = Box::from_raw(mount_ptr);
                return Err(RamfsError::NoMemory.into());
            }
        }

        // 执行VFS挂载
        unsafe {
            if crate::vfs::mount::fs_mount("ramfs", path, mount_ptr as *mut u8).is_err() {
                // 挂载失败，清理资源
                alloc::alloc::dealloc(
                    memory,
                    core::alloc::Layout::from_size_align(block_size, 8).unwrap(),
                );
                let _ = Box::from_raw(mount_ptr);
                return Err(RamfsError::InvalidArgument.into());
            }
        }

        Ok(())
    }

    /// 卸载RAMFS文件系统
    pub fn unmount(path: &str) -> SystemResult<()> {
        // 调用VFS卸载
        unsafe {
            crate::vfs::mount::fs_unmount(path)?;
        }
        Ok(())
    }

    /// 初始化RAMFS文件系统
    pub fn init() -> SystemResult<()> {
        static mut RAMFS_INITED: bool = false;
        
        unsafe {
            if RAMFS_INITED {
                return Ok(());
            }

            // 初始化VFS
            if crate::vfs::init::vfs_init().is_err() {
                return Err(RamfsError::InvalidArgument.into());
            }

            // 注册文件系统
            if crate::vfs::mount::fs_register(&mut RAMFS_FILESYSTEM).is_err() {
                return Err(RamfsError::InvalidArgument.into());
            }

            RAMFS_INITED = true;
        }

        Ok(())
    }
}