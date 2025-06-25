use critical_section::with;

use crate::{
    config::QUEUE_LIMIT,
    queue::{
        error::QueueError,
        global::QUEUE_POOL,
        types::{QueueId, QueueInfo},
    },
    result::SystemResult,
};

/// 获取队列信息
pub fn get_queue_info(queue_id: QueueId, queue_info: &mut QueueInfo) -> SystemResult<()> {
    // 检查队列ID是否有效
    let index = queue_id.get_index();
    if index as u32 >= QUEUE_LIMIT {
        return Err(QueueError::NotFound.into());
    }

    with(|cs| {
        let mut queue_pool = QUEUE_POOL.borrow_ref_mut(cs);
        let queue = queue_pool.get_mut(index as usize).unwrap();
        // 临界区开始 - 验证队列状态
        if !queue.matches_id(queue_id) || queue.is_unused() {
            return Err(QueueError::NotCreate.into());
        }

        // 填充队列信息
        *queue_info = queue.get_info();
        // restore_interrupt_state(int_save);
        Ok(())
    })
}

/// 获取当前使用的消息队列数量
#[inline]
#[unsafe(export_name = "OsUsedQueueCountGet")]
pub fn get_used_count() -> usize {
    with(|cs| {
        QUEUE_POOL
            .borrow_ref(cs)
            .iter()
            .filter(|queue| !queue.is_unused())
            .count()
    })
}

/// 打印当前使用的消息队列信息
#[inline]
#[unsafe(export_name = "OsUsedQueueInfoPrint")]
pub fn print_used_info() {
    with(|cs| {
        QUEUE_POOL
            .borrow_ref(cs)
            .iter()
            .filter(|queue| !queue.is_unused())
            .for_each(|queue| {
                queue.print_info();
            });
    })
}
