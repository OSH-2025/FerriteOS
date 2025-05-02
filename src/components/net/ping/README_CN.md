# ping使用手册

`ping`是用于检测网络是否通畅或者网络连接速度的命令，时延越小网络速度越快，反之时延越大速度越慢。

理论上支持`lwIP`协议栈的设备均可支持`ping`命令，目前已验证`ping`功能的开发板有以下几类：

- STM32F769IDISCOVERY
- Cloud_STM32F429IGTx_FIRE

## ping命令格式

[ping <IP地址>]

## 使能ping

1. 设置`LiteOS`源码根目录下根据实际使用的开发板，拷贝`tools/build/config/`目录下的默认配置文件`${platform}.config`到根目录，并重命名为`.config`。

2. 继续在`LiteOS`源码根目录下执行`make menuconfig`命令，按如下菜单路径使能`ping`功能。

   ```
   Components  --->
        Network  --->
            [*] Enable Ping
   ```

    使能`ping`后，`LiteOS`会自动使能`ping`依赖的`lwIP`并设置为`Ipv4`模式。保存退出`menuconfig`后，`LiteOS`会从`github`上自动下载`lwIP`源代码，并从`gitee`的LiteOS组件仓库`LiteOS_Components`下载对应的patch包，并打入patch，关于自动下载的详细流程，请查阅`components/download.sh`脚本。

   **注意**：下载需要Linux主机可以访问互联网，并安装`git`。

## 测试

1. 准备一台PC，`Linux`或`Windows`。
2. 将PC与开发板连接到同一局域网内。
3. 将开发版串口和PC连接并打开串口监视软件（测试PC的`IP`地址为`192.168.3.37`）。
4. 使能`ping`后，在`LiteOS`源码根目录下执行`make clean ; make -j`命令编译`LiteOS`工程，编译成功后将在`out/${platform}/lib`目录下生成组件静态库文件`libping.a`，系统镜像文件为`Huawei_LiteOS.bin`。
5. 烧录系统镜像文件，复位开发板。
6. 进入`shell`，输入`help`命令后，如果显示`ping`命令，则表明已经生效。

    ```
    Huawei LiteOS # help
    *******************shell commands:*************************

    date          free          help          hwi           ifconfig      log           memcheck      ping          
    swtmr         systeminfo    task          uname         watch         

    Huawei LiteOS #

    ```

7. 测试网络

    在`shell`中输入`ping 192.168.3.37`命令，如下所示则说明网络畅通。

    ```
    Huawei LiteOS # ping 192.168.3.37
    Ping 4 packets start.

    Huawei LiteOS # 
    Huawei LiteOS # [0]Reply from 192.168.3.37: time=2ms TTL=128.
    [1]Reply from 192.168.3.37: time=0ms TTL=128.
    [2]Reply from 192.168.3.37: time=1ms TTL=128.
    [3]Reply from 192.168.3.37: time=1ms TTL=128.

    ```

    如果出现请求超时，如下所示则说明网络故障。

    ```
    Huawei LiteOS # ping 192.168.3.37
    Ping 4 packets start.

    Huawei LiteOS # Ping : request timed out.
    Ping : request timed out.
    Ping : request timed out.
    Ping : request timed out.
    ```
