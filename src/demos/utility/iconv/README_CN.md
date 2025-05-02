# Iconv Demo说明文档

libiconv是一个开源的字符集转换库，可以在已知的字符集之间相互转换。详情参考<a href="https://github.com/winlibs/libiconv" target="_blank">libiconv</a>。

LiteOS中提供了`IconvDemoTask`来演示如何使用`Iconv组件`。根据`iconv`所需资源，暂时只支持在以下开发板上运行`IconvDemoTask`。
- STM32F429
- STM32F407
- STM32F769
- realview-pbx-a9
- qemu-virt-a53


## 解析Iconv Demo

在`iconv_demo.c`中，`CodeConvert()`通过调用`libiconv`库中的`iconv_open()`函数，生成两种字符转换的结构体`cd`，然后作为入参传入`iconv()`函数中，就可以对输入数据进行字符转换，然后保存到输出数据中。

`iconv_demo.c`中分别对`GB18030`，`GBK`，`GB2312`的字符和`utf-8`的字符进行相互转换。


## 使能Iconv Demo

1. 在LiteOS源码根目录下根据实际使用的开发板，拷贝tools/build/config/目录下的默认配置文件${platform}.config到根目录，并重命名为.config。

2. 继续在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能Iconv组件和Iconv Demo。

    ```
    Components -->
        Utility  --->
                [*] Enable Iconv
    Demos  --->
        Utility Demo  --->
                [*] Enable Iconv Demo
    ```

保存退出后，LiteOS会从github上自动下载libiconv源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。
详细流程在`components/download.sh`文件中。

**注意：** 下载需要Linux主机可以访问互联网，并安装`git`。


## 编译运行Iconv Demo

使能Iconv Demo后，在LiteOS源码根目录下执行`make clean; make -j`命令编译LiteOS工程，编译成功后会在`out/Cloud_STM32F429IGTx_FIRE/lib`路径下生成`libiconv.a`和`libiconv_demo.a`文件，系统镜像文件为Huawei_LiteOS.bin。

**注意：** 选择不同开发板out路径会有不同。

烧录系统镜像文件到开发板中可以看到如下运行结果。
```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
build data : Mar 13 2021 01:26:38

**********************************
osAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
Iconv demo task start to run.
unicode -> gb2312 : 姝ｅ湪瀹夎 -> 正在安装
gb2312  -> unicode: 正在安装 -> 姝ｅ湪瀹夎
unicode -> gdk    : 姝ｅ湪瀹夎 -> 正在安装
gdk     -> unicode: 正在安装 -> 姝ｅ湪瀹夎
unicode -> gb18030: 姝ｅ湪瀹夎 -> 正在安装
gb18030 -> unicode: 正在安装 -> 姝ｅ湪瀹夎
Iconv demo task finished.

Huawei LiteOS #
```
