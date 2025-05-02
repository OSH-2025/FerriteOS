# Libxml2 Demo说明文档
Libxml2库提供了C语言解析和构造xml文档的接口，可以运行在很多硬件平台。以Cloud_STM32F429IGTx_FIRE开发板为例，介绍如何在Cloud_STM32F429IGTx_FIRE开发板上使能并解析Xml2文档。


## Libxml2 Demo简介

LiteOS中提供了`Libxml2DemoTask` 程序来演示如何使用Libxml2库。调用入口在`targets/bsp/common/demo_entry.c` 内。

```c
#ifdef LOSCFG_DEMOS_LIBXML2
    Libxml2DemoTask();
#endif
```

函数实现在`demos/utility/libxml2/libxml2_demo.c`文件中。

Xml2 Demo 定义文档和节点指针，解析文档，获取文档根元素，定位子节点获取子节点的内容并输出。

## 使能libxml2 Demo

1. 在LiteOS源码根目录下执行`cp tools/build/config/Cloud_STM32F429IGTx_FIRE.config .config`设置相应开发板的默认配置。

2. 使能Libxml2 Demo。本Demo使用SD存放xml文件，所以需要使能FATFS文件系统并设置为SD卡模式`FileSystem --> Enable FATFS --> Choose Peripheral Driver`，使能libxml2组件`Utility --> Enable Xml2`后，最后再使能Demo`Utility Demo --> Enable Libxml2 Demo`。在LiteOS源码根目录下运行`make menuconfig`命令，按如下菜单路径使能Xml2 Demo：

  ```
  Demos	--->
      Utility Demo	--->
        [*] Enable Libxml2 Demo (NEW)
  ```

保存退出后，LiteOS会从github上自动下载libxml2源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。
详细流程在`components/download.sh`文件中。

**注意**：下载需要Linux主机可以访问互联网，并安装`git`。


## 编译运行Libxml2 Demo

操作步骤如下：

1. 将待解析源xml文件存入SD卡，默认路径为`/fatfs`，文件名为`liteos_test.xml` 。
2. 将SD卡插入开发板的卡槽内。
3. 参照上述步骤使能Xml2 Demo。
4. 在LiteOS源码根目录下执行`make clean ; make -j`命令编译LiteOS工程，编译成功后将在`out/Cloud_STM32F429IGTx_FIRE/lib`目录下生成组件库文件libxml2.a和此Demo库文件libxml2_demo.a，系统镜像文件为Huawei_LiteOS.bin。
5. 烧录系统镜像文件到开发板，复位后可以看到串口输出Demo运行结果如下所示。

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
build data : Mar 13 2021 01:45:11

**********************************
osAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
Libxml2 demo task start to run.
file system mount success

name: XiaoMing 
addr: RenMinRoad 
Libxml2 demo task finished.

Huawei LiteOS # 
```

