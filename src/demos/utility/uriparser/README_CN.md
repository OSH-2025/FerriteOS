# Uriparser Demo说明文档
## URI
统一资源标识符（Uniform Resource Identifier，URI)是一个用于标识某一互联网资源名称的字符串。详情参考<a href="https://baike.baidu.com/item/URI/2901761?fr=aladdin" target="_blank">URI</a>。

## Uriparser
Uriparser是用C语言编写的uri解析器。详情参考<a href="https://github.com/uriparser/uriparser" target="_blank">uriparser</a>。

## 使能Uriparser Demo

1. 在LiteOS源码根目录下根据实际使用的开发板，拷贝tools/build/config/目录下的默认配置文件${platform}.config到根目录，并重命名为.config。

2. 继续在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能Uriparser Demo。

    ```
    Demos --->
        Utility Demo --->
                [*] Enable Uriparser Demo
    ```
使能Uriparser Demo后会自动使能Uriparser组件和LWIP。

保存退出后，会从github上自动下载Uriparser源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。
详细流程在`components/download.sh`文件中。

**注意：** 下载需要Linux主机可以访问互联网，并安装`git`。

## 编译运行Uriparser Demo

在LiteOS源码根目录下执行`make clean; make -j`命令编译LiteOS工程，编译成功后会在`out/{platform}/lib`路径下生成`uriparser.a`和`uriparser_demo.a`文件，系统镜像文件为Huawei_LiteOS.bin。

烧录操作请参考<a href="https://gitee.com/LiteOS/LiteOS/blob/master/doc/LiteOS_Quick_Start.md" target="_blank">快速入门</a>。

Uriparser Demo执行结果如下：
```

********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.1.0
build data : Jul 27 2021 17:18:24

**********************************
OsAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
lwip_init OK!!
dhcp start...
Uriparser demo start to run.
uri:          https://www.huawei.com/minisite/liteos/cn/index.html
scheme:       https
hostText:     www.huawei.com
 .. pathSeg:  minisite
 .. pathSeg:  liteos
 .. pathSeg:  cn
 .. pathSeg:  index.html
absolutePath: false
              (always false for URIs with host)

Uriparser demo finished.

Huawei LiteOS # 
```