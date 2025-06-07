//! 硬件中断信息命令实现

use crate::interrupt::{is_interrupt_registered,types::{InterruptHandler}};
use crate::print_common;
use crate::shellcmd::types::{CmdType, ShellCmd};
use crate::ffi::exports::hwi::{os_get_hwi_form, os_get_hwi_form_cnt};
use core::ffi::c_char;

// 手动声明ShellCmd是线程安全的
// 这是安全的，因为我们知道cmd_key指向的是静态字符串
unsafe impl Sync for ShellCmd {}

// 常量声明
pub const OS_ERROR: u32 = u32::MAX; // 等同于 C 中的 (UINT32)(-1)
const LOSCFG_PLATFORM_HWI_LIMIT: u32 = 96;

/// 打印中断信息表头
pub fn print_hwi_info_title() {
    print_common!("InterruptNo     Share     ResponseCount     Name             DevId\n");
    print_common!("-----------     -----     -------------     ---------       --------\n");
}

/// 获取中断是否共享
#[inline]
pub fn get_hwi_share(_hwi_form: *mut InterruptHandler) -> bool {
    // 与原始C代码相同，始终返回false
    // 在实际实现中，可能会检查中断的共享状态
    false
}

/// 硬件中断信息命令实现
pub fn cmd_hwi(argc: i32, _argv: *const *const u8) -> u32 {
    // 参数检查
    if argc > 0 {
        print_common!("\nUsage: hwi\n");
        return OS_ERROR;
    }

    // 打印表头
    print_hwi_info_title();

    // 遍历所有中断
    let hwi_limit = LOSCFG_PLATFORM_HWI_LIMIT;
    for i in 0..hwi_limit {
        // 跳过未注册的中断
        if !is_interrupt_registered(i) {
            continue;
        }

        // 获取中断表单 - 不同的核心有不同的中断表单实现
        let hwi_form = os_get_hwi_form(i);
        if !hwi_form.is_null() {
            // 获取中断响应计数
            let count = os_get_hwi_form_cnt(i);

            // 打印中断信息
            print_common!(
                "{:<8}\t  {}\t  {:<10}",
                i,
                if get_hwi_share(hwi_form) { "Y" } else { "N" },
                count
            );
        }
    }

    0 // 成功返回
}

#[unsafe(no_mangle)]
pub unsafe extern "C" fn rust_hwi_cmd(argc: i32, argv: *const *const u8) -> u32 {
    cmd_hwi(argc, argv)
}

// 注册hwi命令
#[unsafe(no_mangle)]  // 防止编译器修改符号名
#[used]
#[unsafe(link_section = ".liteos.table.shellcmd.data")]
pub static hwi_shellcmd: ShellCmd = ShellCmd {
    cmd_type: CmdType::Ex,
    cmd_key: b"hwi\0".as_ptr() as *const c_char,
    para_num: 0,
    cmd_hook: rust_hwi_cmd,
};
