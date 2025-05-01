unsafe extern "C" {
    #[link_name = "dprintf"]
    pub unsafe fn dprintf(fmt: *const u8, ...);
}
