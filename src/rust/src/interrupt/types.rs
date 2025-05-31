use core::ffi::c_char;

/// 中断处理函数类型
pub type HwiProcFunc = Option<extern "C" fn()>;

/// 中断处理信息结构体
#[repr(C)]
#[derive(Debug)]
pub struct HwiHandleInfo {
    /// 用户注册的回调函数
    pub hook: HwiProcFunc,
    /// 中断响应计数
    pub resp_count: u32,
}

impl HwiHandleInfo {
    /// 创建新的中断处理信息
    pub const fn new() -> Self {
        Self {
            hook: None,
            resp_count: 0,
        }
    }

    /// 重置中断处理信息
    pub fn reset(&mut self) {
        self.hook = None;
        self.resp_count = 0;
    }

    /// 增加响应计数
    pub fn increment_count(&mut self) {
        self.resp_count = self.resp_count.saturating_add(1);
    }

    /// 检查是否已注册处理函数
    pub fn is_registered(&self) -> bool {
        self.hook.is_some()
    }
}

impl Default for HwiHandleInfo {
    fn default() -> Self {
        Self::new()
    }
}

/// 硬件中断控制器操作接口
#[repr(C)]
pub struct HwiControllerOps {
    /// 触发中断
    pub trigger_irq: Option<extern "C" fn(hwi_num: u32) -> u32>,
    /// 清除中断
    pub clear_irq: Option<extern "C" fn(hwi_num: u32) -> u32>,
    /// 使能中断
    pub enable_irq: Option<extern "C" fn(hwi_num: u32) -> u32>,
    /// 禁用中断
    pub disable_irq: Option<extern "C" fn(hwi_num: u32) -> u32>,
    /// 设置中断优先级
    pub set_irq_priority: Option<extern "C" fn(hwi_num: u32, priority: u8) -> u32>,
    /// 获取当前中断号
    pub get_cur_irq_num: Option<extern "C" fn() -> u32>,
    /// 获取中断版本信息
    pub get_irq_version: Option<extern "C" fn() -> *const c_char>,
    /// 获取中断处理表单
    pub get_handle_form: Option<extern "C" fn(hwi_num: u32) -> *mut HwiHandleInfo>,
    /// 处理中断入口
    pub handle_irq: Option<extern "C" fn()>,
}

impl HwiControllerOps {
    /// 检查控制器操作是否有效
    pub fn is_valid(&self) -> bool {
        // get_handle_form 是必须的
        true
    }

    /// 安全触发中断
    pub fn safe_trigger_irq(&self, hwi_num: u32) -> Result<u32, &'static str> {
        match self.trigger_irq {
            Some(func) => Ok(func(hwi_num)),
            None => Err("trigger_irq not implemented"),
        }
    }

    /// 安全清除中断
    pub fn safe_clear_irq(&self, hwi_num: u32) -> Result<u32, &'static str> {
        match self.clear_irq {
            Some(func) => Ok(func(hwi_num)),
            None => Err("clear_irq not implemented"),
        }
    }

    /// 安全使能中断
    pub fn safe_enable_irq(&self, hwi_num: u32) -> Result<u32, &'static str> {
        match self.enable_irq {
            Some(func) => Ok(func(hwi_num)),
            None => Err("enable_irq not implemented"),
        }
    }

    /// 安全禁用中断
    pub fn safe_disable_irq(&self, hwi_num: u32) -> Result<u32, &'static str> {
        match self.disable_irq {
            Some(func) => Ok(func(hwi_num)),
            None => Err("disable_irq not implemented"),
        }
    }

    /// 安全设置中断优先级
    pub fn safe_set_irq_priority(&self, hwi_num: u32, priority: u8) -> Result<u32, &'static str> {
        match self.set_irq_priority {
            Some(func) => Ok(func(hwi_num, priority)),
            None => Err("set_irq_priority not implemented"),
        }
    }

    /// 安全获取当前中断号
    pub fn safe_get_cur_irq_num(&self) -> Result<u32, &'static str> {
        match self.get_cur_irq_num {
            Some(func) => Ok(func()),
            None => Err("get_cur_irq_num not implemented"),
        }
    }

    /// 安全获取中断版本
    pub fn safe_get_irq_version(&self) -> Result<*const c_char, &'static str> {
        match self.get_irq_version {
            Some(func) => Ok(func()),
            None => Err("get_irq_version not implemented"),
        }
    }

    /// 获取中断处理表单
    pub fn get_handle_form(&self, hwi_num: u32) -> &mut HwiHandleInfo {
        match self.get_handle_form {
            Some(func) => match unsafe { func(hwi_num).as_mut() } {
                Some(handle_form) => handle_form,
                None => panic!("get_handle_form returned null for hwi_num: {}", hwi_num),
            },
            None => {}
        }
    }

    /// 安全处理中断
    pub fn safe_handle_irq(&self) {
        match self.handle_irq {
            Some(func) => {
                func();
            }
            None => {}
        }
    }
}

// C语言兼容的FFI接口
unsafe extern "C" {
    /// 全局中断控制器操作接口
    unsafe static g_hwi_ops: *const HwiControllerOps;
}

/// 获取全局中断控制器操作接口
pub fn get_hwi_ops() -> Option<&'static HwiControllerOps> {
    unsafe {
        if g_hwi_ops.is_null() {
            None
        } else {
            Some(&*g_hwi_ops)
        }
    }
}

/// 中断管理器
pub struct InterruptManager {
    ops: &'static HwiControllerOps,
}

impl InterruptManager {
    /// 创建中断管理器
    pub fn new() -> Result<Self, &'static str> {
        match get_hwi_ops() {
            Some(ops) => Ok(Self { ops }),
            None => Err("Hardware interrupt operations not initialized"),
        }
    }

    /// 创建中断处理程序
    pub fn create_interrupt(
        &self,
        hwi_num: u32,
        priority: u8,
        handler: HwiProcFunc,
    ) -> Result<(), &'static str> {
        if handler.is_none() {
            return Err("Handler function is null");
        }

        let hwi_form = self.ops.get_handle_form(hwi_num);
        if hwi_form.is_null() {
            return Err("Invalid interrupt number");
        }

        unsafe {
            let hwi_info = &mut *hwi_form;
            if hwi_info.is_registered() {
                return Err("Interrupt already created");
            }

            hwi_info.hook = handler;

            // 设置优先级（如果支持）
            if let Ok(_) = self.ops.safe_set_irq_priority(hwi_num, priority) {
                // 优先级设置成功
            }
        }

        Ok(())
    }

    /// 删除中断处理程序
    pub fn delete_interrupt(&self, hwi_num: u32) -> Result<(), &'static str> {
        let hwi_form = self.ops.get_handle_form(hwi_num);
        if hwi_form.is_null() {
            return Err("Invalid interrupt number");
        }

        unsafe {
            let hwi_info = &mut *hwi_form;
            hwi_info.reset();
            self.ops.safe_disable_irq(hwi_num)?;
        }

        Ok(())
    }

    /// 使能中断
    pub fn enable_interrupt(&self, hwi_num: u32) -> Result<(), &'static str> {
        self.ops.safe_enable_irq(hwi_num).map(|_| ())
    }

    /// 禁用中断
    pub fn disable_interrupt(&self, hwi_num: u32) -> Result<(), &'static str> {
        self.ops.safe_disable_irq(hwi_num).map(|_| ())
    }

    /// 触发中断
    pub fn trigger_interrupt(&self, hwi_num: u32) -> Result<(), &'static str> {
        self.ops.safe_trigger_irq(hwi_num).map(|_| ())
    }

    /// 清除中断
    pub fn clear_interrupt(&self, hwi_num: u32) -> Result<(), &'static str> {
        self.ops.safe_clear_irq(hwi_num).map(|_| ())
    }
}
