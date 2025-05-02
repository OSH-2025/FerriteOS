# Libevent Demo说明文档

libevent是一个轻量级的高性能的开源事件触发库，适用于Linux、Windows、bsd等多种平台，内部使用epoll、select、kqueue等系统调用管理事件机制。目前支持在以下开发板上运行：

- STM32F429
- STM32F769


## 解析Libevent Demo

本demo通过事件触发方式创建一个服务端，用于接收来自客户端的消息。

##### 接口介绍：

```c
void event_set(struct event *ev, evutil_socket_t fd, short events, void (*callback)(evutil_socket_t, short, void *), void *arg);
```

功能：初始化struct event结构体。

```c
int event_base_set(struct event_base *base, struct event *ev);
```

功能：绑定base到ev，并设置ev的pri优先级。

```c
int event_add(struct event *ev, const struct timeval *tv);
```

功能：注册事件。

```c
int event_base_dispatch(struct event_base *event_base);
```

功能：事件处理。

```c
int event_del(struct event *ev);
```

功能：删除事件。


## 使能Libevent Demo

1. 在LiteOS源码根目录下根据实际使用的开发板，拷贝tools/build/config/目录下的默认配置文件${platform}.config到根目录，并重命名为.config。

2. 继续在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能Libevent组件和Libevent Demo。

    ```
    Demos  --->
        Utility Demo  --->
                [*] Enable Libevent Demo
    ```

保存退出后，LiteOS会从github上自动下载Libevent源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。
详细流程请参考`components/download.sh`。

**注意：** 下载需要Linux主机可以访问互联网，并安装`git`。


## 编译运行Libevent Demo

在LiteOS源码根目录下执行`make clean ; make -j`命令编译LiteOS工程，编译成功后会在`out/${platform}/lib`路径下生成`liblibevent.a`和`liblibevent_demo.a`文件，系统镜像文件为Huawei_LiteOS.bin。

**注意：** 选择不同开发板out路径会有不同。

烧录系统镜像文件到开发板后，等待客户端连接，待客户端与其连接成功后，就可以接收到来自客户端的数据。

客户端`tcp_client.py`代码：

```python
import sys
import string
import getopt
import socket


def tcp_client(host, port):
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((host, int(port, base=10)))
    while True:
        send_data = "hello LiteOS"
        client.send(send_data.encode("utf-8"))
        recv_data = client.recv(1024)
        print("recv data from server：", recv_data)


def main(argv):
    host = ""
    port = ""
    try:
        opts, args = getopt.getopt(argv, "h:p:", ["host=", "port="])
    except getopt.GetoptError:
        print('tcp_client.py -h <server ip> -p <server port>')
        sys.exit(2)
    for opt, arg in opts:
        if opt in ("-h", "host="):
            host = arg
        elif opt in ("-p", "port="):
            port = arg
    tcp_client(host, port)


if __name__ == "__main__":
    main(sys.argv[1:])
```

Linux系统执行以下操作：

```
python tcp_client.py -h ip -p port
```

Windows系统执行以下操作：

```
py.exe tcp_client.py -h ip -p port
```

结果如下：

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.1.0
build data : Oct 20 2021 17:31:12

**********************************
OsAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
lwip_init OK!!
dhcp start...
Libevents demo task star to run.
liteos no support socketpair.
liteos not support exit.

Huawei LiteOS # Establish a connection with the client success.
Receive data : hello LiteOS, size : 12
Send data : Welcome to the LiteOS community. success.
```
