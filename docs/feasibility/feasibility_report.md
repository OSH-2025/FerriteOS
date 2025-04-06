- [可行性报告](#可行性报告)
  - [摘要](#摘要)
  - [理论依据](#理论依据)
    - [LiteOS](#liteos)
      - [代码框架介绍](#代码框架介绍)
      - [可改写模块分析](#可改写模块分析)
        - [组件层](#组件层)
        - [用户接口层](#用户接口层)
        - [驱动层](#驱动层)
        - [内核层](#内核层)
      - [内核层介绍](#内核层介绍)
        - [内核架构](#内核架构)
        - [各模块简介](#各模块简介)
          - [内存管理](#内存管理)
          - [任务管理](#任务管理)
          - [硬件相关](#硬件相关)
          - [IPC通信](#ipc通信)
          - [软件定时器](#软件定时器)
          - [自旋锁](#自旋锁)
          - [低功耗](#低功耗)
          - [维测](#维测)
          - [C++支持](#c支持)
        - [可改写模块分析](#可改写模块分析-1)
          - [内存管理模块](#内存管理模块)
          - [任务调度模块](#任务调度模块)
          - [同步原语模块](#同步原语模块)
          - [中断管理模块](#中断管理模块)
          - [IPC通信模块](#ipc通信模块)
    - [Rust](#rust)
      - [Unsafe代码的边界管理](#unsafe代码的边界管理)
      - [Rust类型系统如何静态保证并发安全](#rust类型系统如何静态保证并发安全)
      - [Rust抽象与C语言宏和函数指针的性能等效性](#rust抽象与c语言宏和函数指针的性能等效性)
      - [Result/Option类型对内核错误处理的改进](#resultoption类型对内核错误处理的改进)
      - [Rust模块化特性与LiteOS代码组织的适配性](#rust模块化特性与liteos代码组织的适配性)
      - [用Rust重写中断处理例程](#用rust重写中断处理例程)
      - [Rust的裸机支持能力](#rust的裸机支持能力)
      - [总结](#总结)
  - [技术依据](#技术依据)
    - [LiteOS编译](#liteos编译)
      - [搭建环境](#搭建环境)
      - [编译](#编译)
    - [qemu运行LiteOS](#qemu运行liteos)
    - [C与Rust交互](#c与rust交互)
      - [实现机制](#实现机制)
        - [函数调用](#函数调用)
        - [数据类型映射](#数据类型映射)
        - [指针管理](#指针管理)
        - [构建与链接](#构建与链接)
      - [实现示例](#实现示例)
        - [C调用Rust](#c调用rust)
        - [Rust调用C](#rust调用c)
      - [工具支持](#工具支持)
        - [bindgen](#bindgen)
          - [安装](#安装)
          - [使用](#使用)
        - [cbindgen](#cbindgen)
          - [安装](#安装-1)
          - [使用要求](#使用要求)
          - [基本使用](#基本使用)
          - [集成到构建脚本](#集成到构建脚本)
  - [创新点](#创新点)
  - [参考文献](#参考文献)

# 可行性报告

## 摘要

随着物联网设备对系统安全性与可靠性的需求日益迫切，传统基于C语言开发的操作系统面临内存管理漏洞、数据竞争等严峻挑战，FerriteOS小组计划使用**Rust重构LiteOS内核模块**。本文聚焦于项目的可行性分析，首先介绍了包括LiteOS代码树结构、不同模块改写的优点和挑战，阐述了Rust语言在重写LiteOS内核方面的理论依据和可行性；接着分析了LiteOS编译、qemu运行LiteOS、C与Rust交互等技术方法；最后简要概述了该项目的创新点。

## 理论依据

### LiteOS

#### 代码框架介绍

LiteOS的代码树以及各个目录存放的源代码的相关内容简介如下：

```
.
├── arch        		# 各种架构支持
├── build        		# LiteOS编译系统需要的配置及脚本
├── copa        		# liteos提供的CMSIS-RTOS 1.0和2.0接口
├── components
│   ├── ai        		# ai(基于mindspore算子库实现)
│   ├── bootloader
│   ├── connectivity	# 一些协议实现
│   ├── fs        		# 各种文件系统
│   ├── gui        		# 开源LittlevGL图形库
│   ├── language        # 语言相关组件，含lua
│   ├── lib
│   │   └── cjson		# c语言json库
│   ├── media			# 媒体相关组件
│   ├── net				# 网络相关实现
│   ├── ota				# 固件升级代码
│   ├── security        # 一些安全协议
│   ├── sensorhub       # 传感器相关实现
│   └── utility         # 许多解析工具
├── demos
│   ├── ...				# 此处省略各种demo
│   ├── kernel
│   │   ├── api			# 供开发者测试LiteOS内核的demo示例代码
│   │   └── include		# API功能头文件存放目录
│   ├── ...				# 此处省略各种demo
│   └── utility
├── doc        			# 此目录存放的是LiteOS的使用文档和API说明等文档
├── drivers        		# 驱动框架与中断、定时器、串口接口
├── include        		# components各个模块所依赖的头文件
├── kernel
│   ├── base        	# LiteOS基础内核代码，包括任务、中断、软件定时器、队列、事件、信号量、互斥锁、tick等功能
│   │   ├── debug		# LiteOS内核调测代码，包括队列、信号量、互斥锁及任务调度的调测
│   │   ├── include		# LiteOS基础内核内部使用的头文件
│   │   ├── mem			# LiteOS中的内存管理相关代码
│   │   ├── sched		# 任务调度支持，包括对多核的调度支持
│   │   └── shellcmd	# LiteOS中与基础内核相关的shell命令，包括memcheck、task、systeminfo、swtmr等
│   ├── ...				# 此处省略任务、中断、软件定时器、队列、事件、信号量、互斥锁、tick 等相关代码。
│   ├── extended        # LiteOS拓展模块代码
│   ├── include			# LiteOS开源内核头文件
│   └── init			# LiteOS内核初始化相关代码
├── lib					# 一些库，包括LiteOS自研libc库、musl libc库、华为安全函数库、开源zlib库等
├── osdepends			# LiteOS提供的部分OS适配接口
├── shell				# 实现shell命令的代码和头文件，支持基本调试功能
├── targets				# 各种板子的开发工程源码包
├── test
├── tests
├── tools
│   ├── build			# LiteOS支持的开发板编译配置文件
│   └── menuconfigs		# LiteOS编译所需的menuconfig脚本
├── Makefile
└── .config				# 开发板的配置文件
```

#### 可改写模块分析

LiteOS可改写的模块可分为 **内核层（kernel）**、**组件层（components）**、**驱动层（drivers）** 和 **用户接口层（shell）**。  基于代码量、技术可行性、安全价值与团队能力，对各模块重构可行性的分析如下：

##### 组件层

组件层（components）包含文件系统（fs）、网络协议栈（net）、GUI 等模块，总代码量191211行，远超改写目标。

- 优势：部分模块（如文件系统）功能边界清晰，可独立改写。

- 劣势：
  
  - 模块间依赖复杂（如网络协议栈依赖内核线程机制），需连带改写大量基础设施。
  
  - 功能性代码（如 GUI）对安全性提升意义有限。

##### 用户接口层

用户接口层（shell）有4289行代码，包含命令解析、交互逻辑。

- 优势：
  
  - 代码独立性强，可完整替换为 Rust 实现。
  
  - 适合验证 Rust 字符串处理与模式匹配能力。

- 劣势：
  
  - 作为用户输入前端，不涉及核心内存/线程安全。
  
  - 对系统可靠性提升贡献有限。

##### 驱动层

驱动层（drivers）总代码量有4448行，硬件相关驱动代码分散，单设备驱动约500~2000行。

- 优势：Rust 的 `unsafe` 边界控制可提升硬件操作安全性。

- 劣势：
  
  - 需深度绑定芯片厂商 SDK（如 HiSilicon），改写依赖硬件验证环境。
  
  - 代码碎片化，难以集中改写。

##### 内核层

内核层（kernel）的核心功能代码19736行，可以进一步细分为若干子模块。

- 优势：
  
  - 安全关键性高：任务调度、内存管理、同步原语（锁/信号量）的漏洞直接影响系统稳定性。
  
  - 模块化设计：子模块（如 `sched`、`mem`）耦合度低，支持分步改写。
  
  - Rust 特性契合：所有权模型可消除内存错误，类型系统可强化并发安全。

- 劣势：
  
  - 需处理 C 头文件与 Rust 的 FFI（Foreign Function Interface），工具链复杂度增加。
  
  - 部分算法（如调度策略）需保证实时性，Rust 运行时可能引入不确定性。

经过上述分析，考虑到代码量、可行性、安全价值、重要性等因素，我们组决定对内核层进行 rust 改写。

#### 内核层介绍

##### 内核架构

Huawei LiteOS基础内核包括不可裁剪的极小内核和可裁剪的其他模块。

- 极小内核：包含任务管理、内存管理、中断管理、异常管理和系统时钟。

- 可裁剪的模块：包括信号量、互斥锁、队列管理、事件管理、软件定时器等。 

Huawei LiteOS支持 UP（单核）与 SMP（多核）模式，即支持在单核或者多核的环境上运行。

具体框架如图所示：

![Huawei LiteOS Kernel Architecture](../../assets/liteos_kernel_architecture.png)

##### 各模块简介

###### 内存管理

- 提供静态内存和动态内存两种算法，支持内存申请、释放。目前支持的内存管理算法有固定大小的BOX算法、动态申请的bestfit算法和bestfit_little算法。
- 提供内存统计、内存越界检测功能。

###### 任务管理

提供任务的创建、删除、延迟、挂起、恢复等功能，以及锁定和解锁任务调度。支持任务按优先级高低的抢占调度以及同优先级时间片轮转调度。

###### 硬件相关

提供中断管理、异常管理、系统时钟等功能。

- 中断管理：提供中断的创建、删除、使能、禁止、请求位的清除功能。
- 异常管理：系统运行过程中发生异常后，跳转到异常处理模块，打印当前发生异常的函数调用栈信息，或者保存当前系统状态。
- Tick：Tick是操作系统调度的基本时间单位，对应的时长由每秒Tick数决定，由用户配置。

###### IPC通信

提供消息队列、事件、信号量和互斥锁功能。

- 消息队列：支持消息队列的创建、删除、发送和接收功能。

- 事件：支持读事件和写事件功能。

- 信号量：支持信号量的创建、删除、申请和释放功能。

- 互斥锁：支持互斥锁的创建、删除、申请和释放功能。

###### 软件定时器

软件定时器提供了定时器的创建、删除、启动、停止功能。

###### 自旋锁

多核场景下，支持自旋锁的初始化、申请、释放功能。

###### 低功耗

- Run-stop：即休眠唤醒，是Huawei LiteOS提供的保存系统现场镜像以及从系统现场镜像中恢复运行的机制。
- Tickless：Tickless机制通过计算下一次有意义的时钟中断的时间，来减少不必要的时钟中断，从而降低系统功耗。打开Tickless功能后，系统会在CPU空闲时启动Tickless机制。

###### 维测

- CPU占用率：可以获取系统或者指定任务的CPU占用率。
- Trace事件跟踪：实时获取事件发生的上下文，并写入缓冲区。支持自定义缓冲区，跟踪指定模块的事件，开启/停止Trace，清除/输出trace缓冲区数据等。
- LMS：实时检测内存操作合法性，LMS能够

- 检测的内存问题包括缓冲区溢出（buffer overflow），释放后使用（use after free），多重释放（double free）和释放野指针（wild pointer）。
- Shell：Huawei LiteOS Shell使用串口接收用户输入的命令，通过命令的方式调用、执行相应的应用程序。Huawei LiteOS Shell支持常用的基本调试功能，同时支持用户添加自定义命令。

###### C++支持

Huawei LiteOS支持部分STL特性、异常和RTTI特性，其他特性由编译器支持。

##### 可改写模块分析

LiteOS kernel的基础内核部分是其核心所在，而内核增强部分并非操作系统的必备部分，对其进行改写的意义较为有限。真正影响操作系统安全性的关键在于基础内核，因此我们将改写的重点放在基础内核上。

###### 内存管理模块

内存管理模块有3644行代码，主要实现动态内存分配（`los_malloc`/`los_free`）、内存池管理、碎片整理等功能。

- 安全价值：
  
  - C 语言手动管理内存易导致泄漏、越界访问等问题，Rust 的所有权模型可从根本上解决。
  
  - 内存管理是内核稳定性的基石，漏洞可能导致系统崩溃或安全攻击（如堆溢出）。

- 技术挑战：
  
  - LiteOS 使用 两级内存池（静态预分配 + 动态扩展），需将 C 的 `void*` 指针转换为 Rust 的安全抽象（如 `Box<T>` 或自定义分配器）。
  
  - 碎片整理算法（如 First-Fit/Best-Fit）需保证 Rust 的借用检查不破坏原有逻辑。

###### 任务调度模块

任务调度模块代码量为462行，主要功能为任务优先级队列、时间片轮转调度、上下文切换（`los_task_switch`）。

- 安全价值：
  
  - 避免优先级反转（Priority Inversion）和死锁。
  
  - Rust 的类型系统可强制隔离不同优先级任务的资源访问。

- 技术挑战：
  
  - 上下文切换依赖汇编代码（如 ARM Cortex-M 的 `svc` 指令），需用 Rust 的 `asm!` 宏重写。
  
  - 实时性要求高，需确保 Rust 无运行时开销（禁用标准库，使用 `#![no_std]`）。

###### 同步原语模块

同步原语模块隐含在 `kernel/base` 下的`los_sem.c` 文件中，有218行代码，实现了互斥锁（`LOS_MuxLock`）、信号量（`LOS_SemPend`/`LOS_SemPost`）、优先级继承协议等功能。

- 安全价值：
  
  - 同步原语的竞态条件是内核崩溃的主要诱因，Rust 的 `Mutex<T>` 和 `Atomic` 类型可消除数据竞争。
  
  - 锁的优先级继承逻辑可通过 Rust 类型系统强制约束（如 `MutexGuard` 生命周期绑定任务优先级）。

- 技术挑战：
  
  - LiteOS 的锁实现依赖自旋等待（Spinlock），需与 Rust 的 `futex` 或 `park/unpark` 机制兼容。
  
  - 信号量的 `P/V` 操作需保证原子性（Rust 的 `AtomicUsize` 可替代 C 的 `volatile`）。

###### 中断管理模块

中断管理模块隐含在 `kernel/base` 下的中断处理代码中，总代码量约600 行，实现的功能有中断控制器配置（如 ARM GIC）、中断服务例程（ISR）注册。

- 安全价值：
  - 中断上下文的非托管操作易导致内存不安全，Rust 的 `unsafe` 边界可显式标记风险。
  
  - 硬件寄存器访问需严格隔离。
  
- 技术挑战：
  
  - 中断处理函数需满足 `extern "C"` ABI，且不能直接传递 Rust 闭包。
  
  - 硬件寄存器操作（如 `*(volatile u32*)0xFFFF0000 = 1`）需用 `Volatile` 类型封装。

###### IPC通信模块

IPC 通信模块在 `kernel/base`下的`los_queue.c`、`los_event.c`文件中，前者549行代码，后者245行代码。该模块实现的核心功能为消息队列（`LOS_QueueCreate`/`LOS_QueueSend`）和事件组（`LOS_EventRead`/`LOS_EventWrite`）。

- 安全价值：
  
  - 跨任务通信易出现数据竞争，Rust 的通道（`Channel`）和 `Send`/`Sync` trait 可保证类型安全。
  
  - 事件组的位操作需原子性保证，避免信号丢失。

- 技术挑战：
  
  - C 的 `void*` 消息需转换为泛型 `T: Send + Sync`。
  
  - 需兼容 LiteOS 的异步事件通知机制。

### Rust

在操作系统内核开发中，Rust 凭借其独特的设计哲学和语言特性，为传统系统编程的痛点提供了革新性解决方案。从内存安全的底层保障到零成本抽象的性能承诺，Rust 不仅延续了 C 语言对硬件的直接控制能力，更通过类型系统和现代编程范式，显著提升了代码的可靠性、可维护性及并发安全性。本节将从语言设计理论出发，系统性论证 Rust 在 LiteOS 内核模块改造中的技术适配性，为项目可行性奠定理论基础。  

#### Unsafe代码的边界管理

在嵌入式系统中，如 LiteOS 内核，直接操作硬件是常见需求，例如配置寄存器或处理内存映射外设。Rust 的 `unsafe` 机制允许在必要时绕过安全检查，但要求开发者明确划定边界以限制潜在风险。

- 详细阐述

Rust 的 `unsafe` 块明确标记了可能违反内存安全或类型安全的代码段，便于审查和隔离。例如，在 LiteOS 中操作硬件外设时，可能需要直接读写特定内存地址。Rust 通过指针解引用和裸指针操作支持此类场景，同时要求开发者手动确保边界条件（如地址对齐、访问权限）。相比 C 的无约束指针操作，Rust 的 `unsafe` 提供了一种“受控的危险区域”，减少了意外错误传播的可能性。

- 代码示例

假设 LiteOS 需要配置一个 GPIO 寄存器：

```rust
use core::ptr;

const GPIO_BASE: usize = 0x4000_0000;	// 假设的 GPIO 基地址
const GPIO_DIR_OFFSET: usize = 0x04;	// 方向寄存器偏移

fn configure_gpio(pin: u32, output: bool) {
    let dir_reg = (GPIO_BASE + GPIO_DIR_OFFSET) as *mut u32;
    unsafe {
        // 直接操作硬件寄存器
        let mut value = ptr::read_volatile(dir_reg);
        if output {
            value |= 1 << pin;  	// 设置为输出
        } else {
            value &= !(1 << pin);	// 设置为输入
        }
        ptr::write_volatile(dir_reg, value);
    }
}
```

- `unsafe` 块限制了对裸指针的使用，仅在必要时解引用。
- 使用 `read_volatile` 和 `write_volatile` 确保编译器不会优化掉硬件访问。
- 相比 C 的直接指针操作，Rust 的边界更清晰，开发者必须显式声明 `unsafe`，便于审计。

#### Rust类型系统如何静态保证并发安全

Rust 的类型系统通过所有权和借用规则在编译期消除数据竞争，特别在并发场景中表现优异。`Arc<Mutex<T>>` 是典型设计，用于多线程共享可变数据。

- 详细阐述

在 LiteOS 中，任务间可能需要共享资源（如缓冲区）。Rust 的 `Arc`（原子引用计数）允许多线程安全共享数据，而 `Mutex` 提供互斥锁机制，确保同一时刻只有一个线程访问数据。类型系统强制要求锁在使用前被获取，并在作用域结束时自动释放，避免了 C 中常见的锁遗漏或死锁问题。

- 代码示例

共享计数器示例：

```rust
use std::sync::{Arc, Mutex};
use std::thread;

fn main() {
    let counter = Arc::new(Mutex::new(0));
    let mut handles = vec![];

    for _ in 0..4 {
        let counter = Arc::clone(&counter);
        let handle = thread::spawn(move || {
            let mut num = counter.lock().unwrap();
            *num += 1;
        });
        handles.push(handle);
    }

    for handle in handles {
        handle.join().unwrap();
    }
    println!("Result: {}", *counter.lock().unwrap());
}
```

- `Arc` 确保线程安全的引用计数，`Mutex` 保证数据访问的互斥性。
- 编译器静态检查锁的使用，消除了未加锁访问的风险。
- 相比 C 的 pthread 锁，Rust 的设计更简洁且无运行时开销。

#### Rust抽象与C语言宏和函数指针的性能等效性

Rust 的 trait 和泛型提供了强大的抽象能力，但在嵌入式实时系统中，性能至关重要。需要理论论证其与 C 的宏和函数指针在性能上的等效性。

- 详细阐述

Rust 的 trait 通过单态化（monomorphization）在编译期展开泛型代码，生成与 C 宏类似的静态分派代码，避免运行时开销。相比 C 的函数指针（动态分派），Rust 的 trait 对象（带虚表）虽有少量开销，但在明确场景下可通过静态分派优化。嵌入式实时性要求低延迟和高确定性，Rust 的零成本抽象恰好满足这一需求。

- 代码示例

用 trait 实现设备驱动接口：

```rust
trait Device {
    fn read(&self) -> u32;
    fn write(&self, value: u32);
}

struct Sensor;
impl Device for Sensor {
    fn read(&self) -> u32 { 42 } { 
        /* 模拟读取 */
    }
    fn write(&self, value: u32) { 
        /* 模拟写入 */
    }
}

fn operate<T: Device>(device: T) {
    let data = device.read();
    device.write(data + 1);
}

fn main() {
    let sensor = Sensor;
    operate(sensor);  // 编译期单态化，无运行时开销
}
```

- `operate` 函数的泛型在编译时展开为具体实现，与 C 宏等效。
- 相比 C 函数指针的间接调用，Rust 的静态分派无额外开销，满足实时性要求。

#### Result/Option类型对内核错误处理的改进

C 语言通常使用返回码或全局  `errno`  处理错误，而 Rust 的 `Result` 和 `Option` 类型提供了更类型安全的替代方案。

- 详细阐述

`Result` 强制开发者处理成功和失败两种情况，避免遗漏错误检查。`Option` 用于表示可能为空的值，消除了 C 中常见的空指针问题。在 LiteOS 内核中，错误处理直接影响系统稳定性，Rust 的模式匹配和 `?` 操作符简化了错误传播逻辑。

- 代码示例

检查设备初始化：

```rust
enum DeviceError {
    NotFound,
    InitFailed,
}

fn init_device(id: u32) -> Result<(), DeviceError> {
    if id == 0 {
        Err(DeviceError::NotFound)
    } else {
        Ok(())
    }
}

fn setup() -> Result<(), DeviceError> {
    init_device(1)?;
    Ok(())
}

fn main() {
    match setup() {
        Ok(()) => println!("Setup complete"),
        Err(e) => println!("Error: {:?}", e),
    }
}
```

- `Result` 强制处理错误，相比 C 的 `if (ret < 0)` 更直观。
- `?` 操作符简化错误传播，减少样板代码。
- 消除了全局 `errno` 的线程安全隐患。

#### Rust模块化特性与LiteOS代码组织的适配性

Rust 的 `mod` 和 `crate` 系统提供了强大的模块化能力，可适配 LiteOS 的组件化改写。

- 详细阐述

LiteOS 通常按功能划分模块（如任务管理、内存管理）。Rust 的 `mod` 允许在单一文件中定义子模块，`crate` 支持外部库复用。通过 `pub` 关键字控制可见性，Rust 的模块化比 C 的头文件和源文件分离更灵活，且编译器可检查跨模块依赖。

- 代码示例

模拟 LiteOS 模块结构：

```rust
mod task {
    pub fn create_task() {
        println!("Task created");
    }
}

mod memory {
    pub fn alloc(size: usize) {
        println!("Allocated {} bytes", size);
    }
}

pub fn kernel_init() {
    task::create_task();
    memory::alloc(1024);
}

fn main() {
    kernel_init();
}
```

- `mod` 清晰划分功能单元，与 LiteOS 的模块化改写需求一致。
- `pub` 控制访问权限，避免 C 中头文件暴露过多细节的问题。

#### 用Rust重写中断处理例程

中断处理在嵌入式系统中要求极高的实时性，Rust 支持与 C 的 ISR 协作或使用 `#[naked]` 函数。

- 详细阐述

Rust 提供了 `no_mangle` 和 `extern "C"` 属性与 C 交互，`#[naked]` 可直接编写汇编代码。中断处理需要保存上下文并快速响应，Rust 的零成本抽象和手动内存管理能力使其适合此类场景。

- 代码示例

定时器中断

```rust
#[no_mangle]
pub extern "C" fn timer_interrupt() {
    unsafe {
        // 模拟保存上下文
        asm!("push {lr}");
        // 处理中断
        println!("Timer tick");
        // 恢复上下文
        asm!("pop {lr}");
    }
}
```

- `extern "C"` 确保与 C ABI 兼容。
- `unsafe` 和内联汇编支持低级操作，满足实时性要求。

#### Rust的裸机支持能力 

- `#![no_std]`：在没有操作系统、没有底层运行时的环境下，通过在 crate 根部加上 `#![no_std]`，禁用标准库，只依赖核心库 `core`，即可在裸机上运行。  
- `core` 提供基础设施：所有与操作系统无关的类型（整数、切片、字符串、原子类型等）都在 `core` 中，支持原子操作、内存模型等。

- 代码示例

一个最简裸机程序
```rust
#![no_std]
#![no_main]

use core::panic::PanicInfo;
use core::sync::atomic::{AtomicU32, Ordering};

// 必须提供 panic_handler
#[panic_handler]
fn panic(_info: &PanicInfo) -> ! { loop {} }

// 假设有一个 MMIO 寄存器，用原子操作模拟
static COUNTER: AtomicU32 = AtomicU32::new(0);

#[no_mangle]
pub extern "C" fn main() -> ! {
    // 每次进来自增
    COUNTER.fetch_add(1, Ordering::Relaxed);
    loop {}
}
```

#### 总结
综上所述，Rust 在系统编程领域的理论优势并非局限于理想化模型，而是通过严谨的语言规则和丰富的实践验证，展现出对嵌入式内核开发的深度兼容性。其内存安全机制、零成本抽象特性与模块化设计，不仅能够有效规避传统内核开发中的典型风险，还为高性能、高可靠性的系统演进提供了新的可能性。尽管引入 Rust 需权衡既有生态与学习成本，但其在安全性、可维护性上的收益，无疑为 LiteOS 的现代化改造开辟了一条值得探索的技术路径。后续研究可进一步聚焦于具体模块的实践验证与性能量化，以完整闭环理论到落地的可行性论证。

## 技术依据

### LiteOS编译

LiteOS使用Kconfig配置系统，基于GCC/Makefile实现组件化编译。

不论是Linux下使用make menuconfig命令配置系统，还是Windows下使用Huawei LiteOS Studio进行图形化配置，Huawei LiteOS都会同时解析、展示根目录下的.config文件和tools/menuconfig/config.in文件（该文件包含了各个模块的Kconfig文件），同时在开发板的include文件夹下生成menuconfig.h。config.in文件由Kconfig语言（一种菜单配置语言）编写而成。config.in文件决定了要展示的配置项，.config文件决定了各个配置项的默认值。

LiteOS通过在根目录下执行make命令完成自动化编译整个工程。对于根目录下的Makefile文件，其中包含了config.mk，config.mk又包含了los_config.mk，而los_config.mk则包含了各个模块的Makefile和.config文件，从而定义了对整个工程的编译链接规则。

Huawei LiteOS目前支持Windows及Linux平台的配置、编译。

- 对于Windows平台，提供了Huawei LiteOS Studio图形化IDE，用户可直接在Studio上完成配置、编译。
- 对于Linux平台，通过make menuconfig进行组件化配置及裁剪后，执行make命令完成编译。

由于我们选择在Linux平台下完成改写项目，下面以Ubuntu 22.04为例，详细介绍Linux下LiteOS编译流程。

#### 搭建环境

1. 安装GNU Arm Embedded Toolchain编译器和GNU Make构建器

```shell
sudo apt install gcc-arm-none-eabi
sudo apt install make
```

2. 安装python

```shell
sudo apt install python3
sudo ln -sf /usr/bin/python3 /usr/bin/python
```

3. 安装pip包管理工具

```shell
sudo apt install python3-setuptools python3-pip
sudo pip3 install --upgrade pip
```

4. 安装kconfiglib库

```shell
sudo pip install kconfiglib
```

#### 编译

1. 下载LiteOS代码

```shell
git clone https://gitee.com/LiteOS/LiteOS.git
```

2. 拷贝开发板配置文件

根据实际使用的开发板，拷贝`tools/build/config/`目录下的默认配置文件`{platform}.config`到根目录，并重命名为 `.config` 

```shell
cp tools/build/config/{platform}.config .config
```

3. 配置系统（可选）

```shell
make menuconfig
```

4. 清理工程，编译工程

```shell
make clean && make
```

编译成功后会看到如下输出：

```
########################################################################################################
########                      LiteOS build successfully!                                        ########
########################################################################################################
```

### qemu运行LiteOS

qemu是一款通用的开源虚拟化模拟器，通过软件模拟硬件设备，当QEMU直接模拟CPU时，它能够独立运行操作系统。realview-pbx-a9工程就是使用 qemu 模拟Cortex-A9处理器，以运行 LiteOS 操作系统。具体流程如下。

1. 搭建编译环境，具体参考前文

2. 安装qemu模拟器

```shell
sudo apt install qemu-system-arm
```

3. 编译，具体参考前文，其中第二步拷贝realview-pbx-a9模拟器工程的.config文件，即realview-pbx-a9.config文件

4. 运行

```shell
qemu-system-arm -machine realview-pbx-a9 -smp 4 -m 512M -kernel out/realview-pbx-a9/Huawei_LiteOS.bin -nographic
```

上述命令各参数含义如下：

- -machine：设置QEMU要仿真的虚拟机类型
- -smp：设置guest虚拟机的CPU的个数。因为realview-pbx-a9工程默认使能了SMP（多核），所以启动虚拟机时也需要设置-smp参数
- -m：为此guest虚拟机预留的内存大小，如果不指定，默认为128M
- -kernel：设置要运行的镜像文件（包含文件路径）
- -nographic：以非图形界面启动虚拟机

运行成功后会看到如下输出：

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.1.0
Processor   : Cortex-A9 * 4
Run Mode    : SMP
GIC Rev     : GICv1
build time  : Mar 26 2025 18:09:53

**********************************

main core booting up...
OsAppInit
releasing 3 secondary cores
cpu 0 entering scheduler
cpu 1 entering scheduler
cpu 3 entering scheduler
cpu 2 entering scheduler
app init!

Huawei LiteOS #
```

### C与Rust交互

#### 实现机制

C 与 Rust 的交互主要通过**外部函数接口（FFI）**实现，结合构建链接支持、数据类型映射和指针管理等策略，确保跨语言调用的安全性与效率。

##### 函数调用

- Rust调用C函数：使用 `extern "C"` 块声明C函数，并通过 `unsafe` 调用
- C调用Rust函数：使用 `extern "C"` 和 `#[no_mangle]` 暴露Rust函数

##### 数据类型映射

- 基本类型：Rust的基础类型（如`i32`）与C语言对应类型（如`int32_t`）无缝兼容
- 复杂类型：使用 `#[repr(C)]` 确保结构体等复杂类型内存布局兼容

##### 指针管理

通过 `Box::into_raw` 将 Rust 对象转为指针供C使用，通过 `Box::from_raw` 将指针转为 Rust 对象

##### 构建与链接

- Rust可以在 `Cargo.toml` 或者构建脚本里指定 C 的链接库
- 可以配置Rust代码编译为C兼容的静态库或者动态库，供C链接使用


#### 实现示例

##### C调用Rust

1. 创建lib项目`c_call_rust`，同时创建C文件，完成后，项目目录结构如下

```
.
├── csrc/
│   ├── main.c
└── src/
    ├── lib.rs
└── target/
    ├── .gitignore
	├── Cargo.lock
    └── Cargo.toml
```

2. 编写`lib.rs`

```rust
#[unsafe(no_mangle)]
pub fn add(left: i32, right: i32) -> i32 {
    left + right
}
```

为了防止函数名被编译器修改，可以加上`#[unsafe(no_mangle)]`。

为了能让rust的函数通过FFI被C调用，需要加上`extern "C"`对函数进行修饰。

3. 编写`Cargo.toml`

```toml
[package]
name = "c_call_rust"
version = "0.1.0"
edition = "2024"

[lib]
name="add"
crate-type = ["staticlib"]
```

其中`crate-type = ["staticlib"]`指定rustc编译结果是什么类型，默认为rust自用的rlib格式库，为了让C语言调用，需要更改为静态库或者动态库，这里指定为静态库类型。

4. 编译rust项目

```shell
cargo build
```

5. 编写`main.c`

```C
#include <stdio.h>
#include <stdint.h>

extern int32_t add(int32_t left, int32_t right);

int main()
{
    printf("1 + 1 = %d\n", add(1, 1));
    return 0;
}
```

这里就和写正常C语言代码差不多，唯一需要注意的是声明一下要使用的rust函数。

6. 编译`main.c`

```shell
gcc -o main main.c -L../target/debug -ladd
```

`-L../target/debug`指定静态库所在的目录，`-ladd`链接名为`libadd.a`的静态库。

7. 运行`main`程序，观察到以下结果

```
1 + 1 = 2
```

##### Rust调用C

1. 创建项目rust_call_c，同时创建C文件以及构建脚本文件，完成后，项目目录结构如下

```shell
.
├── csrc/
│   ├── add.c
└── src/
    ├── main.rs
└── target/
    ├── .gitignore
    ├── build.rs
	├── Cargo.lock
    └── Cargo.toml
```

2. 编写`Cargo.toml`，添加配置和必要的依赖

```toml
[package]
name = "rust_call_c"
version = "0.1.0"
edition = "2024"
build = "build.rs"

[dependencies]
libc = "0.2.171"

[build-dependencies]
cc = "1.2.18"
```

- `build = "build.rs"`指定了一个自定义的构建脚本文件`build.rs`。这个脚本会在 Cargo 构建项目时运行，通常用于处理额外的构建任务，例如编译C程序。
- `libc = "0.2.171"`声明了项目对 libc 的依赖 。libc是一个常用的crate，提供了对 C 标准库和系统调用的绑定，通常用于与 C 代码交互。
- `cc = "1.2.18"`声明了项目对 cc 的构建依赖。cc是一个构建工具，用于在构建过程中编译C代码，它通常与build.rs配合使用，用于将C代码编译为静态库或动态库，并链接到 Rust 项目中。

3. 编写 `build.rs`

```rust
use cc;

fn main() {
    cc::Build::new().file("csrc/add.c").compile("add.a");
}
```

这段构建脚本的主要作用是

- 在构建过程中编译 `add.c` 文件。

- 将生成的静态库 `add.a` 链接到 Rust 项目中，使得 Rust 代码可以调用 `add.c` 中的函数。

- 编写 `add.c`

```c
int add(int left, int right)
{
    return left + right;
}
```

5.  编写 `main.rs`

```rust
use libc::c_int;

unsafe extern "C" {
    unsafe fn add(left: c_int, right: c_int) -> c_int;
}

fn main() {
    println!("{}", unsafe { add(1, 1) });
}
```

`unsafe extern "C" { ... }` 这部分代码声明了一个外部的 C 函数接口：

- `unsafe`表示调用这个函数可能是不安全的，因为 Rust 无法验证它的行为是否符合安全性要求，调用者需要显式地使用 `unsafe` 块来调用它。
- `extern "C"`指定了函数的调用约定为 C 语言的 ABI，这确保了 Rust 编译器生成的代码能够正确地与 C 函数交互。

6. 运行

```shell
cargo run
```

观察到正确结果。

#### 工具支持

在上述示例中，我们采取了手动声明函数接口的方式。但是当函数接口繁杂时，这无疑是一项非常艰巨的任务。幸运的是，在 C 与 Rust 的交互中，合理使用两个核心工具 `bindgen` 和 `cbindgen`，能显著降低手动编写和维护跨语言接口的成本，同时避免类型不匹配或内存布局错误。

##### bindgen

`bindgen` 是一个从 C 到 Rust 的绑定生成器，可以将 C/C++ 头文件自动转换为 Rust 的 FFI（外部函数接口）绑定代码。

###### 安装

- Command Line Usage

```shell
cargo install bindgen-cli
```

- Library Usage with `build.rs`

```toml
[build-dependencies]
bindgen = "0.71.1"
```

###### 使用

假设我们需要从一个名为 `input.h` 的 C 头文件生成 Rust FFI 绑定，并将结果保存到 `bindings.rs` 文件中。

- Command Line Usage

```shell
bindgen input.h -o bindings.rs
```

- Library Usage with `build.rs`

```rust
use std::env;
use std::path::PathBuf;

fn main() {
    let bindings = bindgen::Builder::default()
        .header("c_lib/header.h")
        .parse_callbacks(Box::new(bindgen::CargoCallbacks::new()))
        .generate()
        .expect("Unable to generate bindings");
    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Couldn't write bindings!");
}
```

在 Rust 代码中使用生成的绑定方式如下

```rust
include!(concat!(env!("OUT_DIR"), "/bindings.rs"));
```

##### cbindgen

`cbindgen` 是从 Rust 到 C 的头文件的生成器，可以将 Rust 的 `extern "C"` 接口自动生成 C/C++ 头文件，供 C 代码调用。

###### 安装

```shell
cargo install --force cbindgen
```

`--force` 用于强制更新到最新版本，如果已安装过旧版

###### 使用要求

1. 配置文件：`cbindgen.toml`（初始可为空文件）
2. Rust crate：需包含公开的 C 接口 API

###### 基本使用

```shell
cbindgen --config cbindgen.toml --crate my_rust_library --output my_header.h
```

默认生成 C++ 头文件，如果想要生成 C 头文件，则添加 `--lang c` 参数。

###### 集成到构建脚本

若不想通过命令行使用 cbindgen，可将以下示例代码添加到 `build.rs` 中，并且在 `Cargo.toml` 中添加 `cbindgen` 构建依赖。

```rust
extern crate cbindgen;
use std::env;

fn main() {
    let crate_dir = env::var("CARGO_MANIFEST_DIR").unwrap();

    cbindgen::Builder::new()
      .with_crate(crate_dir)
      .generate()
      .expect("Unable to generate bindings")
      .write_to_file("bindings.h");
}
```

最后，有关 `bindgen` 和 `cbindgen` 的其它使用介绍参考官网，此处不在赘述。

## 创新点

本项目希望使用Rust语言重写LiteOS内核模块，将改写的Rust程序融入到基于C语言的LiteOS工程中，让整个操作系统能够正常运行，提高操作系统的安全性，这是一个非常有意义的工作，它能为后来的OS的部分改写工作提供借鉴思路，并且为不同语言的代码集成技术提供了参考。

## 参考文献

1. [The Rust Programming Language](https://doc.rust-lang.org/stable/book/index.html)
2. [The bindgen User Guide](https://rust-lang.github.io/rust-bindgen/)
3. [LiteOS 文档](https://support.huaweicloud.com/productdesc-LiteOS/zh-cn_topic_0145347223.html)
4. [LiteOS 仓库](https://gitee.com/LiteOS/LiteOS)

5. [cc crate](https://docs.rs/cc/latest/cc/)

6. [The Rustonomicon](https://doc.rust-lang.org/nomicon/intro.html)

7. [bindgen crate](https://docs.rs/bindgen/latest/bindgen/)

8. [cbindgen User Guide](https://github.com/mozilla/cbindgen/blob/master/docs.md)

9. [cbindgen crate](https://docs.rs/cbindgen/0.28.0/cbindgen/)

10. [libc 仓库](https://github.com/rust-lang/libc/tree/30f03b2a990dd529cf4b2c433c738f4a8e366417)

11. [libc crate](https://docs.rs/libc/latest/libc/)

12. [The Cargo Book](https://doc.rust-lang.org/cargo/index.html)

13. [Rust By Example](https://doc.rust-lang.org/rust-by-example/)

14. [The Embedded Rust Book](https://doc.rust-lang.org/stable/embedded-book/)

15. [The Rust Reference](https://doc.rust-lang.org/reference/index.html)

16. [Command Line Applications in Rust](https://rust-cli.github.io/book/index.html)

17. [The rustc book](https://doc.rust-lang.org/rustc/index.html)
