unsafe extern "C" {
    #[link_name = "dprintf"]
    pub unsafe fn dprintf(fmt: *const u8, ...);
}

#[macro_export]
macro_rules! os_check_null_return {
    ($param:expr) => {
        if $param.is_null() {
            // crate::utils::printf::dprintf(
            //     b"Null pointer detected at %s:%d\n\0".as_ptr(),
            //     file!().as_ptr(),
            //     line!(),
            // );
            crate::utils::printf::dprintf(b"Null pointer detected at\n\0".as_ptr());
            return;
        }
    };
}
