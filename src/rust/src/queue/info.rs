use crate::{
    config::QUEUE_LIMIT,
    interrupt::{disable_interrupts, restore_interrupt_state},
    queue::{
        error::QueueError,
        global::QueueManager,
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

    // 清零队列信息结构体

    // 保存中断状态并禁用中断
    let int_save = disable_interrupts();

    // 获取队列控制块
    let queue = QueueManager::get_queue_by_index(index as usize);

    // 临界区开始 - 验证队列状态
    if !queue.matches_id(queue_id) || queue.is_unused() {
        // 队列不存在或未创建
        restore_interrupt_state(int_save);
        return Err(QueueError::NotCreate.into());
    }

    // 填充队列信息
    *queue_info = queue.get_info();
    // 恢复中断状态
    restore_interrupt_state(int_save);

    Ok(())
}
