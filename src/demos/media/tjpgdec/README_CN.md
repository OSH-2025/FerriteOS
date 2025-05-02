# Tjpgdec Demo说明文档
Tjpgdec是一个通用的 JPEG 图像解压缩模块，针对小型嵌入式系统进行了高度优化。详情参考<a href="http://elm-chan.org/fsw/tjpgd/00index.html" target="_blank">Tjpgdec</a>。

## JPEG
JPEG是最常用的图片格式。详情参考<a href="https://baike.baidu.com/item/JPEG%E6%A0%BC%E5%BC%8F/3462770?fromtitle=jpeg&fromid=213408&fr=aladdin" target="_blank">JPEG</a>。

## 使能Tjpgdec Demo

1. 在LiteOS源码根目录下根据实际使用的开发板，拷贝tools/build/config/目录下的默认配置文件${platform}.config到根目录，并重命名为.config。

    **注意** 根据`Tjpgdec`所需资源，暂时只支持在STM32F769和STM32F429开发板上运行`Tjpgdec demo`。


2. 继续在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能`Tjpgdec demo`。

    ```
    Demos  --->
        Media Demo  --->
                [*] Enable Tjpgdec Demo
    ```
使能Tjpgdec Demo后会自动使能Tjpgdec组件。

保存退出后，会从github上自动下载Tjpgdec源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。
关于在线组件的自动下载，请查阅<a href="https://gitee.com/LiteOS/LiteOS_Components#%E5%9C%A8%E7%BA%BF%E7%BB%84%E4%BB%B6%E4%B8%8B%E8%BD%BD%E6%B5%81%E7%A8%8B" target="_blank">在线组件下载流程</a> 。

**注意：** 下载需要Linux主机可以访问互联网，并安装`git`。

## 编译运行Tjpgdec Demo

1. 编译烧录
在LiteOS源码根目录下执行`make clean; make -j`命令编译LiteOS工程，编译成功后会在`out/{platform}/lib`路径下生成`tjpgdec.a`和`tjpgdec_demo.a`文件，系统镜像文件为Huawei_LiteOS.bin。

烧录操作请参考<a href="https://gitee.com/LiteOS/LiteOS/blob/master/doc/LiteOS_Quick_Start.md" target="_blank">快速入门</a>。

2. 在SD卡中存入`test.jpg`文件。插入SD卡，启动开发板后会去自动运行`Tjpgdec demo`，进行对`test.jpg`文件的解码执行结果如下：

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.1.0
build data : Aug 19 2021 09:21:36

**********************************
OsAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
File system mount success.
Tjpgdec demo start to run.
Image size is 160 x 87.
3096 bytes of work ares is used.

0%
18%
36%
55%
73%
Decompression succeeded.
Tjpgdec demo finished.

Huawei LiteOS # 
```
