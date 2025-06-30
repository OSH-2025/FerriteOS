//! RAMFS文件操作接口

use crate::{
    result::SystemResult,
    vfs::types::{Dir, DirEntry, File, FileOperations, MountPoint},
};

use super::{
    core::RamfsCore,
    element::ElementManager,
    error::RamfsError,
    types::{RamfsElement, RAMFS_TYPE_DIR, RAMFS_TYPE_FILE},
};

/// RAMFS目录操作
pub struct RamfsDirOps;

impl RamfsDirOps {
    /// 打开目录
    pub fn opendir(dir: &mut Dir, path_in_mp: &str) -> SystemResult<()> {
        let (element, remaining_path) = ElementManager::find_element(&dir.mount_point, path_in_mp)?;
        
        unsafe {
            if !remaining_path.is_empty() {
                return Err(RamfsError::FileNotFound.into());
            }

            let ramfs_dir = &mut *element;
            if !ramfs_dir.is_dir() {
                return Err(RamfsError::NotDirectory.into());
            }

            ramfs_dir.inc_ref();
            dir.private_data = element as *mut u8;
            dir.offset = 0;
        }

        Ok(())
    }

    /// 读取目录项
    pub fn readdir(dir: &mut Dir, entry: &mut DirEntry) -> SystemResult<()> {
        if dir.private_data.is_null() {
            return Err(RamfsError::InvalidArgument.into());
        }

        unsafe {
            let ramfs_dir = &*(dir.private_data as *mut RamfsElement);
            let mut current_child = ramfs_dir.content.dir.child.as_ref();
            let mut index = 0i64;

            // 遍历到指定的偏移位置
            while index < dir.offset && current_child.is_some() {
                current_child = current_child.unwrap().sibling.as_ref();
                index += 1;
            }

            // 如果没有更多子项
            if current_child.is_none() {
                return Err(RamfsError::FileNotFound.into());
            }

            let child = current_child.unwrap();
            
            // 填充目录项信息
            let name = child.get_name();
            if name.len() >= 256 {
                return Err(RamfsError::NameTooLong.into());
            }
            
            entry.name[..name.len()].copy_from_slice(name.as_bytes());
            entry.name[name.len()..].fill(0);
            entry.size = 0;

            if child.is_dir() {
                entry.entry_type = RAMFS_TYPE_DIR;
            } else {
                entry.entry_type = RAMFS_TYPE_FILE;
                entry.size = child.content.file.size;
            }

            dir.offset += 1;
        }

        Ok(())
    }

    /// 关闭目录
    pub fn closedir(dir: &mut Dir) -> SystemResult<()> {
        if !dir.private_data.is_null() {
            unsafe {
                let ramfs_dir = &mut *(dir.private_data as *mut RamfsElement);
                ramfs_dir.dec_ref();
            }
        }
        Ok(())
    }

    /// 创建目录
    pub fn mkdir(mp: &MountPoint, path_in_mp: &str) -> SystemResult<()> {
        let (parent_element, remaining_path) = ElementManager::find_element(mp, path_in_mp)?;
        
        unsafe {
            if remaining_path.is_empty() {
                return Err(RamfsError::FileExists.into()); // 目录已存在
            }

            let parent = &mut *parent_element;
            
            // 解析路径，确保只创建一级目录
            let next_slash = remaining_path.find('/');
            let (dir_name, rest) = if let Some(pos) = next_slash {
                let dir_name = &remaining_path[..pos];
                let mut rest = &remaining_path[pos..];
                
                // 跳过多余的斜杠
                while rest.starts_with('/') {
                    rest = &rest[1..];
                }
                
                if !rest.is_empty() {
                    return Err(RamfsError::FileNotFound.into()); // 试图在不存在的目录下创建
                }
                
                (dir_name, rest)
            } else {
                (remaining_path.as_str(), "")
            };

            // 检查目录名长度
            if dir_name.len() >= 256 {
                return Err(RamfsError::NameTooLong.into());
            }

            // 创建新目录
            let new_dir = Box::new(RamfsElement::new_dir(dir_name));
            ElementManager::add_child_to_dir(parent, new_dir)?;
        }

        Ok(())
    }
}

/// RAMFS文件操作
pub struct RamfsFileOps;

impl RamfsFileOps {
    /// 删除文件或目录
    pub fn unlink(mp: &MountPoint, path_in_mp: &str) -> SystemResult<()> {
        let (element, remaining_path) = ElementManager::find_element(mp, path_in_mp)?;
        
        unsafe {
            if !remaining_path.is_empty() {
                return Err(RamfsError::FileNotFound.into());
            }

            let ramfs_file = &mut *element;
            
            if ramfs_file.get_ref_count() != 0 {
                return Err(RamfsError::FileBusy.into());
            }

            if ramfs_file.is_dir() {
                if !ElementManager::is_dir_empty(ramfs_file) {
                    return Err(RamfsError::FileBusy.into()); // 目录非空
                }
            } else {
                // 释放文件内容
                if let Some(content_ptr) = ramfs_file.content.file.content {
                    let mount_data = mp.get_mount_data() as *mut crate::ramfs::types::RamfsMountPoint;
                    if let Some(memory_pool) = (*mount_data).memory {
                        crate::mem::memory::MemoryPool::free(memory_pool, content_ptr);
                    }
                    ramfs_file.content.file.content = None;
                }
            }

            // 从父目录中移除
            if let Some(parent_ptr) = ramfs_file.parent {
                let parent = &mut *parent_ptr;
                let _ = ElementManager::remove_child_from_dir(parent, ramfs_file.get_name());
            }
        }

        Ok(())
    }

    /// 重命名文件或目录
    pub fn rename(mp: &MountPoint, old_path: &str, new_path: &str) -> SystemResult<()> {
        let (old_element, old_remaining) = ElementManager::find_element(mp, old_path)?;
        let (new_parent, new_remaining) = ElementManager::find_element(mp, new_path)?;
        
        unsafe {
            if !old_remaining.is_empty() {
                return Err(RamfsError::FileNotFound.into());
            }

            if new_remaining.is_empty() {
                return Err(RamfsError::FileExists.into());
            }

            // 检查是否在同一目录下重命名
            if new_remaining.contains('/') {
                return Err(RamfsError::NotSupported.into());
            }

            let old_file = &mut *old_element;
            let new_parent_dir = &mut *new_parent;

            // 必须在同一目录下
            if old_file.parent != Some(new_parent_dir as *mut RamfsElement) {
                return Err(RamfsError::NotSupported.into());
            }

            // 检查新名称长度
            if new_remaining.len() >= 256 {
                return Err(RamfsError::NameTooLong.into());
            }

            // 更新名称
            let new_name_bytes = new_remaining.as_bytes();
            let copy_len = core::cmp::min(new_name_bytes.len(), 255);
            old_file.name[..copy_len].copy_from_slice(&new_name_bytes[..copy_len]);
            old_file.name[copy_len..].fill(0);
        }

        Ok(())
    }
}

/// RAMFS文件操作表
pub static RAMFS_FILE_OPS: FileOperations = FileOperations {
    open: Some(ramfs_open),
    close: Some(ramfs_close),
    read: Some(ramfs_read),
    write: Some(ramfs_write),
    lseek: Some(ramfs_lseek),
    lseek64: Some(ramfs_lseek64),
    stat: None,
    unlink: Some(ramfs_unlink),
    rename: Some(ramfs_rename),
    ioctl: None,
    sync: None,
    opendir: Some(ramfs_opendir),
    readdir: Some(ramfs_readdir),
    closedir: Some(ramfs_closedir),
    mkdir: Some(ramfs_mkdir),
};

// C接口包装函数
extern "C" fn ramfs_open(file: *mut File, path_in_mp: *const u8, flags: i32) -> i32 {
    if file.is_null() || path_in_mp.is_null() {
        return -1;
    }
    
    unsafe {
        let path_str = core::ffi::CStr::from_ptr(path_in_mp as *const i8)
            .to_str()
            .unwrap_or("");
        
        match RamfsCore::open(&mut *file, path_str, flags) {
            Ok(()) => 0,
            Err(_) => -1,
        }
    }
}

extern "C" fn ramfs_close(file: *mut File) -> i32 {
    if file.is_null() {
        return -1;
    }
    
    unsafe {
        match RamfsCore::close(&mut *file) {
            Ok(()) => 0,
            Err(_) => -1,
        }
    }
}

extern "C" fn ramfs_read(file: *mut File, buffer: *mut u8, bytes: usize) -> isize {
    if file.is_null() || buffer.is_null() {
        return -1;
    }
    
    unsafe {
        let buf_slice = core::slice::from_raw_parts_mut(buffer, bytes);
        match RamfsCore::read(&mut *file, buf_slice) {
            Ok(read_bytes) => read_bytes as isize,
            Err(_) => -1,
        }
    }
}

extern "C" fn ramfs_write(file: *mut File, buffer: *const u8, bytes: usize) -> isize {
    if file.is_null() || buffer.is_null() {
        return -1;
    }
    
    unsafe {
        let buf_slice = core::slice::from_raw_parts(buffer, bytes);
        match RamfsCore::write(&mut *file, buf_slice) {
            Ok(written_bytes) => written_bytes as isize,
            Err(_) => -1,
        }
    }
}

extern "C" fn ramfs_lseek(file: *mut File, offset: i64, whence: i32) -> i64 {
    if file.is_null() {
        return -1;
    }
    
    unsafe {
        match RamfsCore::lseek(&mut *file, offset, whence) {
            Ok(new_offset) => new_offset,
            Err(_) => -1,
        }
    }
}

extern "C" fn ramfs_lseek64(file: *mut File, offset: i64, whence: i32) -> i64 {
    ramfs_lseek(file, offset, whence)
}

extern "C" fn ramfs_unlink(mp: *mut MountPoint, path_in_mp: *const u8) -> i32 {
    if mp.is_null() || path_in_mp.is_null() {
        return -1;
    }
    
    unsafe {
        let path_str = core::ffi::CStr::from_ptr(path_in_mp as *const i8)
            .to_str()
            .unwrap_or("");
        
        match RamfsFileOps::unlink(&*mp, path_str) {
            Ok(()) => 0,
            Err(_) => -1,
        }
    }
}

extern "C" fn ramfs_rename(mp: *mut MountPoint, old_path: *const u8, new_path: *const u8) -> i32 {
    if mp.is_null() || old_path.is_null() || new_path.is_null() {
        return -1;
    }
    
    unsafe {
        let old_path_str = core::ffi::CStr::from_ptr(old_path as *const i8)
            .to_str()
            .unwrap_or("");
        let new_path_str = core::ffi::CStr::from_ptr(new_path as *const i8)
            .to_str()
            .unwrap_or("");
        
        match RamfsFileOps::rename(&*mp, old_path_str, new_path_str) {
            Ok(()) => 0,
            Err(_) => -1,
        }
    }
}

extern "C" fn ramfs_opendir(dir: *mut Dir, path_in_mp: *const u8) -> i32 {
    if dir.is_null() || path_in_mp.is_null() {
        return -1;
    }
    
    unsafe {
        let path_str = core::ffi::CStr::from_ptr(path_in_mp as *const i8)
            .to_str()
            .unwrap_or("");
        
        match RamfsDirOps::opendir(&mut *dir, path_str) {
            Ok(()) => 0,
            Err(_) => -1,
        }
    }
}

extern "C" fn ramfs_readdir(dir: *mut Dir, entry: *mut DirEntry) -> i32 {
    if dir.is_null() || entry.is_null() {
        return -1;
    }
    
    unsafe {
        match RamfsDirOps::readdir(&mut *dir, &mut *entry) {
            Ok(()) => 0,
            Err(_) => -1,
        }
    }
}

extern "C" fn ramfs_closedir(dir: *mut Dir) -> i32 {
    if dir.is_null() {
        return -1;
    }
    
    unsafe {
        match RamfsDirOps::closedir(&mut *dir) {
            Ok(()) => 0,
            Err(_) => -1,
        }
    }
}

extern "C" fn ramfs_mkdir(mp: *mut MountPoint, path_in_mp: *const u8) -> i32 {
    if mp.is_null() || path_in_mp.is_null() {
        return -1;
    }
    
    unsafe {
        let path_str = core::ffi::CStr::from_ptr(path_in_mp as *const i8)
            .to_str()
            .unwrap_or("");
        
        match RamfsDirOps::mkdir(&*mp, path_str) {
            Ok(()) => 0,
            Err(_) => -1,
        }
    }
}
