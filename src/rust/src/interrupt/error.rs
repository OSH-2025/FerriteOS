/// 中断管理错误
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum InterruptError {
    /// 中断处理函数为空
    ProcFuncNull,
    /// 中断已经被创建/注册
    AlreadyCreated,
    /// 无效的中断号
    NumInvalid,
}

impl From<InterruptError> for u32 {
    fn from(error: InterruptError) -> Self {
        match error {
            InterruptError::ProcFuncNull => ERRNO_HWI_PROC_FUNC_NULL,
            InterruptError::AlreadyCreated => ERRNO_HWI_ALREADY_CREATED,
            InterruptError::NumInvalid => ERRNO_HWI_NUM_INVALID,
        }
    }
}

const ERRNO_HWI_NUM_INVALID: u32 = 0x02000900;
const ERRNO_HWI_PROC_FUNC_NULL: u32 = 0x02000901;
const ERRNO_HWI_ALREADY_CREATED: u32 = 0x02000904;

/// 从u32错误码转换为InterruptError
impl TryFrom<u32> for InterruptError {
    type Error = ();

    fn try_from(errno: u32) -> Result<Self, Self::Error> {
        match errno {
            ERRNO_HWI_PROC_FUNC_NULL => Ok(InterruptError::ProcFuncNull),
            ERRNO_HWI_ALREADY_CREATED => Ok(InterruptError::AlreadyCreated),
            ERRNO_HWI_NUM_INVALID => Ok(InterruptError::NumInvalid),
            _ => Err(()),
        }
    }
}
