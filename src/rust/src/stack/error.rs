/// 栈水位线检测结果
#[derive(Debug, Clone, Copy, PartialEq, Eq)]
pub enum StackError {
    /// 栈魔数无效
    Corrupted,
}

impl From<StackError> for u32 {
    fn from(error: StackError) -> Self {
        match error {
            StackError::Corrupted => u32::MAX,
        }
    }
}

impl TryFrom<u32> for StackError {
    type Error = ();

    fn try_from(errno: u32) -> Result<Self, Self::Error> {
        if errno == u32::MAX {
            Ok(StackError::Corrupted)
        } else {
            Err(())
        }
    }
}

impl core::fmt::Display for StackError {
    fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
        match self {
            StackError::Corrupted => write!(f, "Stack corrupted"),
        }
    }
}
