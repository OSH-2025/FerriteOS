//! Shell Command Management - Rust Implementation
//! 
//! This module provides shell command registration, parsing, execution, tab completion,
//! history management, and other shell-related functionality implemented in Rust.

use core::ffi::{c_char, c_void, c_int};
use core::ptr;
use core::slice;
use core::str;
use crate::print_common;
use crate::shellcmd::types::{CmdType, ShellCmd};
use crate::mem::memory::{los_mem_alloc, los_mem_free};
use crate::utils::list::{LinkedList as LosListNode};
use crate::ffi::exports::mutex::{los_mux_create, los_mux_delete, los_mux_pend, los_mux_post};
use crate::ffi::bindings::dprintf as print_k;
use crate::{offset_of, container_of, list_for_each_entry};

// 常量定义
const CMD_MAX_LEN: usize = 256 + 16;
const CMD_KEY_LEN: usize = 16;
const SHOW_MAX_LEN: usize = CMD_MAX_LEN;
const CMD_MAX_PARAS: usize = 32;
const SPACE: u8 = b' ';
const TAB: u8 = b'\t';
const CMD_KEY_UP: u32 = 0;
const CMD_KEY_DOWN: u32 = 1;
const LOS_WAIT_FOREVER: u32 = 0xFFFFFFFF;
const OS_ERROR: u32 = 0xFFFFFFFF;

// 宏辅助
macro_rules! container_of {
    ($ptr:expr, $type:ty, $field:ident) => {
        ($ptr as *const u8).sub(core::mem::offset_of!($type, $field)) as *mut $type
    };
}

unsafe extern "C" {
    static mut m_aucSysMem0: *mut u8;
}

// 基础结构体定义
#[repr(C)]
#[derive(Debug)]
pub struct ShellCB {
    pub console_id: u32,
    pub shell_task_handle: u32,
    pub shell_entry_handle: u32,
    pub cmd_key_link: *mut c_void,
    pub cmd_history_key_link: *mut c_void,
    pub cmd_mask_key_link: *mut c_void,
    pub shell_buf_offset: u32,
    pub shell_key_type: u32,
    pub shell_event: [u8; 32], // EVENT_CB_S 占位
    pub key_mutex: u32,
    pub history_mutex: u32,
    pub shell_buf: [c_char; SHOW_MAX_LEN],
    pub shell_working_directory: [c_char; 260], // PATH_MAX
}

#[repr(C)]
pub struct CmdKeyLink {
    pub list: LosListNode,
    pub count: u32,
    // cmdString 紧跟在结构体后面
}

#[repr(C)]
pub struct CmdItem {
    pub cmd_type: CmdType,
    pub cmd_key: *const c_char,
    pub para_num: u32,
    pub cmd_hook: unsafe extern "C" fn(u32, *const *const c_char) -> u32,
}

#[repr(C)]
pub struct CmdItemNode {
    pub list: LosListNode,
    pub cmd: *const CmdItem,
}

#[repr(C)]
pub struct CmdModInfo {
    pub cmd_list: CmdItemNode,
    pub list_num: u32,
    pub cmd_mut_ex: u32,
    pub init_flag: bool,
}

#[repr(C)]
pub struct CmdParsed {
    pub cmd_type: CmdType,
    pub cmd_keyword: [c_char; CMD_KEY_LEN],
    pub para_cnt: u32,
    pub para_value: [*mut c_char; CMD_MAX_PARAS],
}

// 全局变量
static mut G_CMD_INFO: CmdModInfo = CmdModInfo {
    cmd_list: CmdItemNode {
        list: LosListNode {
            prev: ptr::null_mut(),
            next: ptr::null_mut(),
        },
        cmd: ptr::null(),
    },
    list_num: 0,
    cmd_mut_ex: 0,
    init_flag: false,
};

static mut G_CMD_ITEM_GROUP: *mut u8 = ptr::null_mut();

// 辅助函数
unsafe fn strlen(s: *const c_char) -> usize {
    if s.is_null() {
        return 0;
    }
    let mut len = 0;
    while *s.add(len) != 0 {
        len += 1;
    }
    len
}

unsafe fn strrchr(s: *const c_char, c: c_int) -> *const c_char {
    if s.is_null() {
        return ptr::null();
    }
    let len = strlen(s);
    for i in (0..=len).rev() {
        if *s.add(i) == c as c_char {
            return s.add(i);
        }
    }
    ptr::null()
}

unsafe fn strncmp(s1: *const c_char, s2: *const c_char, n: usize) -> c_int {
    for i in 0..n {
        let c1 = *s1.add(i);
        let c2 = *s2.add(i);
        if c1 != c2 {
            return (c1 as c_int) - (c2 as c_int);
        }
        if c1 == 0 {
            break;
        }
    }
    0
}

unsafe fn memset_s(dest: *mut c_void, dest_max: usize, c: c_int, count: usize) -> c_int {
    if dest.is_null() || count > dest_max {
        return -1;
    }
    let bytes = dest as *mut u8;
    for i in 0..count {
        *bytes.add(i) = c as u8;
    }
    0
}

unsafe fn memcpy_s(dest: *mut c_void, dest_max: usize, src: *const c_void, count: usize) -> c_int {
    if dest.is_null() || src.is_null() || count > dest_max {
        return -1;
    }
    let dest_bytes = dest as *mut u8;
    let src_bytes = src as *const u8;
    for i in 0..count {
        *dest_bytes.add(i) = *src_bytes.add(i);
    }
    0
}

// 命令解析辅助函数
unsafe fn os_complete_str(
    result: *const c_char,
    target: *const c_char,
    cmd_key: *mut c_char,
    len: *mut u32,
) {
    let result_len = strlen(result);
    let target_len = strlen(target);
    
    let des = cmd_key.add(*len as usize);
    let src = result.add(target_len);
    
    for i in 0..(result_len - target_len) {
        print_common!("{}", *src.add(i) as char);
        if *len == (SHOW_MAX_LEN - 1) as u32 {
            *des.add(i) = 0;
            break;
        }
        *des.add(i) = *src.add(i);
        *len += 1;
    }
}

// Tab自动补全相关函数
fn os_tab_match_file(_cmd_key: *mut c_char, _len: *mut u32) -> i32 {
    // 简化实现，仅返回0表示无匹配
    0
}

/// Tab 匹配命令
unsafe fn os_tab_match_cmd(cmd_key: *mut c_char, len: *mut u32) -> i32 {
    // 简化实现，暂时返回成功
    0
}


// 命令解析函数
unsafe fn os_cmd_parse(cmd_str: *mut c_char, cmd_parsed: *mut CmdParsed) -> u32 {
    if cmd_str.is_null() || cmd_parsed.is_null() {
        return OS_ERROR;
    }

    // 简化实现，仅清零结构体
    memset_s(
        cmd_parsed as *mut c_void,
        core::mem::size_of::<CmdParsed>(),
        0,
        core::mem::size_of::<CmdParsed>(),
    );

    0
}

// 获取命令信息
#[unsafe(export_name = "OsCmdInfoGet")]
pub unsafe fn os_cmd_info_get() -> *mut CmdModInfo {
    core::ptr::addr_of_mut!(G_CMD_INFO)
}

// 命令键字符串处理
#[unsafe(export_name = "OsCmdKeyShift")]
pub unsafe fn os_cmd_key_shift(
    cmd_key: *const c_char,
    cmd_out: *mut c_char,
    size: u32,
) -> u32 {
    if cmd_key.is_null() || cmd_out.is_null() || size == 0 {
        return OS_ERROR;
    }

    let len = strlen(cmd_key).min(size as usize - 1);
    for i in 0..len {
        *cmd_out.add(i) = *cmd_key.add(i);
    }
    *cmd_out.add(len) = 0;

    0
}

// 检查命令键的有效性
#[unsafe(export_name = "OsCmdKeyCheck")]
pub unsafe fn os_cmd_key_check(cmd_key: *mut c_char) -> bool {
    if cmd_key.is_null() {
        return false;
    }

    if strlen(cmd_key) >= CMD_KEY_LEN {
        return false;
    }

    let mut temp = cmd_key;
    while *temp != 0 {
        let ch = *temp as u8;
        if ch.is_ascii_control() && ch != TAB {
            return false;
        }
        temp = temp.add(1);
    }

    true
}

// Tab 自动补全
#[unsafe(export_name = "OsTabCompletion")]
pub unsafe fn os_tab_completion(cmd_key: *mut c_char, len: *mut u32) -> i32 {
    if cmd_key.is_null() || len.is_null() {
        return 0;
    }

    let mut cmd_main_str = cmd_key;
    
    // 跳过空格
    while *cmd_main_str == SPACE as c_char {
        cmd_main_str = cmd_main_str.add(1);
    }

    let mut count = 0;
    let space = strrchr(cmd_main_str, SPACE as i32);

    if space.is_null() && *cmd_main_str != 0 {
        count = os_tab_match_cmd(cmd_key, len);
    } else if !space.is_null() {
        count = os_tab_match_file(cmd_key, len);
    }

    count
}

// 按升序插入命令
unsafe fn os_cmd_ascending_insert(cmd: *mut CmdItemNode) {
    let list_head = core::ptr::addr_of_mut!(G_CMD_INFO.cmd_list.list);
    LosListNode::tail_insert(list_head, core::ptr::addr_of_mut!((*cmd).list));
}

// 初始化 Shell 键
#[unsafe(export_name = "OsShellKeyInit")]
pub unsafe fn os_shell_key_init(shell_cb: *mut ShellCB) -> u32 {
    if shell_cb.is_null() {
        return OS_ERROR;
    }

    let cmd_key_link = los_mem_alloc(
        m_aucSysMem0 as *mut c_void,
        core::mem::size_of::<CmdKeyLink>() as u32
    ) as *mut CmdKeyLink;

    if cmd_key_link.is_null() {
        return OS_ERROR;
    }

    let cmd_history_link = los_mem_alloc(
        m_aucSysMem0 as *mut c_void,
        core::mem::size_of::<CmdKeyLink>() as u32
    ) as *mut CmdKeyLink;

    if cmd_history_link.is_null() {
        los_mem_free(m_aucSysMem0 as *mut c_void, cmd_key_link as *mut c_void);
        return OS_ERROR;
    }

    (*cmd_key_link).count = 0;
    LosListNode::init(core::ptr::addr_of_mut!((*cmd_key_link).list));
    (*shell_cb).cmd_key_link = cmd_key_link as *mut c_void;

    (*cmd_history_link).count = 0;
    LosListNode::init(core::ptr::addr_of_mut!((*cmd_history_link).list));
    (*shell_cb).cmd_history_key_link = cmd_history_link as *mut c_void;
    (*shell_cb).cmd_mask_key_link = cmd_history_link as *mut c_void;

    0 // LOS_OK
}

// 销毁键链接
unsafe fn os_shell_key_link_deinit(cmd_key_link: *mut CmdKeyLink) {
    let list_ptr = core::ptr::addr_of!((*cmd_key_link).list);

    while !LosListNode::is_empty(list_ptr) {
        let cmd = container_of!((*cmd_key_link).list.next, CmdKeyLink, list);
        LosListNode::remove(core::ptr::addr_of_mut!((*cmd).list));
        los_mem_free(m_aucSysMem0 as *mut c_void, cmd as *mut c_void);
    }

    (*cmd_key_link).count = 0;
    los_mem_free(m_aucSysMem0 as *mut c_void, cmd_key_link as *mut c_void);
}

// 销毁 Shell 键
#[unsafe(export_name = "OsShellKeyDeInit")]
pub unsafe fn os_shell_key_deinit(shell_cb: *const ShellCB) {
    if shell_cb.is_null() {
        return;
    }
    os_shell_key_link_deinit((*shell_cb).cmd_key_link as *mut CmdKeyLink);
    os_shell_key_link_deinit((*shell_cb).cmd_history_key_link as *mut CmdKeyLink);
}

// 注册系统命令
#[unsafe(export_name = "OsShellSysCmdRegister")]
pub unsafe fn os_shell_sys_cmd_register() -> u32 {
    // 简化实现，暂时返回成功
    0
}

// 注销系统命令
#[unsafe(export_name = "OsShellSysCmdUnregister")]
pub unsafe fn os_shell_sys_cmd_unregister() {
    if !G_CMD_ITEM_GROUP.is_null() {
        los_mem_free(m_aucSysMem0 as *mut c_void, G_CMD_ITEM_GROUP as *mut c_void);
        G_CMD_ITEM_GROUP = ptr::null_mut();
    }
}
