// use core::sync::atomic::{AtomicU32, Ordering};

use crate::{utils::list::LinkedList};

const OS_PRIORITY_QUEUE_NUM: usize = 32;
// const PRIQUEUE_PRIOR0_BIT: u32 = 0x8000_0000;

#[unsafe(no_mangle)]
pub static mut PRI_QUEUE_LIST: [LinkedList; OS_PRIORITY_QUEUE_NUM] =
    [LinkedList::UNINIT; OS_PRIORITY_QUEUE_NUM];

// static PRI_QUEUE_BITMAP: AtomicU32 = AtomicU32::new(0);

/// 初始化优先级队列
#[unsafe(export_name = "OsPriQueueInit")]
pub extern "C" fn os_pri_queue_init() {
    for priority in 0..OS_PRIORITY_QUEUE_NUM {
        LinkedList::init(&mut unsafe { PRI_QUEUE_LIST }[priority]);
    }
}

// /// 将任务节点插入优先级队列头部
// #[no_mangle]
// pub extern "C" fn OsPriQueueEnqueueHead(priqueue_item: *mut LinkedList, priority: u32) {
//     unsafe {
//         // 断言任务控制块初始化为零
//         assert!((*priqueue_item).next.is_null());

//         // 如果优先级队列为空，原子更新位图
//         if LinkedList::is_empty(&*PRI_QUEUE_LIST.add(priority as usize)) {
//             // 使用Ordering::Release确保在此之前的内存操作都完成
//             PRI_QUEUE_BITMAP.fetch_or(PRIQUEUE_PRIOR0_BIT >> priority, Ordering::Release);
//         }

//         LinkedList::head_insert(&mut *PRI_QUEUE_LIST.add(priority as usize), priqueue_item);
//     }
// }

// /// 从优先级队列中移除任务节点
// #[no_mangle]
// pub extern "C" fn OsPriQueueDequeue(priqueue_item: *mut LinkedList) {
//     unsafe {
//         // 从链表中删除节点
//         LinkedList::delete(priqueue_item);

//         // 获取包含此节点的任务控制块
//         let run_task = TaskCB::from_pend_list(priqueue_item);

//         // 如果该优先级队列为空，原子更新位图
//         if LinkedList::is_empty(&*PRI_QUEUE_LIST.add(run_task.priority as usize)) {
//             // 使用Ordering::Release确保在此之前的内存操作都完成
//             PRI_QUEUE_BITMAP.fetch_and(
//                 !(PRIQUEUE_PRIOR0_BIT >> run_task.priority),
//                 Ordering::Release,
//             );
//         }
//     }
// }

// /// 获取最高优先级的任务
// #[no_mangle]
// pub extern "C" fn OsGetTopTask() -> *mut TaskCB {
//     unsafe {
//         // 原子读取位图，使用Ordering::Acquire确保后续的内存访问不会被重排序到此操作之前
//         let mut bitmap = PRI_QUEUE_BITMAP.load(Ordering::Acquire);
//         let mut new_task: *mut TaskCB = core::ptr::null_mut();

//         while bitmap != 0 {
//             let priority = bitmap.leading_zeros();

//             let list_head = &mut *PRI_QUEUE_LIST.add(priority as usize);
//             if !LinkedList::is_empty(list_head) {
//                 let node = (*list_head).next;
//                 new_task = TaskCB::from_pend_list(node);

//                 OsPriQueueDequeue(node);
//                 break;
//             }

//             bitmap &= !(1u32 << (OS_PRIORITY_QUEUE_NUM as u32 - priority - 1));
//         }

//         new_task
//     }
// }
