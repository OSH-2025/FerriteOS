//! 栈信息注册和管理

use core::sync::atomic::{AtomicPtr, AtomicU32, Ordering};

use super::types::StackInfo;

/// 全局栈信息注册表
pub struct StackRegistry {
    stack_info: AtomicPtr<StackInfo>,
    stack_count: AtomicU32,
}

impl StackRegistry {
    const fn new() -> Self {
        Self {
            stack_info: AtomicPtr::new(core::ptr::null_mut()),
            stack_count: AtomicU32::new(0),
        }
    }

    /// 注册栈信息
    pub fn register(&self, stack_info: &[StackInfo]) {
        self.stack_info
            .store(stack_info.as_ptr() as *mut _, Ordering::Release);
        self.stack_count
            .store(stack_info.len() as u32, Ordering::Release);
    }

    /// 获取栈信息
    pub fn get(&self) -> (Option<&[StackInfo]>, u32) {
        let ptr = self.stack_info.load(Ordering::Acquire);
        let count = self.stack_count.load(Ordering::Acquire);

        if ptr.is_null() || count == 0 {
            (None, 0)
        } else {
            let slice = unsafe { core::slice::from_raw_parts(ptr, count as usize) };
            (Some(slice), count)
        }
    }
}

/// 全局栈注册表实例
pub static STACK_REGISTRY: StackRegistry = StackRegistry::new();

/// 注册栈信息
pub fn register_stack_info(stack_info: &[StackInfo]) {
    STACK_REGISTRY.register(stack_info);
}

/// 获取栈信息
pub fn get_stack_info() -> (Option<&'static [StackInfo]>, u32) {
    STACK_REGISTRY.get()
}
