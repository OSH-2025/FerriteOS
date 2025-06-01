use crate::exception::backtrace::{back_trace, task_back_trace};

#[unsafe(export_name = "LOS_BackTrace")]
pub extern "C" fn los_back_trace() {
    back_trace();
}

#[unsafe(export_name = "LOS_TaskBackTrace")]
pub extern "C" fn los_task_back_trace(task_id: u32) {
    task_back_trace(task_id);
}
