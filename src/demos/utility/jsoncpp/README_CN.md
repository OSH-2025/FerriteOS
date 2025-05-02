# Jsoncpp Demo 说明文档

Jsoncpp是基于C++的一个轻量级的JSON格式文件编解码器，可以运行在多个硬件平台上。详情参考<a href="https://github.com/open-source-parsers/jsoncpp" target="_blank">Jsoncpp</a>。

LiteOS中提供对C++的支持，并提供Jsoncpp Demo来演示如何使用Jsoncpp。

目前暂时只支持以下开发板运行Jsoncpp Demo：

- STM32F429
- STM32F769
- realview-pbx-a9


## 使能Jsoncpp Demo

1. 在LiteOS源码根目录下根据实际使用的开发板，拷贝tools/build/config/目录下的默认配置文件${platform}.config到根目录，并重命名为.config。

2. 继续在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能C++支持和Jsoncpp Demo。

    ```
    Kernel  --->
        [*] Enable Extend Kernel
        [*]     C++ Support

    Demos  --->
        Utility Demo  --->
                [*] Enable Jsoncpp Demo
    ```
使能Jsoncpp Demo后，LiteOS会去下载Jsoncpp源码，并打入patch。

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

保存退出后，LiteOS会从github上自动下载Jsoncpp源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。
详细流程在`components/download.sh`文件中。

## 编译运行Jsoncpp Demo

使能Jsoncpp Demo后，在LiteOS源码根目录下执行`make clean; make -j`命令编译LiteOS工程，编译成功后会在`out/${platform}/lib`路径下生成`Jsoncpp.a`和`Jsoncpp_demo.a`文件，系统镜像文件为Huawei_LiteOS.bin。

**注意：** 选择不同开发板out路径会有不同。

烧录系统镜像文件到开发板中可以看到如下运行结果。
```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
build data : Apr 14 2021 12:23:47

**********************************
osAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
Jsoncpp read demo start to run.
Jsoncpp read form string:
name : LiteOS
date : 2021
Jsoncpp write demo start to run.
Jsoncpp write to string:
{
	"data" : 
	{
		"date" : 2021
	},
	"name" : "LiteOS"
}
Jsoncpp demo finished.

Huawei LiteOS # 
```