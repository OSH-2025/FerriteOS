# Tinycrypt Demo说明文档
Tinycrypt 是一种快速，简单的加密工具，用在文件安全的高级加密 。根据`tinycrypt`所需资源，暂时只支持在以下开发板上运行。

- STM32F429
- STM32F407
- STM32F769
- realview-pbx-a9
- qemu-virt-a53

## 解析Tinycrypt Demo

在`tinycrypt_demo.c`中主要实现通过分组加密算法模式（CBC)对字符串`g_demoPlainText`进行加解密。
## 使能Tinycrypt Demo

1.在LiteOS源码根目录下根据实际使用的开发板，拷贝tools/build/config/目录下的默认配置文件${platform}.config到根目录，并重命名为.config。

2.继续在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能Tinycrypt 组件和Tinycrypt Demo。

```
Demos  --->
    Security Demo  --->
            [*] Enable Tinycrypt Demo 
```
保存退出后，LiteOS会从github上自动下载Tinycrypt源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。
详细流程在`components/download.sh`文件中。

**注意：** 下载需要Linux主机可以访问互联网，并安装`git`。

## 编译运行Tinycrypt Demo

使能Tinycrypt Demo后，在LiteOS源码根目录下执行`make clean; make -j`命令编译LiteOS工程，编译成功后会在`out/{platform}/lib`路径下生成`tinycrypt.a`和`tinycrypt_demo.a`文件，系统镜像文件为Huawei_LiteOS.bin。

以`Cloud_STM32F429IGTx_FIRE`开发板为例来运行`Tinycrypt Demo`，其他开发板操作请参考<a href="https://gitee.com/LiteOS/LiteOS/blob/master/doc/LiteOS_Quick_Start.md" target="_blank">快速入门</a>。

LiteOS启动后会运行`Tinycrypt Demo`，执行分组加密算法（CBC）对`g_demoPlainText`进行加解密。执行结果如下：
```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
build data : May 10 2021 17:38:10

**********************************
osAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
Tinycrypt demo task start to run.
encrypted = [0123456789abcdef9bcb4cdbc8e194e8fb3febf7f661098d45af9df1f468c5c83b9bacd9df449ba]
decrypted = [hello liteOS !]
Tinycrypt demo task finished.

Huawei LiteOS #  
```
