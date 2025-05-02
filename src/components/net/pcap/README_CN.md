# libpcap使用手册

  `libpcap`是一个网络数据包捕获函数库，通过采集网络通信中的数据，调试网络通信问题。`libpcap`可移植到嵌入式设备应用程序中。

  目前`libpcap`只支持运行在`STM32F769IDISCOVERY`开发板上，该开发板拥有512KB数据SRAM。`libpcap`也可以移植到其他内存资源丰富的开发板。
  
  `LiteOS`通过调用`libpcap`提供的函数接口，提供了一个抓取网络数据包的`shell`命令`tcpdump`。本文档以`STM32F769IDISCOVERY`开发板为例，介绍如何通过`tcpdump`命令在设备端抓包。

  ## 使能libpcap

  首先设置`STM32F769IDISCOVERY`的默认配置，在Linux系统的LiteOS源码根目录下，执行如下命令：   

  ```
  ~/Huawei_LiteOS$ cp tools/build/config/STM32F769IDISCOVERY.config .config
  ```

  继续在LiteOS源码根目录下运行`make menuconfig`命令，按如下菜单路径使能`pcap`组件：  

  ```
  Components  ---> Network  --->  [*] Enable Pcap
  ```

  开启`pcap`组件后，同时会自动使能`lwIP`网络，`fatfs`文件系统，及`ifconfig`命令。

  保存退出后，在LiteOS源码根目录下执行`make clean ; make -j`命令编译LiteOS工程，LiteOS会从github自动下载`libpcap`源代码，并从gitee LiteOS组件仓库里下载对应的patch包，并打入patch，关于自动下载的详细流程，请查阅`components/download.sh`脚本。  

 编译成功后将在`out/STM32F769IDISCOVERY/lib`目录下生成组件静态库文件`libpcap.a`，系统镜像文件为`Huawei_LiteOS.bin`。

  **注意**：下载需要Linux主机可以访问互联网，并安装`git`。

  ## tcpdump 命令

  1. 查看`tcpdump`命令是否生效：

     按上述方法使能`libpcap`后，`tcpdump`命令会被编译进系统镜像文件中。使用串口工具，进入shell，输入help命令后，如果显示`tcpdump`命令，则表明已经生效。   

     ```
     Huawei LiteOS # help
     *******shell commands:*********
     
     date          free          help          hwi      log           memcheck          
     
     swtmr       systeminfo    task          tcpdump    ifconfig     uname      watch         
     
     Huawei LiteOS # 
     ```

  2. 查看`tcpdump`命令的使用帮助信息：

     进入shell， 输入`tcpdump -h`

     ```
     Huawei LiteOS # tcpdump -h
     USAGE:
              tcpdump -i ifname -w "path" [-c "package-count"] ["filter expression"]
              example1: tcpdump -i st0 -w /ramfs/cap -c 15
              example2: tcpdump -i st0 -w /ramfs/cap -c 15 "arp or ip"
              example3: tcpdump stop
     
     Huawei LiteOS # 
     ```

  3. 获取设备网卡名称

     进入shell, 输入`ifconfig`

     ```
     Huawei LiteOS # ifconfig
     st0	ip:192.168.3.120 netmask:255.255.255.0 gateway:192.168.3.1
         HWaddr 00:80:e1:aa:00:00 MTU:1500 Running Default Link UP
     
     Huawei LiteOS # 
     ```

     `ifconfig`命令会依次显示设备的当前网卡名，设备ip地址`ip`，子网掩码`netmask`，网关`gateway`等信息。

 ## tcpdump 测试步骤

  `tcpdump`命令的参数说明，请参考`tcpdump -h`的帮助描述。   

  下面演示设备端的抓包步骤：

  - 准备一台PC，环境为`Linux`或`Windows`，且已安装`wireshark`软件。
  - 将PC与开发板通过以太网连接。
  - 准备一张`SD`卡，开发板上电前，安装到开发板的`SD`卡槽中，获取到的数据包将会保存在该`SD`卡中。

  1. 执行抓包命令

     在设备端进入shell, 执行 `tcpdump -i st0 -w /fatfs/cap.pcap -c 30`

     - -i  设置抓包所使用的网卡，例如 `st0`
     - -w 保存抓包数据的文件名，需要设置为全路径，例如 `/fatfs/cap.pcap`，**注意**当前`SD`卡默认挂载在`/fatfs`目录下，所以文件必须保存在此目录下。
     - -c 设置抓取数据的总包数

     当shell界面中显示`Tcpdump file saved.`时，则表明已经抓取到指定数量的网络数据。

  2. 查看抓包数据

     本文档使用`wireshark`软件查看抓包数据。在查看数据之前，需要先从开发板上下载抓包文件到PC上。以`Windows PC`为例，运行设备端的`tftpServer`功能，在`Windows`端运行`tftp client`服务，下载抓包文件。关于`tftpServer`的具体用法，请参考[tftp_server/README_CN.md](../../tftp_server/README_CN.md)文档。   