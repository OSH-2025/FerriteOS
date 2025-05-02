# Openssl Demo说明文档
Openssl是一个安全套接字层密码库，详情参考<a href="https://github.com/openssl/openssl.git" target="_blank">openssl</a>。(暂不支持Huawei-LiteOS-Studio编译)

## 解析Openssl Demo

LiteOS中提供了`OpensslDemoTask`程序来演示如何使用`Openssl`。

在`openssl_demo.c`中定义了待加密字符串，用于MD5加密及DES-CBC加解密，内容为：
```c
#define MD5_ENCRYDATA         "MD5 test for liteos"
#define DESCBC_ENCRYDATA      "DES-CBC test for liteos"
```

## 使能Openssl Demo

1. 在LiteOS源码根目录下根据实际使用的开发板，拷贝tools/build/config/目录下的默认配置文件${platform}.config到根目录，并重命名为.config。

    **注意：** 根据`Openssl`所需资源，暂时只支持在以下开发板上运行`OpensslDemoTask`。
    - Cloud_STM32F429IGTx_FIRE
    - STM32F407_ATK_Explorer
    - STM32F769IDISCOVERY
    - STM32L431_BearPi
    - realview-pbx-a9

2. 继续在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能Openssl Demo。

    ```
    Demos  --->
        Security Demo  --->
                [*] Enable Openssl Demo 
    ```
    使能Openssl Demo后会自动使能`Openssl`组件。

    保存退出后，会从github上自动下载`Openssl`源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。详细流程在`components/download.sh`文件中。

    **注意：** 下载需要Linux主机可以访问互联网，并安装`git`。

## 编译运行Openssl Demo

使能Openssl Demo后，在LiteOS源码根目录下执行`make clean; make -j`命令编译LiteOS工程，编译成功后会在`out/{platform}/lib`路径下生成`libopenssl.a`和`libopenssl_demo.a`文件，系统镜像文件为Huawei_LiteOS.bin。

以`Cloud_STM32F429IGTx_FIRE`开发板为例来运行`Openssl Demo`，其他开发板操作请参考<a href="https://gitee.com/LiteOS/LiteOS/blob/master/doc/LiteOS_Quick_Start.md" target="_blank">快速入门</a>。

LiteOS启动后会运行`Openssl Demo`，执行MD5加密及DES-CBC加解密。执行结果如下：
```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
build data : Mar 17 2021 15:08:30

**********************************
osAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
Openssl demo task start to run.
Data:MD5 test for liteos
Encrypt:579ACA835BBAA22C7F8BBBFC8E11F239
Data:DES-CBC test for liteos
Encrypt:4964E09763C6ED6099562404F7FE580CF54F35FB6113D402
Decrypt:DES-CBC test for liteos
Openssl demo task finished.

Huawei LiteOS #  
```
