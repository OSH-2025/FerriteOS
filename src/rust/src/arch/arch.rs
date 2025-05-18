//! 架构相关函数的Rust实现

use core::arch::asm;
use core::ffi::c_void;

/// 锁定中断并返回之前的状态
#[unsafe(no_mangle)]
pub extern "C" fn ArchIntLock() -> u32 {
    let psr: u32;
    unsafe {
        asm!("mrs {}, cpsr", out(reg) psr);
        // 禁用中断
        asm!("cpsid i");
    }
    psr
}

/// 恢复中断状态
#[unsafe(no_mangle)]
pub extern "C" fn ArchIntRestore(int_save: u32) {
    unsafe {
        // 从int_save恢复CPSR
        asm!("msr cpsr_c, {}", in(reg) int_save);
    }
}

// /// 检查是否在中断上下文
// #[unsafe(no_mangle)]
// pub extern "C" fn IntActive() -> usize {
//     // 获取当前处理器状态，检查是否在中断上下文
//     let psr: u32;
//     unsafe {
//         asm!("mrs {}, cpsr", out(reg) psr);
//     }
//     ((psr >> 8) & 1) as usize  // 假设PSR.EE位表示中断活动
// }

unsafe extern "C" {
    pub fn IntActive() -> usize;
}

/// 获取当前任务控制块
#[unsafe(no_mangle)]
pub extern "C" fn OsCurrTaskGet() -> *mut c_void {
    unsafe { ArchCurrTaskGet() }
}

/// 获取当前任务指针
#[unsafe(no_mangle)]
pub extern "C" fn ArchCurrTaskGet() -> *mut c_void {
    // 使用内联汇编读取ARM系统寄存器
    let task_ptr: usize;
    unsafe {
        // 根据芯片架构可能需要调整具体的汇编指令
        asm!("mrc p15, 0, {}, c13, c0, 3", out(reg) task_ptr);
    }
    task_ptr as *mut c_void
}

/// 获取当前CPU的ID
#[unsafe(no_mangle)]
pub extern "C" fn ArchCurrCpuid() -> u32 {
    #[cfg(feature = "kernel_smp")]
    {
        // 对于SMP系统，需要读取MPIDR寄存器
        let cpuid: u32;
        unsafe {
            // 读取MPIDR寄存器并应用掩码获取CPU ID
            asm!("mrc p15, 0, {}, c0, c0, 5", out(reg) cpuid);
        }
        // 根据ARM架构，CPU ID在MPIDR的最低字节
        cpuid & 0xFF
    }
    
    #[cfg(not(feature = "kernel_smp"))]
    {
        // 单处理器系统固定返回0
        0
    }
}