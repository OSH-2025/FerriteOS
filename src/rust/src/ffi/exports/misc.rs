use crate::{
    task::manager::delay::task_delay,
    utils::{
        align::{align_down, align_up},
        printf::dprintf,
    },
};

#[unsafe(export_name = "LOS_Align")]
pub extern "C" fn los_align(addr: u32, boundary: u32) -> u32 {
    return align_up(addr, boundary);
}

//TODO remove this
unsafe extern "C" {
    fn LOS_MS2Tick(millisec: u32) -> u32;
}

/// 毫秒级休眠
///
/// # 参数
///
/// * `msecs` - 休眠毫秒数
#[unsafe(export_name = "LOS_Msleep")]
pub extern "C" fn los_msleep(msecs: u32) {
    let mut interval;

    if msecs == 0 {
        interval = 0; // 值为0表示直接调度
    } else {
        unsafe {
            interval = LOS_MS2Tick(msecs);
        }
        // 添加一个tick补偿不准确的tick计数
        if interval < u32::MAX {
            interval += 1; // 使用+=赋值，而不是返回表达式结果
        }
        // 不需要else块，因为interval已经有值
    }

    // 使用外部声明的LOS_TaskDelay函数
    let _ = task_delay(interval);
}

#[unsafe(no_mangle)]
pub extern "C" fn OsDumpMemByte(length: usize, addr: usize) {
    const SIZE_OF_UINTPTR: usize = core::mem::size_of::<usize>();
    const SIZE_OF_CHAR_PTR: usize = core::mem::size_of::<*const u8>();

    let data_len = align_up(length as u32, SIZE_OF_UINTPTR as u32) as usize; // ALIGN宏
    let align_addr = align_down(addr as u32, SIZE_OF_UINTPTR as u32) as usize; // ALIGN_DOWN宏

    if data_len == 0 || align_addr == 0 {
        return;
    }

    let mut count = 0;
    let mut current_addr = align_addr;
    let mut remaining = data_len;

    while remaining > 0 {
        // 使用IS_ALIGNED宏：((value) & ((alignSize) - 1)) == 0
        if ((count as usize) & (SIZE_OF_CHAR_PTR - 1)) == 0 {
            unsafe {
                dprintf(b"\n 0x%lx :\0".as_ptr(), current_addr as *mut usize);

                #[cfg(feature = "shell_excinfo_dump")]
                WriteExcInfoToBuf(b"\n 0x%lx :\0".as_ptr(), current_addr as *mut usize);
            }
        }

        unsafe {
            #[cfg(target_pointer_width = "64")]
            PrintkWrapper(b"%0+16lx \0".as_ptr(), *(current_addr as *const usize));

            #[cfg(target_pointer_width = "32")]
            dprintf(b"%0+8lx \0".as_ptr(), *(current_addr as *const usize));

            #[cfg(all(feature = "shell_excinfo_dump", target_pointer_width = "64"))]
            WriteExcInfoToBuf(b"0x%0+16x \0".as_ptr(), *(current_addr as *const usize));

            #[cfg(all(feature = "shell_excinfo_dump", target_pointer_width = "32"))]
            WriteExcInfoToBuf(b"0x%0+8x \0".as_ptr(), *(current_addr as *const usize));
        }

        current_addr += SIZE_OF_UINTPTR;
        remaining -= SIZE_OF_UINTPTR;
        count += 1;
    }

    unsafe {
        dprintf(b"\n\0".as_ptr());

        #[cfg(feature = "shell_excinfo_dump")]
        WriteExcInfoToBuf(b"\n\0".as_ptr());
    }
}
