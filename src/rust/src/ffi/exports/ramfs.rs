use crate::{
    config::OK,
    components::ramfs::{
        core::RamfsCore,
        mount::RamfsMountManager,
        ops::{RamfsDirOps, RamfsFileOps},
        error::RamfsError,
    },
    vfs::types::{Dir, DirEntry, File, MountPoint},
};
use core::ffi::{c_char, c_int, c_void};

/// 初始化RAMFS文件系统
#[unsafe(export_name = "OsRamfsInit")]
pub extern "C" fn os_ramfs_init() -> u32 {
    match RamfsMountManager::init() {
        Ok(()) => OK,
        Err(e) => e.into(),
    }
}

/// 挂载RAMFS文件系统
#[unsafe(export_name = "LOS_RamfsMount")]
pub extern "C" fn los_ramfs_mount(path: *const c_char, block_size: usize) -> u32 {
    if path.is_null() {
        return RamfsError::NullPointer.into();
    }

    unsafe {
        let path_str = match core::ffi::CStr::from_ptr(path).to_str() {
            Ok(s) => s,
            Err(_) => return RamfsError::InvalidArgument.into(),
        };

        match RamfsMountManager::mount(path_str, block_size) {
            Ok(()) => OK,
            Err(e) => e.into(),
        }
    }
}

/// 卸载RAMFS文件系统
#[unsafe(export_name = "LOS_RamfsUnmount")]
pub extern "C" fn los_ramfs_unmount(path: *const c_char) -> u32 {
    if path.is_null() {
        return RamfsError::NullPointer.into();
    }

    unsafe {
        let path_str = match core::ffi::CStr::from_ptr(path).to_str() {
            Ok(s) => s,
            Err(_) => return RamfsError::InvalidArgument.into(),
        };

        match RamfsMountManager::unmount(path_str) {
            Ok(()) => OK,
            Err(e) => e.into(),
        }
    }
}

/// 打开文件
#[unsafe(export_name = "LOS_RamfsOpen")]
pub extern "C" fn los_ramfs_open(
    file: *mut File,
    path_in_mp: *const c_char,
    flags: c_int,
) -> u32 {
    if file.is_null() || path_in_mp.is_null() {
        return RamfsError::NullPointer.into();
    }

    unsafe {
        let path_str = match core::ffi::CStr::from_ptr(path_in_mp).to_str() {
            Ok(s) => s,
            Err(_) => return RamfsError::InvalidArgument.into(),
        };

        match RamfsCore::open(&mut *file, path_str, flags) {
            Ok(()) => OK,
            Err(e) => e.into(),
        }
    }
}

/// 关闭文件
#[unsafe(export_name = "LOS_RamfsClose")]
pub extern "C" fn los_ramfs_close(file: *mut File) -> u32 {
    if file.is_null() {
        return RamfsError::NullPointer.into();
    }

    unsafe {
        match RamfsCore::close(&mut *file) {
            Ok(()) => OK,
            Err(e) => e.into(),
        }
    }
}

/// 读取文件
#[unsafe(export_name = "LOS_RamfsRead")]
pub extern "C" fn los_ramfs_read(
    file: *mut File,
    buffer: *mut u8,
    bytes: usize,
) -> isize {
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

/// 写入文件
#[unsafe(export_name = "LOS_RamfsWrite")]
pub extern "C" fn los_ramfs_write(
    file: *mut File,
    buffer: *const u8,
    bytes: usize,
) -> isize {
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

/// 文件定位
#[unsafe(export_name = "LOS_RamfsLseek")]
pub extern "C" fn los_ramfs_lseek(
    file: *mut File,
    offset: i64,
    whence: c_int,
) -> i64 {
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

/// 删除文件或目录
#[unsafe(export_name = "LOS_RamfsUnlink")]
pub extern "C" fn los_ramfs_unlink(
    mp: *mut MountPoint,
    path_in_mp: *const c_char,
) -> u32 {
    if mp.is_null() || path_in_mp.is_null() {
        return RamfsError::NullPointer.into();
    }

    unsafe {
        let path_str = match core::ffi::CStr::from_ptr(path_in_mp).to_str() {
            Ok(s) => s,
            Err(_) => return RamfsError::InvalidArgument.into(),
        };

        match RamfsFileOps::unlink(&*mp, path_str) {
            Ok(()) => OK,
            Err(e) => e.into(),
        }
    }
}

/// 重命名文件或目录
#[unsafe(export_name = "LOS_RamfsRename")]
pub extern "C" fn los_ramfs_rename(
    mp: *mut MountPoint,
    old_path: *const c_char,
    new_path: *const c_char,
) -> u32 {
    if mp.is_null() || old_path.is_null() || new_path.is_null() {
        return RamfsError::NullPointer.into();
    }

    unsafe {
        let old_path_str = match core::ffi::CStr::from_ptr(old_path).to_str() {
            Ok(s) => s,
            Err(_) => return RamfsError::InvalidArgument.into(),
        };
        let new_path_str = match core::ffi::CStr::from_ptr(new_path).to_str() {
            Ok(s) => s,
            Err(_) => return RamfsError::InvalidArgument.into(),
        };

        match RamfsFileOps::rename(&*mp, old_path_str, new_path_str) {
            Ok(()) => OK,
            Err(e) => e.into(),
        }
    }
}

/// 打开目录
#[unsafe(export_name = "LOS_RamfsOpendir")]
pub extern "C" fn los_ramfs_opendir(
    dir: *mut Dir,
    path_in_mp: *const c_char,
) -> u32 {
    if dir.is_null() || path_in_mp.is_null() {
        return RamfsError::NullPointer.into();
    }

    unsafe {
        let path_str = match core::ffi::CStr::from_ptr(path_in_mp).to_str() {
            Ok(s) => s,
            Err(_) => return RamfsError::InvalidArgument.into(),
        };

        match RamfsDirOps::opendir(&mut *dir, path_str) {
            Ok(()) => OK,
            Err(e) => e.into(),
        }
    }
}

/// 读取目录项
#[unsafe(export_name = "LOS_RamfsReaddir")]
pub extern "C" fn los_ramfs_readdir(
    dir: *mut Dir,
    entry: *mut DirEntry,
) -> u32 {
    if dir.is_null() || entry.is_null() {
        return RamfsError::NullPointer.into();
    }

    unsafe {
        match RamfsDirOps::readdir(&mut *dir, &mut *entry) {
            Ok(()) => OK,
            Err(e) => e.into(),
        }
    }
}

/// 关闭目录
#[unsafe(export_name = "LOS_RamfsClosedir")]
pub extern "C" fn los_ramfs_closedir(dir: *mut Dir) -> u32 {
    if dir.is_null() {
        return RamfsError::NullPointer.into();
    }

    unsafe {
        match RamfsDirOps::closedir(&mut *dir) {
            Ok(()) => OK,
            Err(e) => e.into(),
        }
    }
}

/// 创建目录
#[unsafe(export_name = "LOS_RamfsMkdir")]
pub extern "C" fn los_ramfs_mkdir(
    mp: *mut MountPoint,
    path_in_mp: *const c_char,
) -> u32 {
    if mp.is_null() || path_in_mp.is_null() {
        return RamfsError::NullPointer.into();
    }

    unsafe {
        let path_str = match core::ffi::CStr::from_ptr(path_in_mp).to_str() {
            Ok(s) => s,
            Err(_) => return RamfsError::InvalidArgument.into(),
        };

        match RamfsDirOps::mkdir(&*mp, path_str) {
            Ok(()) => OK,
            Err(e) => e.into(),
        }
    }
}

/// 获取文件状态信息
#[unsafe(export_name = "LOS_RamfsStat")]
pub extern "C" fn los_ramfs_stat(
    mp: *mut MountPoint,
    path_in_mp: *const c_char,
    stat_buf: *mut c_void,
) -> u32 {
    if mp.is_null() || path_in_mp.is_null() || stat_buf.is_null() {
        return RamfsError::NullPointer.into();
    }

    // 暂时返回不支持，后续可以实现
    RamfsError::NotSupported.into()
}

/// 同步文件数据
#[unsafe(export_name = "LOS_RamfsSync")]
pub extern "C" fn los_ramfs_sync(file: *mut File) -> u32 {
    if file.is_null() {
        return RamfsError::NullPointer.into();
    }

    // RAMFS是内存文件系统，无需同步
    OK
}