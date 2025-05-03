use crate::bindings::config::get_os_sys_mem_size;
use crate::utils::dl_list::DlList;
use crate::utils::printf::dprintf;

/// The start address of the exception interaction dynamic memory pool.
/// When the exception interaction feature is not supported, `m_aucSysMem0` equals `m_aucSysMem1`.
#[unsafe(no_mangle)]
#[allow(non_upper_case_globals)]
pub static mut m_aucSysMem0: *mut u8 = core::ptr::null_mut();

/// The start address of the system dynamic memory pool.
#[unsafe(no_mangle)]
#[allow(non_upper_case_globals)]
pub static mut m_aucSysMem1: *mut u8 = core::ptr::null_mut();

#[unsafe(link_section = ".data.init")]
#[unsafe(no_mangle)]
#[allow(non_upper_case_globals)]
pub static mut g_sys_mem_addr_end: usize = 0;

#[repr(C)]
pub union NodeInfo {
    pub free_node_info: DlList,
    pub used_node_info: UsedNodeInfo,
}

#[repr(C)]
#[derive(Debug, Copy, Clone)]
pub struct UsedNodeInfo {
    pub magic: u32,
    pub task_id: u16,
}

#[repr(C)]
pub struct LosMemCtlNode {
    pub node_info: NodeInfo,
    pub pre_node: *mut LosMemDynNode,
    pub size_and_flag: u32,
}

#[repr(C)]
pub struct LosMemDynNode {
    pub self_node: LosMemCtlNode,
}

impl LosMemDynNode {
    // fn set_task_id(&mut self, task_id: u32) {
    //     self.self_node.node_info.used_node_info.task_id = task_id as u16;
    // }

    // fn get_task_id(&self) -> u32 {
    //     unsafe { self.self_node.node_info.used_node_info.task_id as u32 }
    // }
}

#[unsafe(export_name = "OsMemSystemInit")]
pub unsafe extern "C" fn os_mem_system_init(mem_start: usize) -> u32 {
    unsafe { m_aucSysMem1 = mem_start as *mut u8 };
    let pool_size = get_os_sys_mem_size();
    let ret = los_mem_init(unsafe { m_aucSysMem1 } as *mut core::ffi::c_void, pool_size);
    unsafe {
        dprintf(
            b"LiteOS system heap memory address:%p,size:0x%x\n\0" as *const u8,
            m_aucSysMem1,
            pool_size,
        )
    };
    unsafe { m_aucSysMem0 = m_aucSysMem1 };
    ret
}

unsafe extern "C" {
    #[link_name = "LOS_MemInit"]
    unsafe fn los_mem_init_wrapper(pool: *mut core::ffi::c_void, size: u32) -> u32;
}

pub fn los_mem_init(pool: *mut core::ffi::c_void, size: u32) -> u32 {
    unsafe { los_mem_init_wrapper(pool, size) }
}
