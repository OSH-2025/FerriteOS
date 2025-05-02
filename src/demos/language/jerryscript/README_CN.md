# Jerryscript Demo说明文档

`Jerryscript`是一个轻量级的JavaScript引擎，可运行在内存资源受到限制的嵌入式设备上。

本demo仅是一个简单的jerryscript示例，其实现的功能是运行一段js代码：`print("Hello, World! I am JerryScript.");`。目前LiteOS已适配的开发板中可以支持其运行的开发板有：
  - realview-pbx-a9


## Jerryscript Demo

1. 在`LiteOS`源码根目录下根据实际使用的开发板，拷贝`tools/build/config/`目录下的默认配置文件`${platform}.config`到根目录，并重命名为`.config`。以`Linux`系统和`realview-pbx-a9`开发板为例，在`LiteOS`源码根目录下，执行如下命令完成配置文件的拷贝和重命名：
   ```
   cp tools/build/config/realview-pbx-a9/.config .config
   ```

2. 使能`jerryscript Demo`。在`LiteOS`源码根目录下运行`make menuconfig`命令，按如下路径使能demo：
   ```
   Demos  --->
      Language Demo  --->
         [*] Enable Jerryscript Demo
   ```

   使能本demo后，会自动使能其依赖的`jerryscript`组件及`vfs`文件系统适配层。保存退出`menuconfig`后，`LiteOS`会从`github`上自动下载`jerryscript`源代码，关于在线组件的自动下载，请查阅<a href="https://gitee.com/LiteOS/LiteOS_Components#在线组件下载流程" target="_blank">在线组件下载流程</a>。

   **注意**：下载需要`Linux`主机可以访问互联网，并安装`git`工具。

## 编译运行Jerryscript Demo

操作步骤如下：

1. 参照上述步骤使能`jerryscript demo`。
2. 在`LiteOS`源码根目录下执行`make clean ; make -j`命令编译`LiteOS`工程，编译成功后将在`out/${platform}/lib`目录下生成`jerryscript`的库文件`libjerryscript.a`和本Demo的库文件`libjerryscript_demo.a`，系统镜像文件为`Huawei_LiteOS.bin`。以`realview-pbx-a9`开发板为例，其目录就是`out/realview-pbx-a9`。
3. 烧录系统镜像文件到开发板，复位后可以看到串口输出Demo运行结果如下所示。

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.1.0
Processor   : Cortex-A9 * 4
Run Mode    : SMP
GIC Rev     : GICv1
build time  : Jul 20 2021 14:47:42

**********************************

main core booting up...
OsAppInit
releasing 3 secondary cores
cpu 0 entering scheduler
cpu 3 entering scheduler
cpu 1 entering scheduler
cpu 2 entering scheduler
app init!
Hello, welcome to liteos demo!
Jerryscript demo task start to run.
This test run the following script code: print("Hello, World! I am JerryScript.");
Hello, World! I am JerryScript.
Jerryscript demo task finished.

Huawei LiteOS # 
```
更多内容及示列可参考：https://github.com/jerryscript-project/jerryscript

