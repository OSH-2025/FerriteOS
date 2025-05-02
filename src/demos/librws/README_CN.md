# Librws Demo说明文档
Librws是一个跨平台的websocket客户端，它用C语言编写。详情参考<a href="https://github.com/OlehKulykov/librws" target="_blank">librws</a>。

## Websocket
WebSocket是一种在单个TCP连接上进行全双工通信的协议。详情参考<a href="https://baike.baidu.com/item/WebSocket/1953845?fr=aladdin" target="_blank">websocket</a>。

## 使能Librws Demo

1. 在LiteOS源码根目录下根据实际使用的开发板，拷贝tools/build/config/目录下的默认配置文件${platform}.config到根目录，并重命名为.config。

    **注意** 根据`Librws`所需资源，暂时只支持在STM32F769开发板上运行`LibrwsDemo`。


2. 继续在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能Librws Demo。

    ```
    Demos  --->
        Librws Demo  --->
                [*] Enable Librws Demo
    ```
使能Librws Demo后会自动使能Librws组件和LWIP。

保存退出后，会从github上自动下载Librws源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。
详细流程在`components/download.sh`文件中。

**注意：** 下载需要Linux主机可以访问互联网，并安装`git`。

## 编译运行Librws Demo

### 服务端配置

服务端代码路径为LiteOS/demos/librws/websocketserver.py，需要python3环境可运行。

- 启动服务端步骤如下(命令行操作)：

1. 修改变量的`ip`为服务器ip地址。
    ```python
    # creat websockets server， please change ip as your ip address.
    ip = ''
    startServer = websockets.serve(MainLogic, ip, 8000)
    ```
2. 通过python pip命令安装websocket包。
    ```
    pip install websockets
    ``` 
3. 运行websocketserver.py脚本。
    ```
    python websocketserver.py
    ```

### 客户端配置
Librws Demo需要网络连接，在STM32F769开发板插入网线，并确保网络连接。
- 客户端操作步骤如下：
1. 修改LiteOS/demos/librws/librws_demo.c中的`WEBSOCKET_DEMO_SERVER`为服务端IP或域名。
    ```c
    #define TEXT_BUFF_LEN                   1024
    #define BIN_BUFF_LEN                    1024
    #define WEBSOCKET_DEMO_SERVER           "your.ip.address"
    #define WEBSOCKET_DEMO_PORT             8000
    #define LIBRWS_TASK_PRIORITY            6
    #define LIBRWS_TASK_STACK_SIZE          0x1400
    ```
    **注意：**  改为域名需要开启LWIP_DHCP(DNS需要DHCP来初始化，在Linux下编译，使能LWIP会默认开启DHCP)。

2. 修改Kernel中默认任务栈大小。在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径设置，修改为4096。
    ```
    Kernel  --->
        Basic Config  --->
            Task  --->
                (4096) Task Defalut Stack Size
    ```

3. 编译烧录
在LiteOS源码根目录下执行`make clean; make -j`命令编译LiteOS工程，编译成功后会在`out/{platform}/lib`路径下生成`liblibrws.a`和`liblibrws_demo.a`文件，系统镜像文件为Huawei_LiteOS.bin。

烧录操作请参考<a href="https://gitee.com/LiteOS/LiteOS/blob/master/doc/LiteOS_Quick_Start.md" target="_blank">快速入门</a>。

启动LiteOS后会先初始化网络，开启DHCP，然后运行`Librws Demo`，建立websocket连接并向服务端发送数据，服务端接收到数据后会返回数据，运行结果如下:
```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.1.0
build data : Jul 14 2021 17:15:39

**********************************
OsAppInit
cpu 0 entering scheduler
app init!
Hello， welcome to liteos demo!
lwip_init OK!!
dhcp start...
Librws demo start to run.

Huawei LiteOS # 
Websocket connected
Websocket receive text:
your submit context: LiteOS Websocket demo data
```