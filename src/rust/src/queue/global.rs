use crate::{config::QUEUE_LIMIT, queue::types::QueueControlBlock};
use alloc::collections::vec_deque::VecDeque;
use core::cell::RefCell;
use critical_section::Mutex;

pub static QUEUE_POOL: Mutex<RefCell<[QueueControlBlock; QUEUE_LIMIT as usize]>> = Mutex::new(
    RefCell::new([QueueControlBlock::UNINIT; QUEUE_LIMIT as usize]),
);

pub static UNUSED_QUEUE_LIST: Mutex<RefCell<VecDeque<usize>>> =
    Mutex::new(RefCell::new(VecDeque::new()));
