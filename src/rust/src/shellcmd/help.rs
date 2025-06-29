//! Shell help命令的Rust实现

use crate::print_common;
use crate::shellcmd::types::{CmdType, ShellCmd};
use core::ffi::c_char;

// 常量声明
pub const OS_ERROR: u32 = u32::MAX;

// 链表节点结构，匹配C中的LOS_DL_LIST
#[repr(C)]
pub struct LosListNode {
    pub prev: *mut LosListNode,
    pub next: *mut LosListNode,
}

// 命令项结构，匹配C中的CmdItem
#[repr(C)]
pub struct CmdItem {
    pub cmd_type: u32,  // CmdType
    pub cmd_key: *const c_char,  // 命令关键字
    pub para_num: u32,   // 参数数量
    pub cmd_hook: *const u8,  // 命令回调函数指针
}

// 命令项节点，匹配C中的CmdItemNode
#[repr(C)]
pub struct CmdItemNode {
    pub list: LosListNode,  // 链表节点
    pub cmd: *mut CmdItem,  // 指向命令项的指针
}

// 命令模块信息，匹配C中的CmdModInfo
#[repr(C)]
pub struct CmdModInfo {
    pub cmd_list: CmdItemNode,  // 命令列表头节点
    pub list_num: u32,          // 列表中命令数量
    pub init_magic_flag: u32,   // 初始化魔数标志
    pub mux_lock: u32,          // 互斥锁
    pub trans_id_hook: *const u8, // 事务ID钩子函数
}

// 外部C函数声明
unsafe extern "C" {
    fn OsCmdInfoGet() -> *const CmdModInfo;
}

/// help命令的实现
pub fn cmd_help(argc: i32, _argv: *const *const u8) -> u32 {
    // 参数检查
    if argc > 0 {
        print_common!("\nUsage: help\n");
        return OS_ERROR;
    }

    // 获取命令信息
    let cmd_info = unsafe { OsCmdInfoGet() };
    if cmd_info.is_null() {
        print_common!("Error: Cannot get command info\n");
        return OS_ERROR;
    }

    print_common!("*******************shell commands:*************************\n");

    let mut loop_count = 0u32;
    
    // 遍历命令列表 - 实现类似于 C 中的 LOS_DL_LIST_FOR_EACH_ENTRY
    unsafe {
        let cmd_list_head = &(*cmd_info).cmd_list.list;
        let mut current = cmd_list_head.next;
        
        // 遍历双向链表
        while !current.is_null() && current != cmd_list_head as *const _ as *mut _ {
            // 通过链表节点获取包含结构体CmdItemNode的指针
            // 这里使用偏移计算，类似于C中的container_of宏
            let cmd_item_node = current as *mut CmdItemNode;
            let cmd_item = (*cmd_item_node).cmd;
            
            if !cmd_item.is_null() {
                // 每8个命令换行，与C代码保持一致
                if (loop_count & 7) == 0 {  // 8-1 = 7，检查是否是8的倍数
                    print_common!("\n");
                }
                
                // 输出命令名称，左对齐12个字符，与C代码格式一致
                let cmd_key_ptr = (*cmd_item).cmd_key;
                if !cmd_key_ptr.is_null() {
                    // 将C字符串转换为Rust字符串用于打印
                    let cmd_key = core::ffi::CStr::from_ptr(cmd_key_ptr);
                    if let Ok(cmd_str) = cmd_key.to_str() {
                        print_common!("{:<12}  ", cmd_str);
                    }
                }
                
                loop_count += 1;
            }
            
            // 移动到下一个节点
            current = (*current).next;
            
            // 防止无限循环
            if current == cmd_list_head as *const _ as *mut _ {
                break;
            }
        }
    }

    print_common!("\n");
    0 // 成功返回
}

/// C接口函数
#[unsafe(no_mangle)]
pub unsafe extern "C" fn rust_help_cmd(argc: i32, argv: *const *const u8) -> u32 {
    cmd_help(argc, argv)
}

// 注册help命令
#[unsafe(no_mangle)]
#[used]
#[unsafe(link_section = ".liteos.table.shellcmd.data")]
pub static help_shellcmd: ShellCmd = ShellCmd {
    cmd_type: CmdType::Ex,
    cmd_key: b"help\0".as_ptr() as *const c_char,
    para_num: 0,
    cmd_hook: rust_help_cmd,
};
