# Jsmn Demo说明文档

jsmn是基于C语言开发的一个JSON解析器。它以极小的资源占用而著称，适用于资源受限的嵌入式开发环境。目前支持在以下开发板上运行：

- STM32F429
- STM32F407
- STM32F769
- realview-pbx-a9
- qemu-virt-a53


## 解析Jsmn Demo

本demo主要实现对下面json字符串进行逐个字符解析。

```json
"{\"name\": \"lanche\", \"sex\": male, \"age\": 26,\n  "
"\"hobbies\": [\"draw\", \"play games\", \"travles\", \"runing\"]}"
```


## 使能Jsmn Demo

1. 在LiteOS源码根目录下根据实际使用的开发板，拷贝tools/build/config/目录下的默认配置文件${platform}.config到根目录，并重命名为.config。

2. 继续在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能jsmn组件和Jsmn Demo。

    ```
    Demos  --->
        Utility Demo  --->
                [*] Enable Jsmn Demo
    ```

保存退出后，LiteOS会从github上自动下载jsmn源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。
详细流程在`components/download.sh`文件中。

**注意：** 下载需要Linux主机可以访问互联网，并安装`git`。


## 编译运行Jsmn Demo

在LiteOS源码根目录下执行`make clean ; make -j`命令编译LiteOS工程，编译成功后会在`out/${platform}/lib`路径下生成`libjsmn.a`和`libjsmn_demo.a`文件，系统镜像文件为Huawei_LiteOS.bin。

**注意：** 选择不同开发板out路径会有不同。

烧录系统镜像文件到开发板中，可以看到如下解析内容。

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
build data : May 17 2021 17:08:48

**********************************
osAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
Jsmn demo task start to run.
- name: lanche
- sex: male
- age: 26
- hobbies:
  * draw
  * play games
  * travles
  * runing
Jsmn demo task finished.

Huawei LiteOS #
```
