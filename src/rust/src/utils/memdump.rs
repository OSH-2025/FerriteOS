//! 内存调试和打印函数

use semihosting::{print, println};

/// 打印内存内容(十六进制格式)
///
/// # Arguments
/// * `data` - 内存数据切片
/// * `prefix` - 打印前缀
pub fn dump_hex(data: &[u8]) {
    let addr_base = data.as_ptr() as usize;
    const BYTES_PER_LINE: usize = 16;

    for (i, chunk) in data.chunks(BYTES_PER_LINE).enumerate() {
        // 打印行前缀和首地址
        print!(" {:08x} | ", addr_base + i * BYTES_PER_LINE);
        // 打印十六进制值
        for (j, byte) in chunk.iter().enumerate() {
            print!("{:02x} ", byte);
            if j == 7 {
                // 在中间添加一个额外的空格
                print!(" ");
            }
        }
        // 对齐ASCII部分
        if chunk.len() < BYTES_PER_LINE {
            let padding = (BYTES_PER_LINE - chunk.len()) * 3;
            print!("{:width$}", "", width = padding);
            if chunk.len() <= 8 {
                // 额外的空格补偿
                print!(" ");
            }
        }
        // 打印ASCII表示
        print!("| ");
        for byte in chunk {
            if byte.is_ascii_graphic() || *byte == b' ' {
                print!("{}", *byte as char);
            } else {
                print!(".");
            }
        }
        println!();
    }
}

/// 打印内存区域详细信息
pub fn dump_region(addr: usize, size: usize) {
    if size > 0 {
        println!(
            "Memory region: 0x{:x} - 0x{:x} (size: {} bytes)",
            addr,
            addr + size,
            size
        );
        unsafe {
            let data = core::slice::from_raw_parts(addr as *const u8, size);
            dump_hex(data);
        }
    }
}

/// 比较两个内存区域
#[allow(dead_code)]
pub fn compare_regions(addr1: usize, addr2: usize, size: usize) -> bool {
    if size == 0 {
        return true;
    }

    unsafe {
        let region1 = core::slice::from_raw_parts(addr1 as *const u8, size);
        let region2 = core::slice::from_raw_parts(addr2 as *const u8, size);

        if region1 != region2 {
            println!("Memory regions differ:");
            println!("Region 1:");
            dump_hex(region1);
            println!("Region 2:");
            dump_hex(region2);
            false
        } else {
            println!("Memory regions are identical");
            true
        }
    }
}

/// 检查内存区域是否包含特定模式
#[allow(dead_code)]
pub fn find_pattern(addr: usize, size: usize, pattern: &[u8]) -> Option<usize> {
    if size == 0 || pattern.is_empty() {
        return None;
    }
    unsafe {
        let data = core::slice::from_raw_parts(addr as *const u8, size);

        for i in 0..=(data.len() - pattern.len()) {
            if data[i..(i + pattern.len())] == pattern[..] {
                return Some(addr + i);
            }
        }
    }
    None
}
