//! 消息队列核心实现
use crate::{
    config::QUEUE_LIMIT,
    memory::{free, malloc},
    queue::{
        error::QueueError,
        global::{QUEUE_POOL, UNUSED_QUEUE_LIST},
        types::{QueueControlBlock, QueueId, QueueMemoryType},
    },
    result::SystemResult,
    utils::list::LinkedList,
};
use core::ffi::c_void;
use critical_section::with;

/// 初始化队列系统
///
/// 此函数设置全局队列池并初始化空闲队列列表
#[inline]
pub fn init_queue_system() {
    with(|cs| {
        let mut queue_pool = QUEUE_POOL.borrow_ref_mut(cs);
        let mut unused_list = UNUSED_QUEUE_LIST.borrow_ref_mut(cs);
        unused_list.init_ref();
        queue_pool
            .iter_mut()
            .enumerate()
            .for_each(|(index, queue)| {
                queue.set_id(QueueId(index as u32));
                unused_list.tail_insert_ref(&raw mut queue.write_waiting_list);
            });
    })
}

/// 内部队列创建函数
fn create_queue_internal(
    queue_mem: *mut u8,
    mem_type: QueueMemoryType,
    queue_len: u16,
    queue_size: u16,
) -> SystemResult<QueueId> {
    // 临界区开始
    with(|cs| {
        // 检查是否有可用队列控制块
        let unused_list = UNUSED_QUEUE_LIST.borrow_ref(cs);
        if unused_list.is_empty_ref() {
            return Err(QueueError::Unavailable.into());
        }

        let node = unused_list.first_ref();
        LinkedList::remove(node);
        let queue = QueueControlBlock::from_list(node);
        queue.initialize(queue_mem, mem_type, queue_len, queue_size);
        let queue_id = queue.get_id();
        Ok(queue_id)
    })
}

/// 创建动态内存队列
pub fn create_queue(len: u16, msg_size: u16) -> SystemResult<QueueId> {
    // 参数检查
    if msg_size > (u16::MAX - 4) {
        return Err(QueueError::SizeTooBig.into());
    }

    if len == 0 || msg_size == 0 {
        return Err(QueueError::ParaIsZero.into());
    }

    let queue_size = msg_size + 4; // 4字节用于存储消息长度

    // 为队列分配内存
    let queue_mem = malloc(len as usize * queue_size as usize);
    if queue_mem.is_null() {
        return Err(QueueError::CreateNoMemory.into());
    }
    let queue_mem = queue_mem as *mut u8;

    // 调用内部创建函数
    match create_queue_internal(queue_mem, QueueMemoryType::Dynamic, len, queue_size) {
        Ok(queue_id) => Ok(queue_id),
        Err(err) => {
            // 创建失败，释放已分配的内存
            free(queue_mem as *mut c_void);
            Err(err)
        }
    }
}

/// 创建静态内存队列
#[cfg(feature = "queue-static-allocation")]
pub fn create_static_queue(
    len: u16,
    msg_size: u16,
    queue_mem: *mut u8,
    mem_size: u16,
) -> SystemResult<QueueId> {
    // 参数检查
    if msg_size > (u16::MAX - 4) {
        return Err(QueueError::SizeTooBig.into());
    }

    if len == 0 || msg_size == 0 {
        return Err(QueueError::ParaIsZero.into());
    }

    if queue_mem.is_null() {
        return Err(QueueError::CreatePtrNull.into());
    }

    let queue_size = msg_size + 4; // 4字节用于存储消息长度

    // 检查内存大小是否足够
    if mem_size < (len * msg_size) {
        return Err(QueueError::CreateNoMemory.into());
    }

    // 调用内部创建函数
    create_queue_internal(queue_mem, QueueMemoryType::Static, len, queue_size)
}

/// 删除消息队列
pub fn delete_queue(queue_id: QueueId) -> SystemResult<()> {
    // 检查队列索引是否有效
    let index = queue_id.get_index();
    if index as u32 >= QUEUE_LIMIT {
        return Err(QueueError::NotFound.into());
    }

    let res = with(|cs| {
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

        // 保存队列内存指针以便后续释放
        let queue_mem = queue.queue_mem;
        let mem_type = queue.get_mem_type();

        // 回收队列资源
        queue.reset();
        let mut unused_list = UNUSED_QUEUE_LIST.borrow_ref_mut(cs);
        unused_list.tail_insert_ref(&raw mut queue.write_waiting_list);

        Ok((queue_mem, mem_type))
    });

    match res {
        Ok((queue_mem, mem_type)) => {
            // 如果是静态分配的队列，直接返回
            if mem_type == QueueMemoryType::Dynamic {
                free(queue_mem as *mut c_void);
            }
            return Ok(());
        }
        Err(e) => return Err(e),
    }
}
