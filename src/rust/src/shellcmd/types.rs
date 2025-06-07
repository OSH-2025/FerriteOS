use core::ffi::c_char;

// /// 硬件中断处理程序信息结构体
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

