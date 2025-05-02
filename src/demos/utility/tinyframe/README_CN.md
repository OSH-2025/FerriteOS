# Tinyframe Demo说明文档

Tinyframe是一个简单的库，用于构建和解析要通过串行接口（例如 UART、telnet、套接字）发送的数据帧，设计小巧紧凑，可以方便的移植到嵌入式设备应用程序中。

LiteOS中提供了`TinyframeDemoTask`来演示如何使用`Tinyframe`组件。目前支持在以下开发板上运行`TinyframeDemoTask`。

- STM32F769
- STM32F429
- realview-pbx-a9
- qemu-virt-a53

## 解析Tinyframe Demo

在Tinyframe_demo.c中主要实现3个功能：

1. 不带侦听器和结构的发送`TF_SendSimple()`;
2. 发送时侦听器正在等待答复，但不使用结构`TF_QuerySimple()`；
3. 不带侦听器发送`TF_Send()`；


## 使能Tinyframe Demo

在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能Tinyframe Demo。 

```
Demos  --->
       Utility Demo  --->
               [*] Enable Tinyframe Demo (NEW)
```

- 使能Tinyframe Demo后会自动使能Tinyframe组件。  

保存退出后，会从github上自动下载Tinyframe源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。 

## 编译运行Tinyframe Demo

操作步骤如下：

1. 参照上述使能Tinyframe demo。

2. 在Linux主机的LiteOS源码根目录下执行`make clean ; make -j`命令编译LiteOS工程，编译成功后将在~/Huawei_LiteOS/out/${platform}/lib目录下生成Tinyframe的库文件libTinyframe.a和此Demo库文件libTinyframe_demo.a，系统镜像文件为Huawei_LiteOS.bin。以`realview-pbx-a9/`开发板为例，其目录就是`out/realview-pbx-a9/`。

3. 执行`qemu-system-arm -machine realview-pbx-a9 -smp 4 -m 512M -kernel out/realview-pbx-a9/Huawei_LiteOS.bin -nographic`命令后，可以看到串口输出Demo部分运行结果，如下所示。

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.1.0
Processor   : Cortex-A9 * 4
Run Mode    : SMP
GIC Rev     : GICv1
build time  : Aug 18 2021 11:30:07

**********************************

main core booting up...
OsAppInit
releasing 3 secondary cores
cpu 0 entering scheduler
cpu 3 entering scheduler
cpu 2 entering scheduler
cpu 1 entering scheduler
app init!
Tinyframe demo task start to run.
--------------------
TF_WriteImpl - sending frame:
  1 01 .
128 80 .
  0 00 .
  7 07 .
  1 01 .
240 F0 .
215 D7 .
108 6C l
105 69 i
116 74 t
101 65 e
111 6F o
115 73 s
  0 00 .
213 D5 .
 30 1E .
--- end of frame ---

GenericListener demo
Frame info
  type: 01h
  data: "liteos"
   len: 7
    id: 80h

--------------------
...
...
...
```
