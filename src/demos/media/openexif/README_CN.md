# Openexif Demo 说明文档

OpenExif是用于访问Exif格式的JPEG图像文件的面向对象的库。该工具包允许在Exif文件中创建，读取和修改元数据。详情参考<a href="http://openexif.sourceforge.net/" target="_blank">Openexif</a>。

LiteOS中提供对C++的支持，并提供Openexif Demo来演示如何使用Openexif。

目前暂时只支持以下开发板运行Openexif Demo：

- STM32F769

## 使能Openexif Demo

1. 在LiteOS源码根目录下根据实际使用的开发板，拷贝tools/build/config/目录下的默认配置文件${platform}.config到根目录，并重命名为.config。

2. 继续在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能C++支持和Openexif Demo。

    ```
    Kernel  --->
        [*] Enable Extend Kernel
        [*]     C++ Support

    Demos  --->
        Media Demo  --->
                [*] Enable Openexif Demo
    ```
使能Openexif Demo后，LiteOS会去下载Openexif源码，并打入patch。

**注意**

1. 如需开启C++ thread特性，需要使能LOSCFG_LIB_CPP_EXTEND。并动态调整Task Defalut Stack Size的大小。

    ```
    Kernel  --->
        Basic Config  --->
            Task  --->
                (4096)Task Defalut Stack Size
    Lib  --->
        [*] Enable libc++ extend
    ```
使能LOSCFG_LIB_CPP_EXTEND后，LiteOS会去下载libstdc++源码。

保存退出后，LiteOS会从github上自动下载Openexif源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。
详细流程在`components/download.sh`文件中。

## 编译运行Openexif Demo

1. 需要一张SD卡，并在SD卡中放入一张测试图片。
**注意：** 由于嵌入式设备内存有限，文件过大可能导致打开失败。大小限制在100kb以下即可。

2. 修改`demos/media/openexif/openexif_demo.cpp`文件中`DEMO_FILENAME`为放入SD卡的图片文件名。本Demo所用文件名为`/fatfs/test.jpg`，可根据实际自行修改。
```C
#define DEMO_FILENAME               "/fatfs/test.jpg"

```

3. 使能Openexif Demo后，在LiteOS源码根目录下执行`make clean; make -j`命令编译LiteOS工程，编译成功后会在`out/${platform}/lib`路径下生成`openexif.a`和`openexif_demo.a`文件，系统镜像文件为Huawei_LiteOS.bin。

**注意：** 选择不同开发板out路径会有不同。

烧录系统镜像文件到开发板中可以看到如下运行结果。
```

********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
build data : Apr 20 2021 17:05:27

**********************************
osAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
Openexif demo task start to run.
file system mount success
"Exif" entries:
IFD: 0  Idx: 0
Tag#	Type	Count	Value
282	5	1	72
283	5	1	72
296	3	1	2
531	3	1	1
34665	4	1	90
IFD: 0  Idx: 0
IFD: 34665  Idx: 0
Tag#	Type	Count	Value
36864	7	4	[48,50,49,48]
37121	7	4	[1,2,3,0]
40960	7	4	[48,49,48,48]
40961	3	1	1
40962	4	1	0
40963	4	1	0
Openexif demo task finished.

Huawei LiteOS # 
```

可以看到Openexif Demo会去读取图片文件的元数据，并打印出来。

其他更多关于exif的信息请参考<a href="http://www.exif.org/" target="_blank">Exif</a>。