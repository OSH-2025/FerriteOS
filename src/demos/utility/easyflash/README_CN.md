
# EasyFlash Demo说明文档
`EasyFlash`是一个用于管理设备环境变量表的开源库，实现对存储在flash上的一些设备参数信息进行读写，擦除等操作。

## 使能 EasyFlash Demo

1. 以`Linux`系统编译环境和`Cloud_STM32F429IGTx_FIRE`开发板为例，拷贝`Cloud_STM32F429IGTx_FIRE`开发板的默认配置文件`${platform}.config`到在`LiteOS`工程的根目录下，并重命名为`.config`。  
```
   cp tools/build/config/Cloud_STM32F429IGTx_FIRE.config .config
```

2. 在`LiteOS`工程的根目录下执行`make menuconfig`命令，并勾选以下相关配置，使能`EasyFlash Demo`：  

```
   Demos  --->
        Utility Demo  --->
               [*] Enable EasyFlash Demo
```

3. 勾选`EasyFlash Demo`后，自动使能其依赖的`EasyFlash`组件。保存并退出`menuconfig`后，自动执行`components/download.sh`脚本。  
* 从互联网下载`EasyFlash-3.3.0`源代码。  
* 从`gitee`的`LiteOS_Components`组件仓库下载对应的patch包，并把该patch打入源码中。  

关于组件自动下载的详细流程，请查阅`components/download.sh`脚本。  

**注意**：下载组件时，`Linux`环境可以访问互联网，并已安装`git`工具。  

## 编译运行 EasyFlash Demo
1. 在`LiteOS`工程根目录下执行以下命令进行编译：  
```
make clean ; make -j
```
2. 编译结束后，在`out/Cloud_STM32F429IGTx_FIRE/lib`目录下生成`libeasyflash.a`和`libef_demo.a`，系统镜像文件为out/Cloud_STM32F429IGTx_FIRE/`Huawei_LiteOS.bin`。  
3. 烧录系统镜像文件到`Cloud_STM32F429IGTx_FIRE`开发板，上电复位后可以看到串口输出Demo运行结果日志如下所示。  
```txt
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.1.0
build data : Aug 17 2021 17:13:30

**********************************
OsAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
[SFUD]Find a Winbond W25Q256JV flash chip. Size is 33554432 bytes.
[SFUD]W25Q256JV flash device is initialize success.
[Flash]EasyFlash V3.3.0 is initialize success.

[Flash]You can get the latest version on https://github.com/armink/EasyFlash .


Huawei LiteOS # Ready to write Event.
Save env, "TickCount"=2007
Ready to write Event.
Save env, "TickCount"=4007
Ready to write Event.
Save env, "TickCount"=6007
Ready to write Event.
Save env, "TickCount"=8007
Get Env, TickCount=8007
Swtmr delete.
Mutex delete.
Event destroy.
Easyflash task delete.
```

