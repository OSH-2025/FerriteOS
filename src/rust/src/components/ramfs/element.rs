//! RAMFS元素操作

use crate::{
    result::SystemResult,
    vfs::types::{MountPoint, VfsNode},
};

use super::{
    error::RamfsError,
    types::{RamfsElement, RamfsMountPoint, RAMFS_TYPE_DIR, RAMFS_TYPE_FILE},
};

/// RAMFS元素查找和操作
pub struct ElementManager;

impl ElementManager {
    /// 在RAMFS中查找文件/目录
    /// 
    /// # 参数
    /// * `mp` - 挂载点
    /// * `path_in_mp` - 在挂载点内的路径
    /// * `path_unresolved` - 返回未解析的路径部分
    /// 
    /// # 返回值
    /// 找到的元素指针，如果未找到则返回None
    pub fn find_element(
        mp: &MountPoint,
        path_in_mp: &str,
    ) -> SystemResult<(*mut RamfsElement, String)> {
        let mount_data = mp.get_mount_data() as *mut RamfsMountPoint;
        if mount_data.is_null() {
            return Err(RamfsError::InvalidArgument.into());
        }

        let mut walk = unsafe { &mut (*mount_data).root as *mut RamfsElement };
        let mut remaining_path = path_in_mp.to_string();

        loop {
            // 检查当前元素是否为目录
            unsafe {
                if (*walk).element_type != RAMFS_TYPE_DIR {
                    return Err(RamfsError::NotDirectory.into());
                }
            }

            // 跳过前导斜杠
            while remaining_path.starts_with('/') {
                remaining_path = remaining_path[1..].to_string();
            }

            // 找到下一个路径分隔符
            let next_slash = remaining_path.find('/');
            let (current_name, rest) = if let Some(pos) = next_slash {
                (remaining_path[..pos].to_string(), remaining_path[pos..].to_string())
            } else {
                (remaining_path.clone(), String::new())
            };

            // 检查名称长度
            if current_name.len() >= 256 {
                return Err(RamfsError::NameTooLong.into());
            }

            // 如果没有更多路径要解析，返回当前元素
            if current_name.is_empty() {
                return Ok((walk, remaining_path));
            }

            // 在当前目录的子元素中查找
            let mut found = false;
            unsafe {
                if let Some(mut child) = (*walk).content.dir.child.as_mut() {
                    loop {
                        if child.get_name() == current_name {
                            walk = child.as_mut() as *mut RamfsElement;
                            remaining_path = rest;
                            found = true;
                            break;
                        }
                        
                        if let Some(sibling) = child.sibling.as_mut() {
                            child = sibling;
                        } else {
                            break;
                        }
                    }
                }
            }

            if !found {
                // 没有找到匹配的子元素
                return Ok((walk, remaining_path));
            }

            // 如果没有更多路径要解析，说明找到了目标
            if rest.is_empty() {
                return Ok((walk, String::new()));
            }
        }
    }

    /// 在目录中添加子元素
    pub fn add_child_to_dir(
        parent: &mut RamfsElement,
        child: Box<RamfsElement>,
    ) -> SystemResult<()> {
        if parent.element_type != RAMFS_TYPE_DIR {
            return Err(RamfsError::NotDirectory.into());
        }

        unsafe {
            // 将新子元素插入到子元素链表的头部
            let old_child = parent.content.dir.child.take();
            let mut new_child = child;
            new_child.sibling = old_child;
            new_child.parent = Some(parent as *mut RamfsElement);
            parent.content.dir.child = Some(new_child);
        }

        Ok(())
    }

    /// 从目录中移除子元素
    pub fn remove_child_from_dir(
        parent: &mut RamfsElement,
        child_name: &str,
    ) -> SystemResult<Box<RamfsElement>> {
        if parent.element_type != RAMFS_TYPE_DIR {
            return Err(RamfsError::NotDirectory.into());
        }

        unsafe {
            let mut current = parent.content.dir.child.as_mut();
            let mut prev: Option<&mut Box<RamfsElement>> = None;

            while let Some(ref mut element) = current {
                if element.get_name() == child_name {
                    // 找到要删除的元素
                    let removed = if let Some(prev_elem) = prev {
                        // 从链表中间删除
                        let mut removed = element.sibling.take().unwrap();
                        core::mem::swap(&mut prev_elem.sibling, &mut Some(removed));
                        prev_elem.sibling.take().unwrap()
                    } else {
                        // 从链表头部删除
                        let mut removed = parent.content.dir.child.take().unwrap();
                        parent.content.dir.child = removed.sibling.take();
                        removed
                    };
                    
                    return Ok(removed);
                }

                prev = current;
                current = element.sibling.as_mut();
            }
        }

        Err(RamfsError::FileNotFound.into())
    }

    /// 检查目录是否为空
    pub fn is_dir_empty(element: &RamfsElement) -> bool {
        if element.element_type != RAMFS_TYPE_DIR {
            return false;
        }

        unsafe { element.content.dir.child.is_none() }
    }
}