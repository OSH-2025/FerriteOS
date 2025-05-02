# Sqlite Demo说明文档

Sqlite是一个轻型的嵌入式SQL数据库引擎。它需要文件系统的支持，可运行在已支持文件系统的开发板上。

本Demo使用`FATFS`文件系统，打开一个名为`database.db`的数据库文件，在该文件中创建名为`userinfo`的表，并在表中插入两条数据，最后遍历打印该表中的数据。本Demo可以运行在支持`FATFS`文件系统的开发板中，目前包括：
- Cloud_STM32F429IGTx_FIRE
- STM32F769IDISCOVERY

## 使能Sqlite Demo

1. 在LiteOS源码根目录下根据实际使用的开发板，拷贝tools/build/config/目录下的默认配置文件${platform}.config到根目录，并重命名为.config。以Linux系统和Cloud_STM32F429IGTx_FIRE开发板为例，在LiteOS源码根目录下，执行如下命令完成配置文件的拷贝和重命名：
   ```
   cp tools/build/config/Cloud_STM32F429IGTx_FIRE.config .config
   ```

2. 使能Sqlite Demo。本Demo使用SD存放数据库文件，所以需要使能FATFS文件系统并设置为SD卡模式，使能Sqlite组件后，最后再使能本Demo。在LiteOS源码根目录下运行`make menuconfig`命令，按如下路径使能各组件及Demo：
   ```
   Components  --->
        FileSystem  --->
            -*- Enable FATFS (NEW)
                    Choose Peripheral Driver (SD Card)  --->
                        (X) SD Card
        Utility  --->
            -*- Enable Sqlite

   Demos  --->
        Utility Demo  --->
               [*] Enable Sqlite Demo
   ```

保存退出后，LiteOS会从github自动下载Sqlite源码，并从gitee上的LiteOS组件仓库里下载对应的patch包，并将patch打入源码，此过程的详细流程，请查阅`components/download.sh`文件。

**注意**：下载需要Linux主机可以访问互联网，并安装`git`工具。


## 编译运行Sqlite Demo

在LiteOS源码根目录下执行如下命令编译LiteOS工程：
```
make clean; make -j
```
编译成功后将在`out/${platform}/lib`目录下生成Sqlite的库文件`libsqlite.a`和本Demo的库文件`libsqlite_demo.a`，系统镜像文件为`Huawei_LiteOS.bin`，以Cloud_STM32F429IGTx_FIRE开发板为例，其目录就是`out/Cloud_STM32F429IGTx_FIRE/lib`。将系统镜像文件烧录到开发板，重启开发板Demo即开始运行，串口输出结果如下所示：

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
build data : Apr  1 2021 18:20:09

**********************************
osAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
Sqlite demo task start to run.
file system mount success

name: huawei1; number: 20210324
name: huawei2; number: 20210401
Sqlite demo task finised.

Huawei LiteOS # 
```

其中输出的两行内容`name: xxx number: xxx`就是写入`userinfo`表中的数据。