use crate::kernel::base::misc::los_misc_pri::*;
use crate::kernel::base::sem::los_sem_pri::*;
use crate::kernel::base::task::los_task_pri::*;
use crate::kernel::base::typedef::*;
use crate::kernel::common::stdlib::*;
#[cfg(feature = "LOSCFG_SHELL")]
use crate::shell::shcmd::*;

const OS_ALL_SEM_MASK: u32 = 0xffffffff;

static mut G_SEM_DEBUG_ARRAY: [SemDebugCB; LOSCFG_BASE_IPC_SEM_LIMIT] = 
    [SemDebugCB::new(); LOSCFG_BASE_IPC_SEM_LIMIT];

#[derive(Clone, Copy)]
struct SemDebugCB {
    orig_sem_count: u16,
    last_access_time: u64,
    creator: Option<TskEntryFunc>,
}

impl SemDebugCB {
    const fn new() -> Self {
        Self {
            orig_sem_count: 0,
            last_access_time: 0,
            creator: None,
        }
    }
}

fn os_sem_pended_task_name_print(sem_node: &LosSemCB) {
    let mut name_arr: Vec<&str> = Vec::with_capacity(LOSCFG_BASE_CORE_TSK_LIMIT);
    
    let int_save = scheduler_lock();
    
    if sem_node.sem_stat == LOS_UNUSED || sem_node.sem_list.is_empty() {
        scheduler_unlock(int_save);
        return;
    }

    for task_cb in sem_node.sem_list.iter() {
        if let Some(task_name) = &task_cb.task_name {
            name_arr.push(task_name);
            if name_arr.len() >= LOSCFG_BASE_CORE_TSK_LIMIT {
                break;
            }
        }
    }
    scheduler_unlock(int_save);

    print!("Pended task list : ");
    for (i, name) in name_arr.iter().enumerate() {
        if i == 0 {
            println!("{}", name);
        } else {
            print!(", {}", name);
        }
    }
    println!();
}

fn sem_compare_value(sort_param: &SortParam, left: u32, right: u32) -> bool {
    unsafe {
        let left_addr = sort_elem_addr(sort_param, left) as *const u64;
        let right_addr = sort_elem_addr(sort_param, right) as *const u64;
        *left_addr > *right_addr
    }
}

pub fn os_sem_dbg_init() {
    unsafe {
        G_SEM_DEBUG_ARRAY = [SemDebugCB::new(); LOSCFG_BASE_IPC_SEM_LIMIT];
    }
}

pub fn os_sem_dbg_time_update(sem_id: u32) {
    let index = get_sem_index(sem_id);
    unsafe {
        G_SEM_DEBUG_ARRAY[index].last_access_time = los_tick_count_get();
    }
}

pub fn os_sem_dbg_update(sem_id: u32, creator: Option<TskEntryFunc>, count: u16) {
    let index = get_sem_index(sem_id);
    unsafe {
        let sem_debug = &mut G_SEM_DEBUG_ARRAY[index];
        sem_debug.creator = creator;
        sem_debug.last_access_time = los_tick_count_get();
        sem_debug.orig_sem_count = count;
    }
}

fn os_sem_sort(sem_index_array: &mut [u32], used_count: usize) {
    let int_save = scheduler_lock();
    
    let sem_sort_param = SortParam {
        buf: unsafe { G_SEM_DEBUG_ARRAY.as_ptr() as *mut u8 },
        ctrl_block_size: core::mem::size_of::<SemDebugCB>(),
        ctrl_block_cnt: LOSCFG_BASE_IPC_SEM_LIMIT,
        sort_elem_off: offset_of!(SemDebugCB, last_access_time),
    };

    println!("Used Semaphore List:");
    println!("\r\n   SemID    Count    OriginalCount   Creater(TaskEntry)    LastAccessTime");
    println!("   ------   ------   -------------   ------------------    --------------");

    os_array_sort(
        &mut sem_index_array[..used_count], 
        0, 
        used_count - 1, 
        &sem_sort_param, 
        sem_compare_value
    );
    scheduler_unlock(int_save);

    for &index in &sem_index_array[..used_count] {
        let sem_cb = get_sem(index);
        let int_save = scheduler_lock();
        
        let sem_node = sem_cb.clone();
        let sem_debug = unsafe { G_SEM_DEBUG_ARRAY[index] };
        
        scheduler_unlock(int_save);

        if sem_node.sem_stat != LOS_USED || sem_debug.creator.is_none() {
            continue;
        }

        println!(
            "   0x{:07x} 0x{:07} 0x{:014} {:22?} 0x{:x}",
            sem_node.sem_id,
            sem_debug.orig_sem_count,
            sem_node.sem_count,
            sem_debug.creator,
            sem_debug.last_access_time
        );

        if !sem_node.sem_list.is_empty() {
            os_sem_pended_task_name_print(&sem_node);
        }
    }
}

pub fn os_sem_info_get_full_data() -> Result<(), u32> {
    let mut used_sem_cnt = 0;
    let int_save = scheduler_lock();

    // 计算已使用的信号量数量
    for i in 0..LOSCFG_BASE_IPC_SEM_LIMIT {
        let sem_node = get_sem(i);
        let sem_debug = unsafe { &G_SEM_DEBUG_ARRAY[i] };
        if sem_node.sem_stat == LOS_USED && sem_debug.creator.is_some() {
            used_sem_cnt += 1;
        }
    }
    scheduler_unlock(int_save);

    if used_sem_cnt > 0 {
        let mut sem_index_array = Vec::with_capacity(used_sem_cnt);
        
        let int_save = scheduler_lock();
        for i in 0..LOSCFG_BASE_IPC_SEM_LIMIT {
            let sem_node = get_sem(i);
            let sem_debug = unsafe { &G_SEM_DEBUG_ARRAY[i] };
            if sem_node.sem_stat == LOS_USED && sem_debug.creator.is_some() {
                sem_index_array.push(i as u32);
                if sem_index_array.len() >= used_sem_cnt {
                    break;
                }
            }
        }
        scheduler_unlock(int_save);
        
        os_sem_sort(&mut sem_index_array, sem_index_array.len());
    }
    
    Ok(())
}

#[cfg(feature = "LOSCFG_SHELL")]
fn os_sem_info_output(sem_id: usize) -> Result<(), u32> {
    if sem_id == OS_ALL_SEM_MASK as usize {
        let mut sem_cnt = 0;
        
        for loop_idx in 0..LOSCFG_BASE_IPC_SEM_LIMIT {
            let sem_cb = get_sem(loop_idx);
            let int_save = scheduler_lock();
            
            if sem_cb.sem_stat == LOS_USED {
                let sem_node = sem_cb.clone();
                scheduler_unlock(int_save);
                sem_cnt += 1;
                println!("\r\n   SemID       Count\n   ----------  -----");
                println!("   0x{:08x}  {}", sem_node.sem_id, sem_node.sem_count);
                continue;
            }
            scheduler_unlock(int_save);
        }
        println!("   SemUsingNum    :  {}\n", sem_cnt);
    } else {
        let sem_cb = get_sem(sem_id);
        let int_save = scheduler_lock();
        let sem_node = sem_cb.clone();
        scheduler_unlock(int_save);
        
        if sem_node.sem_id != sem_id as u32 || sem_node.sem_stat != LOS_USED {
            println!("\nThe semaphore is not in use!");
            return Ok(());
        }

        println!("\r\n   SemID       Count\n   ----------  -----");
        println!("   0x{:08x}      0x{}", sem_node.sem_id, sem_node.sem_count);

        if sem_node.sem_list.is_empty() {
            println!("No task is pended on this semaphore!");
        } else {
            os_sem_pended_task_name_print(&sem_node);
        }
    }
    Ok(())
}

#[cfg(feature = "LOSCFG_SHELL")]
pub fn os_shell_cmd_sem_info_get(argc: u32, argv: &[&str]) -> u32 {
    if argc > 1 {
        println!("\nUsage: sem [fulldata|ID]");
        return OS_ERROR;
    }

    let sem_id = if argc == 0 {
        OS_ALL_SEM_MASK as usize
    } else {
        let arg = argv[0];
        if arg == "fulldata" {
            return match os_sem_info_get_full_data() {
                Ok(_) => LOS_OK,
                Err(e) => e,
            };
        }

        match arg.parse::<usize>() {
            Ok(id) if get_sem_index(id as u32) < LOSCFG_BASE_IPC_SEM_LIMIT => id,
            _ => {
                println!("\nsem ID can't access {}.", arg);
                return OS_ERROR;
            }
        }
    };

    match os_sem_info_output(sem_id) {
        Ok(_) => LOS_OK,
        Err(e) => e,
    }
}

#[cfg(feature = "LOSCFG_SHELL")]
static SEM_SHELL_CMD: ShellCmd = ShellCmd {
    cmd_type: CMD_TYPE_EX,
    cmd_key: "sem",
    param_num: 1,
    cmd_proc: os_shell_cmd_sem_info_get as *const (),
};
