# UPnP 使用教程

## 基本概念

`UPnP（Universal Plug and Play）`是通用即插即用的缩写，是主用于设备智能互联互通的通讯协议。`UPnP`技术是一种架构在`TCP/IP`和`HTTP`之上的分布式开放的网络结构，可以实现已联网设备间的命令及数据的传递。

`UPnP`可以运行在大部分操作系统平台上，而且使用`UPnP`协议不需要设备驱动程序，所以可以方便地在办公室、家庭和其他公共场所构建设备的互联互通。


## 常见使用场景

1. 智能家居
2. 数字音频文件管理
3. 数字图片库

## UPnP设备构成

`UPnP`定义了设备之间、设备和控制点之间、控制点之间的通讯协议。完整的`UPnP`由设备寻址、设备发现、设备描述、设备控制、事件通知和基于`Html`的描述界面构成。一个完整的`UPnP`设备需要一定的硬件设备内存。目前已测试支持运行`UPnP`服务的开发板只有`STM32F769IDISCOVERY`，该开发板拥有`2Mbyte`的`Flash`及`512KB`的`SRAM`。

## 示例介绍

此`Demo`完成了`UPnP SDK`初始化、注册一个`client`设备并完成一次搜索。该`Demo`在`STM32F769IDISCOVERY`开发板上编译运行的流程如下：

### 编译

1. 在`LiteOS`源码根目录下拷贝开发板的默认配置文件`tools/build/config/STM32F769IDISCOVERY.config`到根目录，并重命名为`.config`。

2. 继续在`LiteOS`源码根目录下执行`make menuconfig`命令，按如下菜单路径使能`UPnP Demo`。因为`UPnP`依赖`lwIP`，而且`LiteOS`目前只支持`UPnP`使用`Ipv4`，不支持`Ipv6`，所以还需要同时使能`lwIP`并设置为`Ipv4`模式。

   ```
   Components  --->
        Network  --->
            [*] Enable Lwip  --->
                        Enable Ipv4 Or Ipv6 (Enable Ipv4)  --->
                            [*] Enable Ipv4
    
    Demos  --->
        Media Demo  --->
                [*] Enable UPnP Demo
   ```

   使能`UPnP Demo`后，`LiteOS`会自动使能`UPnP`组件。保存退出`menuconfig`后，`LiteOS`会从`github`上自动下载`UPnP`和`lwIP`的源代码，并从`gitee`的LiteOS组件仓库`LiteOS_Components`下载对应的patch包，并打入patch，关于自动下载的详细流程，请查阅`components/download.sh`脚本。

   **注意**：下载需要Linux主机可以访问互联网，并安装`git`。

### 运行

1. 准备一台PC， `Linux`或`Windows`，确保已安装并打开`wireshark`抓包工具。
2. 将PC与开发板通过以太网连接。
3. 将开发版串口和PC连接并打开串口监视软件。
4. 使能`UPnP Demo`后，在`LiteOS`源码根目录下执行`make clean ; make -j`命令编译`LiteOS`工程，编译成功后将在`out/STM32F769IDISCOVERY/lib`目录下生成组件静态库文件`libupnp.a`及`Demo`静态库文件`libupnp_demo.a`，系统镜像文件为`Huawei_LiteOS.bin`。
5. 烧录系统镜像文件，复位开发板。

    开发板启动后，可以在串口上看到如下输出结果：
    ```
    ********Hello Huawei LiteOS********

    LiteOS Kernel Version : 5.0.0
    build data : May  6 2021 10:58:40

    **********************************
    osAppInit
    cpu 0 entering scheduler
    app init!
    Hello, welcome to liteos demo!
    lwip test init ok.

    Huawei LiteOS # 
    Upnp demo task start to run.
    Upnp demo task finished.
    ```
    显示`Upnp test end`表示`UPnP`控制点已经注册成功并完成了一次搜索请求，通过`wireshark`抓包工具可监测到多播`M-SEARCH SSDP（Simple Service Discovery Protocol）`消息，广播地址为`239.255.255.250`。
