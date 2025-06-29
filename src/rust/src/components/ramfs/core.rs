//! RAMFS核心功能实现

use alloc::{boxed::Box, string::String, vec::Vec};
use core::cmp;

use crate::{
    mem::memory::MemoryPool,
    result::SystemResult,
    vfs::types::{DirEntry, File, MountPoint, VfsError},
};

use super::{
    element::ElementManager,
    error::RamfsError,
    types::{
        ElementContent, FileContent, RamfsElement, RamfsMountPoint, 
        RAMFS_TYPE_DIR, RAMFS_TYPE_FILE
    },
};

/// RAMFS核心操作管理器
pub struct RamfsCore;

impl RamfsCore {
    /// 打开文件
    pub fn open(file: &mut File, path_in_mp: &str, flags: i32) -> SystemResult<()> {
        // 检查是否试图打开根目录
        if path_in_mp.is_empty() {
            return Err(RamfsError::IsDirectory.into());
        }

        let (element, remaining_path) = ElementManager::find_element(&file.mount_point, path_in_mp)?;
        
        unsafe {
            // 如果找到了完整路径的元素
            if remaining_path.is_empty() {
                let ramfs_file = &mut *element;
                
                if ramfs_file.is_dir() {
                    return Err(RamfsError::IsDirectory.into());
                }

                // 检查创建标志冲突
                if (flags & 0x40 != 0) && (flags & 0x80 != 0) { // O_CREAT && O_EXCL
                    return Err(RamfsError::FileExists.into());
                }

                // 如果是追加模式，设置文件偏移到末尾
                if flags & 0x400 != 0 { // O_APPEND
                    file.offset = ramfs_file.content.file.size as i64;
                }

                ramfs_file.inc_ref();
                file.private_data = element as *mut u8;
                return Ok(());
            }

            // 文件不存在的情况
            if flags & 0x40 == 0 { // 没有O_CREAT标志
                return Err(RamfsError::FileNotFound.into());
            }

            let parent = &mut *element;
            if !parent.is_dir() {
                return Err(RamfsError::NotDirectory.into());
            }

            // 检查路径中不能有多级目录
            if remaining_path.contains('/') {
                return Err(RamfsError::FileNotFound.into());
            }

            // 检查文件名长度
            if remaining_path.len() >= 256 {
                return Err(RamfsError::NameTooLong.into());
            }

            // 创建新文件
            let new_file = Box::new(RamfsElement::new_file(&remaining_path));
            let new_file_ptr = new_file.as_ref() as *const RamfsElement as *mut RamfsElement;
            
            ElementManager::add_child_to_dir(parent, new_file)?;
            
            file.private_data = new_file_ptr as *mut u8;
        }

        Ok(())
    }

    /// 关闭文件
    pub fn close(file: &mut File) -> SystemResult<()> {
        if !file.private_data.is_null() {
            unsafe {
                let ramfs_file = &mut *(file.private_data as *mut RamfsElement);
                ramfs_file.dec_ref();
            }
        }
        Ok(())
    }

    /// 读取文件
    pub fn read(file: &mut File, buffer: &mut [u8]) -> SystemResult<usize> {
        if file.private_data.is_null() {
            return Err(RamfsError::InvalidArgument.into());
        }

        unsafe {
            let ramfs_file = &mut *(file.private_data as *mut RamfsElement);
            
            if file.offset < 0 {
                file.offset = 0;
            }

            let file_size = ramfs_file.content.file.size;
            if file_size <= file.offset as usize {
                return Ok(0); // 没有数据可读
            }

            let available = file_size - file.offset as usize;
            let to_read = cmp::min(buffer.len(), available);

            if let Some(content_ptr) = ramfs_file.content.file.content {
                let src = core::slice::from_raw_parts(
                    content_ptr.add(file.offset as usize),
                    to_read
                );
                buffer[..to_read].copy_from_slice(src);
            }

            file.offset += to_read as i64;
            Ok(to_read)
        }
    }

    /// 写入文件
    pub fn write(file: &mut File, buffer: &[u8]) -> SystemResult<usize> {
        if file.private_data.is_null() {
            return Err(RamfsError::InvalidArgument.into());
        }

        unsafe {
            let ramfs_file = &mut *(file.private_data as *mut RamfsElement);
            let mount_point = file.mount_point.get_mount_data() as *mut RamfsMountPoint;
            
            if file.offset < 0 {
                file.offset = 0;
            }

            let new_size = file.offset as usize + buffer.len();
            let current_size = ramfs_file.content.file.size;

            // 如果需要扩展文件大小
            if new_size > current_size {
                if let Some(memory_pool) = (*mount_point).memory {
                    let new_content = if let Some(old_content) = ramfs_file.content.file.content {
                        // 重新分配内存
                        MemoryPool::realloc(memory_pool, old_content, new_size)
                    } else {
                        // 首次分配内存
                        MemoryPool::alloc(memory_pool, new_size)
                    };

                    if let Some(content_ptr) = new_content {
                        ramfs_file.content.file.content = Some(content_ptr);
                        ramfs_file.content.file.size = new_size;
                    } else {
                        // 内存分配失败，只能写入现有空间
                        if current_size <= file.offset as usize {
                            return Err(RamfsError::NoMemory.into());
                        }
                        let writable = current_size - file.offset as usize;
                        return Self::write_partial(file, buffer, writable);
                    }
                } else {
                    return Err(RamfsError::NoMemory.into());
                }
            }

            // 写入数据
            if let Some(content_ptr) = ramfs_file.content.file.content {
                let dst = core::slice::from_raw_parts_mut(
                    content_ptr.add(file.offset as usize),
                    buffer.len()
                );
                dst.copy_from_slice(buffer);
            }

            file.offset += buffer.len() as i64;
            Ok(buffer.len())
        }
    }

    /// 部分写入辅助函数
    fn write_partial(file: &mut File, buffer: &[u8], max_bytes: usize) -> SystemResult<usize> {
        let to_write = cmp::min(buffer.len(), max_bytes);
        
        unsafe {
            let ramfs_file = &mut *(file.private_data as *mut RamfsElement);
            if let Some(content_ptr) = ramfs_file.content.file.content {
                let dst = core::slice::from_raw_parts_mut(
                    content_ptr.add(file.offset as usize),
                    to_write
                );
                dst.copy_from_slice(&buffer[..to_write]);
            }
        }

        file.offset += to_write as i64;
        Ok(to_write)
    }

    /// 文件定位
    pub fn lseek(file: &mut File, offset: i64, whence: i32) -> SystemResult<i64> {
        if file.private_data.is_null() {
            return Err(RamfsError::InvalidArgument.into());
        }

        unsafe {
            let ramfs_file = &*(file.private_data as *mut RamfsElement);
            let file_size = ramfs_file.content.file.size as i64;

            match whence {
                0 => file.offset = offset,        // SEEK_SET
                1 => file.offset += offset,       // SEEK_CUR
                2 => file.offset = file_size,     // SEEK_END
                _ => return Err(RamfsError::InvalidArgument.into()),
            }

            if file.offset < 0 {
                file.offset = 0;
            }

            if file.offset > file_size {
                file.offset = file_size;
            }

            Ok(file.offset)
        }
    }
}
