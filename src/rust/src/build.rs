// 文件: build.rs
fn main() {
    // 编译C代码并链接到Rust库
    cc::Build::new()
        .file("src/fn.c")
        .include("/home/ustc/LiteOS/kernel/include")  // 添加LiteOS头文件路径
        .compile("los_panic");  // 生成liblos_panic.a
        
    // 链接其他可能需要的系统库
    println!("cargo:rustc-link-lib=c");  // 链接libc
    
    // 声明当C文件改变时重新构建
    println!("cargo:rerun-if-changed=src/LOS_Panic.c");
}