mod global;
mod idle;
mod sched;
mod types;

pub use types::{TaskCB, TaskEntryFunc, TaskInitParam};

unsafe extern "C" {
    #[link_name = "LOS_TaskCreate"]
    unsafe fn los_task_create_wrapper(task_id: &mut u32, init_param: &mut TaskInitParam) -> u32;
}

pub fn los_task_create(task_id: &mut u32, init_param: &mut TaskInitParam) -> u32 {
    unsafe { los_task_create_wrapper(task_id, init_param) }
}
