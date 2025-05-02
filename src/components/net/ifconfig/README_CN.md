# ifconfig使用手册

`ifconfig`命令用于查询和设置网卡的IP地址、子网掩码、网关等，也可用于开启/关闭网卡的数据处理。`LiteOS`目前只支持`ifconfig`命令查询和设置`Ipv4`信息，不支持`Ipv6`。

理论上支持`lwIP`协议栈的设备均可支持`ifconfig`命令，目前已验证`ifconfig`功能的开发板有以下几类： 

- STM32F769IDISCOVERY
- Cloud_STM32F429IGTx_FIRE

## 使能ifconfig

1. 在`LiteOS`源码根目录下根据实际使用的开发板，拷贝`tools/build/config/`目录下的默认配置文件`${platform}.config`到根目录，并重命名为`.config`。

2. 继续在`LiteOS`源码根目录下执行`make menuconfig`命令，按如下菜单路径使能`ifconfig`功能。

   ```
   Components  --->
        Network  --->
            [*] Enable Ifconfig
   ```
   使能`ifconfig`后，`LiteOS`会自动使能`ifconfig`依赖的`lwIP`并设置为`Ipv4`模式。保存退出`menuconfig`后，`LiteOS`会从`github`上自动下载`lwIP`源代码，并从`gitee`的LiteOS组件仓库`LiteOS_Components`下载对应的patch包，并打入patch，关于自动下载的详细流程，请查阅`components/download.sh`脚本。

   **注意**：下载需要Linux主机可以访问互联网，并安装`git`。

## 测试

使能`ifconfig`后，在`LiteOS`源码根目录下执行`make clean ; make -j`命令编译`LiteOS`工程，编译成功后将在`out/${platform}/lib`目录下生成组件静态库文件`libifconfig.a`，系统镜像文件为`Huawei_LiteOS.bin`。烧录系统镜像文件，复位开发板，即可运行`ifconfig`命令。

1. 使用串口工具，进入`shell`， 输入`help`命令后， 如果显示`ifconfig`命令，则表明已经生效。

    ```
    Huawei LiteOS # help
    *******************shell commands:*************************

    date          free          help          hwi           ifconfig      log           memcheck      ping          
    swtmr         systeminfo    task          uname         watch         

    Huawei LiteOS #

    ```
2. 查看命令帮助信息

    在`shell`中输入`ifconfig -h`，查看`ifconfig`命令的帮助信息。
    ```
	Huawei LiteOS # ifconfig -h
	Usage:
	ifconfig [-h] 
	ifconfig <interface>
	[<ipaddr>] [netmask <mask>] [gateway <gw>]
	[mtu <size>]
	[up|down]...

	Huawei LiteOS # 
    ```

3. 获取设备网卡信息

    在`shell`中输入`ifconfig`，即可查看设备的IP地址、子网掩码、网关、硬件MAC地址等信息。

    ```
    Huawei LiteOS # ifconfig
    st0	ip:192.168.3.123 netmask:255.255.255.0 gateway:192.168.3.1
        HWaddr 00:80:e1:00:00:00 MTU:1500 Running Default Link UP
    lo0	ip:127.0.0.1 netmask:255.0.0.0 gateway:127.0.0.1
        HWaddr 00 MTU:0 Running Link UP

    Huawei LiteOS #
    ```

4. 设置网卡状态

    在`shell`中输入`ifconfig st0 down`命令，然后再输入`ifconfig`查看网卡状态，从以下打印信息可以看出设置`st0`网卡`down`之后，`st0`的状态从`Running`变为`stop`。

    ```
    Huawei LiteOS # ifconfig st0 down

    Huawei LiteOS # 
    Huawei LiteOS # ifconfig
    st0	ip:192.168.3.123 netmask:255.255.255.0 gateway:192.168.3.1
        HWaddr 00:80:e1:00:00:00 MTU:1500 Stop Link UP
    lo0	ip:127.0.0.1 netmask:255.0.0.0 gateway:127.0.0.1
        HWaddr 00 MTU:0 Running Link UP

    Huawei LiteOS # 
    ```

5. 设置`IP`地址

    在`shell`中输入`ifconfig`查看设备`IP`信息，然后输入命令`ifconfig st0 192.168.3.100`修改`st0`网卡的`IP`地址为`192.168.3.100`，设置完成后再次输入`ifconfig`查看修改的`IP`地址已经生效，如下所示：

    ```
    Huawei LiteOS # ifconfig
    st0	ip:192.168.3.123 netmask:255.255.255.0 gateway:192.168.3.1
        HWaddr 00:80:e1:ee:00:00 MTU:1500 Running Default Link UP
    lo0	ip:127.0.0.1 netmask:255.0.0.0 gateway:127.0.0.1
        HWaddr 00 MTU:0 Running Link UP


    Huawei LiteOS # ifconfig st0 192.168.3.100


    Huawei LiteOS # ifconfig
    st0	ip:192.168.3.100 netmask:255.255.255.0 gateway:192.168.3.1
        HWaddr 00:80:e1:ee:00:00 MTU:1500 Running Default Link UP
    lo0	ip:127.0.0.1 netmask:255.0.0.0 gateway:127.0.0.1
        HWaddr 00 MTU:0 Running Link UP


    Huawei LiteOS # 

    ```

6. 设置子网掩码

    在`shell`中输入`ifconfig`查看设备`IP`信息，然后输入命令`ifconfig st0 netmask 255.255.0.0`修改`st0`网卡的子网掩码为`255.255.0.0`，设置完成后再次输入`ifconfig`查看修改的子网掩码已经生效，操作如下所示：

    ```
    Huawei LiteOS # ifconfig
    st0	ip:192.168.3.123 netmask:255.255.255.0 gateway:192.168.3.1
        HWaddr 00:80:e1:ee:00:00 MTU:1500 Running Default Link UP
    lo0	ip:127.0.0.1 netmask:255.0.0.0 gateway:127.0.0.1
        HWaddr 00 MTU:0 Running Link UP


    Huawei LiteOS # ifconfig st0 netmask 255.255.0.0


    Huawei LiteOS # ifconfig
    st0	ip:192.168.3.100 netmask:255.255.0.0 gateway:192.168.3.1
        HWaddr 00:80:e1:ee:00:00 MTU:1500 Running Default Link UP
    lo0	ip:127.0.0.1 netmask:255.0.0.0 gateway:127.0.0.1
        HWaddr 00 MTU:0 Running Link UP
    ```
