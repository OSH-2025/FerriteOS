# Http Client Demo说明文档

超文本传输协议（Hyper Text Transfer Protocol，HTTP）是最常用的应用层协议，通常运行在TCP上。详情参考<a href="https://baike.baidu.com/item/HTTP/243074?fr=aladdin" target="_blank">HTTP</a>。


## Http parser
Http-parser是用C语言编写的http解析器。详情参考<a href="https://github.com/nodejs/http-parser" target="_blank">http-parser</a>。
Http Client Demo中使用http-parser组件来解析http response header。

## 使能Http Client Demo

1. 在LiteOS源码根目录下根据实际使用的开发板，拷贝tools/build/config/目录下的默认配置文件${platform}.config到根目录，并重命名为.config。

    **注意** 根据`Http-parser`所需资源，暂时只支持在STM32F769和STM32F429开发板上运行`http client demo`。


2. 继续在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能Http Client Demo。

    ```
    Demos  --->
        Http Client Demo  --->
                [*] Enable Http Client Demo
    ```
使能Http Client Demo后会自动使能http-parser组件和LWIP。

保存退出后，会从github上自动下载http-parser源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。
详细流程在`components/download.sh`文件中。

**注意：** 下载需要Linux主机可以访问互联网，并安装`git`。

## 编译运行Http Client Demo

### http服务端配置

使用python内置的http.server即可快速搭建http服务端。
执行如下命令：
```
python -m http.server 80
```
将`demos/http_client/index.html`文件放入启动http server的文件夹中。

### 客户端配置
Http Client Demo需要网络连接，在开发板插入网线，并确保网络连接。
- 客户端操作步骤如下：
1. 修改LiteOS/demos/http_client/httpc_demo.c中的`DEFAULT_HOST`为服务端IP或域名。
    ```c
    #define TASK_PRIORITY           7
    #define TASK_STACK_SIZE         0x2000
    #define DEFAULT_HOST            "your.host.address"
    #define DEFAULT_PORT            80
    #define DEFAULT_URL             "/index.html"
    #define HTTP_CLIENT_WAIT_TIME   20000
    ```
    **注意：**  改为域名需要开启LWIP_DHCP(DNS需要DHCP来初始化，在Linux下编译，使能LWIP会默认开启DHCP)。


2. 编译烧录
在LiteOS源码根目录下执行`make clean; make -j`命令编译LiteOS工程，编译成功后会在`out/{platform}/lib`路径下生成`httpc_demo.a`和`http_parser.a`文件，系统镜像文件为Huawei_LiteOS.bin。

烧录操作请参考<a href="https://gitee.com/LiteOS/LiteOS/blob/master/doc/LiteOS_Quick_Start.md" target="_blank">快速入门</a>。

启动LiteOS后会先初始化网络，开启DHCP，然后运行`Http Client Demo`，通过`http get`方法请求`/index.html`，服务端接收到请求后会返回`/index.html`，并且会执行`HttpParse`函数分析HTTP的请求和响应，`Http Client Demo`运行结果如下:
```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.1.0
build data : Jul 22 2021 14:59:39

**********************************
OsAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
lwip_init OK!!
dhcp start...
Http client demo start to run.

Huawei LiteOS # 
***MESSAGE BEGIN***

Url: /index.html
Header field: User-Agent
Header value: lwIP/2.1.2 (http://savannah.nongnu.org/projects/lwip)
Header field: Accept
Header value: */*
Header field: Host
Header value: 192.168.10.142
Header field: Connection
Header value: Close

***HEADERS COMPLETE***


***MESSAGE COMPLETE***

data received after completed connection: close message (HPE_CLOSED_CONNECTION)

***MESSAGE BEGIN***

Header field: Server
Header value: SimpleHTTP/0.6 Python/3.9.6
Header field: Date
Header value: Thu, 22 Jul 2021 07:03:50 GMT
Header field: Content-type
Header value: text/html
Header field: Content-Length
Header value: 215
Header field: Last-Modified
Header value: Tue, 20 Jul 2021 02:23:51 GMT

***HEADERS COMPLETE***

Http receive:
<html lang="en">

<head>

    <meta charset="UTF-8">

    <meta name="viewport" content="width=device-width, initial-scale=1.0">

    <title>Huawei LiteOS</title>

</head>

<body>

    Hello LiteOS

</body>

</html>
Http receive finished.
Http client demo finished.
```
