use core::ffi::c_char;
use core::ffi::c_void;

/// 硬件中断处理程序信息结构体
// #[repr(C)]
// pub struct HwiHandleInfo {
//     /// 用户注册的回调函数
//     pub hook: Option<extern "C" fn()>,
//     /// 中断响应计数
//     pub resp_count: u32,
// }

/// 命令类型枚举
#[repr(u32)]
pub enum CmdType {
    /// 显示命令
    Show = 0,
    /// 标准命令
    Std = 1,
    /// 扩展命令
    Ex = 2,
    /// 边界值
    Butt
}

/// 命令回调函数类型
pub type CmdCallBackFunc = unsafe extern "C" fn(i32, *const *const u8) -> u32;

/// Shell命令结构体 - 匹配C语言的CmdItem结构体
#[repr(C)]
pub struct ShellCmd {
    /// 命令类型
    pub cmd_type: CmdType,
    /// 命令名称（以null结尾的字符串）
    pub cmd_key: *const c_char,
    /// 参数数量
    pub para_num: u32,
    /// 命令回调函数
    pub cmd_hook: CmdCallBackFunc,
}

// 双向链表节点结构
#[repr(C)]
pub struct LosDlList {
    pub pst_prev: *mut LosDlList,  // 指向前一个节点的指针
    pub pst_next: *mut LosDlList,  // 指向下一个节点的指针
}

// 排序链表结构
#[repr(C)]
pub struct SortLinkList {
    pub sort_link_node: LosDlList,  // 链表节点
    pub idx_roll_num: u32,           // 索引滚动计数
}

// 软件定时器控制块
#[repr(C)]
pub struct LosSwtmrCB {
    pub sort_list: SortLinkList, // 需要添加这个字段
    pub state: u8,               // 修改为u8
    pub mode: u8,                // 修改为u8
    pub overrun: u8,             // 需要添加这个字段
    pub timer_id: u16,           // 修改为u16
    pub interval: u32,
    pub expiry: u32,             // 需要添加这个字段
    pub arg: usize,              // UINTPTR对应usize
    pub handler: *mut c_void,    // SWTMR_PROC_FUNC对应函数指针
}

// 外部C定义的全局变量
unsafe extern "C" {
    pub static g_swtmrCBArray: *mut LosSwtmrCB;
}