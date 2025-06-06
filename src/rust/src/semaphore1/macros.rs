// 这个文件包含可能需要的宏定义
// 注意：大多数宏已经在crate根目录中定义，如container_of
// 如果需要特定的宏，可以在这里定义

/// 调试打印宏
#[macro_export]
macro_rules! sem_debug_print {
    ($($arg:tt)*) => {
        #[cfg(feature = "debug-semaphore")]
        {
            $crate::println_debug!($($arg)*);
        }
    };
}