# Tinyxml2 Demo说明文档

Tinyxml2是一类简易，轻量，高效的C++ XML解析器，可以方便的移植到嵌入式设备应用程序中。

LiteOS中提供对C++的支持，并提供Tinyxml2 Demo来演示如何使用Tinyxml2，目前只支持在以下开发板上运行`Tinyxml2DemoTask`。

- STM32F429
- STM32F769
- realview-pbx-a9
- qemu-virt-a53

## 解析Tinyxml2 Demo

在tinyxml2_demo.cpp中，通过`SaveFile`函数将xml格式的个人信息存储到test.xml文件中，再通过`ReadXML`将个人信息读取出来，xml文本内容如下：

```xml
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<root>
    <user school="guangming" grade ="8">
        <name>xiaoqiang</name>
        <sex>male</sex>
        <age>14</age>
    </user>
</root>
```

## 使能Tinyxml2 Demo

1.在LiteOS源码根目录下根据实际使用的开发板，拷贝tools/build/config/目录下的默认配置文件${platform}.config到根目录，并重命名为.config。

2.继续在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能Tinyxml2组件和Tinyxml2 Demo。

```
Kernel  --->
    [*] Enable Extend Kernel
    [*]     C++ Support
Demos  --->
    Utility Demo  --->
            [*] Enable Tinyxml2 Demo
```

**注意**

1. realview-pbx-a9开启Jsoncpp Demo需关闭smp。
    ```
    Kernel  --->
        [ ] Enable Kernel SMP  ----
    ```

2. 如需开启C++ thread特性，需要使能LOSCFG_LIB_CPP_EXTEND。
    ```
    Lib  --->
        [*] Enable libc++ extend
    ```
    使能LOSCFG_LIB_CPP_EXTEND后，LiteOS会去下载libstdc++源码，并打入patch。

    保存退出后，LiteOS会从github上自动下载Tinyxml2源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。
    详细流程在`components/download.sh`文件中。

**注意：** 下载需要Linux主机可以访问互联网，并安装`git`。

## 编译运行Tinyxml2 Demo

使能Tinyxml2 Demo后，在LiteOS源码根目录下执行`make clean; make -j`命令编译LiteOS工程，编译成功后会在`out/realview-pbx-a9/lib`路径下生成`tinyxml2.a`和`tinyxml2_demo.a`文件，系统镜像文件为Huawei_LiteOS.bin。

**注意：** 选择不同开发板out路径会有不同。

执行以下命令即可运行LiteOS工程，更详细的信息可以参考[CORTEX A9在QEMU下运行LiteOS](https://gitee.com/LiteOS/LiteOS/blob/master/targets/realview-pbx-a9/README_CN.md)。

```
~/Huawei_LiteOS$ qemu-system-arm -machine realview-pbx-a9 -m 512M -kernel out/realview-pbx-a9/Huawei_LiteOS.bin -nographic
```
执行上述命令后可以看到如下运行结果。

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
Processor  : Cortex-A9
Run Mode   : UP
GIC Rev    : GICv1
build time : Apr 20 2021 15:55:30

**********************************

main core booting up...
osAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
Tinyxml2 demo start to run.
school : guangming
grade : 8
name : xiaoqiang
sex : male
age : 14
Tinyxml2 demo finished.

Huawei LiteOS # 
```

