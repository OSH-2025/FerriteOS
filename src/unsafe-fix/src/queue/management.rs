//! 消息队列核心实现
use crate::{
    config::QUEUE_LIMIT,
    queue::{
        error::QueueError,
        global::{QUEUE_POOL, UNUSED_QUEUE_LIST},
        types::QueueId,
    },
    result::SystemResult,
};
use critical_section::with;

/// 初始化队列系统
#[inline]
pub fn init_queue_system() {
    with(|cs| {
        let mut queue_pool = QUEUE_POOL.borrow_ref_mut(cs);
        let mut unused_list = UNUSED_QUEUE_LIST.borrow_ref_mut(cs);
        queue_pool
            .iter_mut()
            .enumerate()
            .for_each(|(index, queue)| {
                queue.set_id(QueueId(index as u32));
                unused_list.push_back(index);
            });
    })
}

/// 内部队列创建函数
fn create_queue_internal(capacity: usize, slot_size: usize) -> SystemResult<QueueId> {
    // 临界区开始
    with(|cs| {
        // 检查是否有可用队列控制块
        let mut unused_list = UNUSED_QUEUE_LIST.borrow_ref_mut(cs);
        let index = unused_list.pop_front().ok_or(QueueError::Unavailable)?;
        let mut queue_pool = QUEUE_POOL.borrow_ref_mut(cs);
        let queue = queue_pool.get_mut(index).unwrap();
        queue.initialize(capacity, slot_size);
        let queue_id = queue.get_id();
        Ok(queue_id)
    })
}

/// 创建动态内存队列
pub fn create_queue(capacity: usize, message_size: usize) -> SystemResult<QueueId> {
    // 参数检查
    if message_size > (usize::MAX - 4) {
        return Err(QueueError::SizeTooBig.into());
    }

    if capacity == 0 || message_size == 0 {
        return Err(QueueError::ParaIsZero.into());
    }

    let slot_size = message_size + 4;

    // 调用内部创建函数
    match create_queue_internal(capacity, slot_size) {
        Ok(queue_id) => Ok(queue_id),
        Err(err) => Err(err),
    }
}

/// 删除消息队列
pub fn delete_queue(queue_id: QueueId) -> SystemResult<()> {
    // 检查队列索引是否有效
    let index = queue_id.get_index();
    if index as u32 >= QUEUE_LIMIT {
        return Err(QueueError::NotFound.into());
    }

    with(|cs| {
        // 获取队列控制块
        let mut queue_pool = QUEUE_POOL.borrow_ref_mut(cs);
        let queue = queue_pool.get_mut(index as usize).unwrap();

        // 临界区开始 - 验证队列状态
        if !queue.matches_id(queue_id) || queue.is_unused() {
            return Err(QueueError::NotCreate.into());
        }

        // 检查是否有任务在等待队列
        if queue.has_waiting_tasks() {
            return Err(QueueError::InTaskUse.into());
        }

        // 检查队列是否存在读写不一致
        if queue.is_read_write_inconsistent() {
            return Err(QueueError::InTaskWrite.into());
        }

        // 回收队列资源
        queue.reset();
        let mut unused_list = UNUSED_QUEUE_LIST.borrow_ref_mut(cs);
        // 将队列索引添加到未使用列表
        unused_list.push_back(index as usize);

        Ok(())
    })
}
