//! RAMFS类型定义

use core::sync::atomic::{AtomicU32, Ordering};
use crate::utils::list::LinkedList;

/// RAMFS文件系统类型常量
pub const RAMFS_TYPE_DIR: u32 = 0x1; // VFS_TYPE_DIR
pub const RAMFS_TYPE_FILE: u32 = 0x2; // VFS_TYPE_FILE

/// RAMFS元素引用计数类型
pub type RefCount = AtomicU32;

/// RAMFS元素
#[repr(C)]
pub struct RamfsElement {
    /// 文件/目录名称
    pub name: [u8; 256], // LOS_MAX_FILE_NAME_LEN
    /// 文件类型 (目录或文件)
    pub element_type: u32,
    /// 兄弟节点指针
    pub sibling: Option<Box<RamfsElement>>,
    /// 父节点指针
    pub parent: Option<*mut RamfsElement>,
    /// 引用计数
    pub refs: RefCount,
    /// 文件或目录的具体内容
    pub content: ElementContent,
}

/// 元素内容的联合体
#[repr(C)]
pub union ElementContent {
    /// 文件内容
    pub file: FileContent,
    /// 目录内容
    pub dir: DirContent,
}

/// 文件内容结构
#[repr(C)]
pub struct FileContent {
    /// 文件大小
    pub size: usize,
    /// 文件内容指针
    pub content: Option<*mut u8>,
}

/// 目录内容结构
#[repr(C)]
pub struct DirContent {
    /// 子元素链表头
    pub child: Option<Box<RamfsElement>>,
}

/// RAMFS挂载点结构
#[repr(C)]
pub struct RamfsMountPoint {
    /// 根目录元素
    pub root: RamfsElement,
    /// 内存池指针
    pub memory: Option<*mut u8>,
}

impl RamfsElement {
    /// 创建新的目录元素
    pub fn new_dir(name: &str) -> Self {
        let mut name_array = [0u8; 256];
        let name_bytes = name.as_bytes();
        let copy_len = core::cmp::min(name_bytes.len(), 255);
        name_array[..copy_len].copy_from_slice(&name_bytes[..copy_len]);
        
        Self {
            name: name_array,
            element_type: RAMFS_TYPE_DIR,
            sibling: None,
            parent: None,
            refs: AtomicU32::new(0),
            content: ElementContent {
                dir: DirContent { child: None }
            },
        }
    }

    /// 创建新的文件元素
    pub fn new_file(name: &str) -> Self {
        let mut name_array = [0u8; 256];
        let name_bytes = name.as_bytes();
        let copy_len = core::cmp::min(name_bytes.len(), 255);
        name_array[..copy_len].copy_from_slice(&name_bytes[..copy_len]);
        
        Self {
            name: name_array,
            element_type: RAMFS_TYPE_FILE,
            sibling: None,
            parent: None,
            refs: AtomicU32::new(1),
            content: ElementContent {
                file: FileContent {
                    size: 0,
                    content: None,
                }
            },
        }
    }

    /// 获取名称字符串
    pub fn get_name(&self) -> &str {
        // 找到第一个空字节的位置
        let end = self.name.iter().position(|&b| b == 0).unwrap_or(self.name.len());
        core::str::from_utf8(&self.name[..end]).unwrap_or("")
    }

    /// 检查是否为目录
    pub fn is_dir(&self) -> bool {
        self.element_type == RAMFS_TYPE_DIR
    }

    /// 检查是否为文件
    pub fn is_file(&self) -> bool {
        self.element_type == RAMFS_TYPE_FILE
    }

    /// 增加引用计数
    pub fn inc_ref(&self) {
        self.refs.fetch_add(1, Ordering::SeqCst);
    }

    /// 减少引用计数
    pub fn dec_ref(&self) -> u32 {
        self.refs.fetch_sub(1, Ordering::SeqCst)
    }

    /// 获取当前引用计数
    pub fn get_ref_count(&self) -> u32 {
        self.refs.load(Ordering::SeqCst)
    }
}

impl RamfsMountPoint {
    /// 创建新的挂载点
    pub fn new(path: &str, memory: *mut u8) -> Self {
        Self {
            root: RamfsElement::new_dir(path),
            memory: Some(memory),
        }
    }
}