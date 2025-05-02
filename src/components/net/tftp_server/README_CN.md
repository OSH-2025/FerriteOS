# tftp server 使用手册

`tftp`是一种基于`UDP`的简单文件传输协议，一般用于小型文件传输。

本文档以`STM32F769IDISCOVERY`开发板为例，讲解如何在`shell`中开启`tftp server`，并通过PC端的`tftp client`与作为`tftp server`端的开发板进行文件传输。

目前支持`tftp server`的开发板有以下几类： 

- STM32F769IDISCOVERY
- Cloud_STM32F429IGTx_FIRE

## 使能 tftp server

首先设置开发板`STM32F769IDISCOVERY`的默认配置，在`Linux`系统的`LiteOS`源码根目录下，执行如下命令：   
```
~/Huawei_LiteOS$ cp tools/build/config/STM32F769IDISCOVERY.config .config
```

继续在`LiteOS`源码根目录下运行`make menuconfig`命令，按如下菜单路径使能`tftp server`组件：  
```
Components  ---> Network  --->  [*] Enable Tftp Server
```
使能`tftp server`后，会自动使能其依赖的`lwIP`网络功能，及`fatfs`文件系统并设置为`SD`卡模式。

编译成功后将在`out/STM32F769IDISCOVERY/lib`目录下生成组件静态库文件`libtftp_server.a`，系统镜像文件为`Huawei_LiteOS.bin`。

## tftpServer 命令

1. 查看`tftpServer`命令是否生效：

   使用串口工具，进入`shell`，输入`help`命令后，如果显示`tftpServer`命令，则表明已经生效。   

    Huawei LiteOS # help
    *******shell commands:*********
    
    date          free          help          hwi      log           memcheck          
    
    swtmr       systeminfo    task          tftpServer       uname      watch         
    
    Huawei LiteOS # 

2. 查看`tftpServer`命令的帮助信息以获取命令用法：

   进入`shell`，输入`tftpServer -h`：
   ```
   Huawei LiteOS # tftpServer -h
   usage:
   Transfer a file from/to tftp client.
   tftpServer -s -m "file path"
       -s: start run tftp server.
       -m: mount file system to file path.
       stop: stop tftp server.
   example:
       tftpServer -s -m /fatfs 
   ```

## tftpServer 测试示例

### 测试条件

- 准备一台`PC`，环境为`Linux`或`Windows`，且已安装了`tftp`客户端功能。
- 在`PC`端开启`tftp`客户端，开启方法请在网上搜索。
- 将`PC`与作为`tftp server`的开发板通过以太网连接。
- 准备一张`SD`卡，在开发板上电前，安装到开发板的`SD`卡槽中。此`SD`卡默认挂载为`fatfs`文件系统。

### 设置`IP`地址

- 在[sys_init.c](/targets/STM32F769IDISCOVERY/Src/sys_init.c)文件的`void net_init(void)`函数中设置开发板的`IP`地址及网关地址，网关地址与路由器中设置的保持一致。   
- 后续使用的`IP`地址，`192.168.3.37`为`PC`端的`IP`地址，`192.168.3.200`为开发板的`IP`地址。   

### 测试步骤

1. 开发板端运行`tftpServer`命令启动`tftp`服务端: 
   ```
   Huawei LiteOS # tftpServer -s -m /fatfs/
   ```   

   - -s：启动`tftp server`服务
   - -m：设置开发板挂载的文件系统路径，例如 `/fatfs/`

2. 以`Windows PC`为例，在`Windows`命令行开启`tftp`客户端。

3. `PC`端执行以下命令从开发板上下载文件：
   ```
   TFTP -i 192.168.3.200 GET recvFile.txt
   ```
   上述命令表示从开发板的文件系统挂载目录下下载`recvFile.txt`文件到`PC`端。

4. `PC`端执行以下命令上传文件到开发板的文件系统挂载目录下：
   ```
   TFTP -i 192.168.3.200 PUT sendFile.txt
   ```
   上述命令表示将`sendFile.txt`文件从`PC`端上传文件到开发板的文件系统挂载目录下。
