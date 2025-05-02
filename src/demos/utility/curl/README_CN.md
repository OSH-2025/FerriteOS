# Curl Demo说明文档

curl是一个使用URL语法进行文件传输的工具。

由于curl demo需要网络和文件系统的支持，目前仅Cloud_STM32F429IGTx_FIRE开发板适配了网络驱动，因此，本demo将基于Cloud_STM32F429IGTx_FIRE开发板演示如何在程序中调用curl提供的API来下载一个文件，并将其保存到本地文件中。更多示列请参考网址：https://github.com/curl/curl/tree/master/docs/examples。

## 参数配置
修改demos/tool/curl/curl_demo.c文件中的以下参数：
- URL_ADDRESS：`192.168.3.37`为测试用地址（文件下载地址，请根据实际情况进行修改）。

## 使能Curl Demo

拷贝Cloud_STM32F429IGTx_FIRE开发板的默认配置文件tools/build/config/Cloud_STM32F429IGTx_FIRE.config到LiteOS源码根目录，并重命名为.config。以Linux系统为例，在LiteOS源码根目录下，执行如下命令完成配置文件的拷贝和重命名：
```
cp tools/build/config/Cloud_STM32F429IGTx_FIRE.config .config
```

本demo使用SD存储下载的测试文件，所以需要使能FATFS文件系统并设置为SD卡模式，在LiteOS源码根目录下执行命令：
```
make menuconfig
```

按如下路径使能各组件及demo：
```
Components  --->
        FileSystem  --->
            -*- Enable FATFS (NEW)
                    Choose Peripheral Driver (SD Card)  --->
                        (X) SD Card
        Network  --->
            -*- Enable Lwip
                    Enable Ipv4 Or Ipv6 (Enable Ipv4)  --->
                        (X) Enable Ipv4
        Utility  --->
            -*- Enable Curl

Demos  --->
       Utility Demo  --->
               [*] Enable Curl Demo
```

保存退出后，LiteOS会从github自动下载curl源码，并从gitee上的LiteOS组件仓库里下载对应的patch包，并将patch打入源码，此过程的详细流程，请查阅`components/download.sh`文件。

**注意**：下载需要Linux主机可以访问互联网，并安装`git`工具。


## 编译运行Curl Demo

在LiteOS源码根目录下执行如下命令编译LiteOS工程：
```
make clean;make -j
```
编译成功后将在`out/Cloud_STM32F429IGTx_FIRE/lib`目录下生成curl的库文件`libcurl.a`和本demo的库文件`libcurl_demo.a`，系统镜像文件为`Huawei_LiteOS.bin`。将系统镜像文件烧录到开发板，重启开发板demo即开始运行，串口输出结果如下所示：

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
build data : Mar 19 2021 15:19:22

**********************************
osAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
lwip test init ok.
Curl demo task start to run.
Fatfs file system mount success

Huawei LiteOS #
*** readed /fatfs/curl.txt 315 data ***
[file info]
**************************************
Curl demo task finised.
```
* 注：demo运行成功后，会将下载的文件保存到本地文件curl.txt中（具体内容参考实际情况，此处用`[file info]`代替）。
