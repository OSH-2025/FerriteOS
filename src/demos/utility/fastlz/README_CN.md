# Fastlz Demo说明文档

`Fastzl`是一个轻量级的快速无损压缩库。

本demo实现的功能为：新建`fastlz_source.txt`文件，写入1600字节的数据，使用`Fastlz`库压缩文件，得到大小为42字节的压缩文件`fastlz_cmprs.txt`，再解压该压缩文件，解压后的文件为`fastlz_decmprs.txt`，该文件的内容和大小均与原文件`fastlz_source.txt`相同。本demo依赖FATFS文件系统，已适配如下开发板：
- Cloud_STM32F429IGTx_FIRE
- STM32F769IDISCOVERY

## 使能Fastlz Demo

1. 在`LiteOS`源码根目录下根据实际使用的开发板，拷贝`tools/build/config/`目录下的默认配置文件`${platform}.config`到根目录，并重命名为`.config`。以`Linux`系统和`Cloud_STM32F429IGTx_FIRE`开发板为例，在`LiteOS`源码根目录下，执行如下命令完成配置文件的拷贝和重命名：
   ```
   cp tools/build/config/Cloud_STM32F429IGTx_FIRE.config .config
   ```

2. 使能`Fastlz Demo`。在`LiteOS`源码根目录下运行`make menuconfig`命令，按如下路径使能demo：
   ```
   Demos  --->
        Utility Demo  --->
               [*] Enable Fastlz Demo
   ```

   使能本demo后，会自动使能其依赖的`Fastlz`组件及`FATFS`文件系统并设置为`SD`卡模式。本Demo中的文件就保存在`SD`卡中。保存退出`menuconfig`后，`LiteOS`会从`github`上自动下载`Fastlz`源代码，并从`gitee`的LiteOS组件仓库`LiteOS_Components`下载对应的patch包，并打入patch，关于自动下载的详细流程，请查阅`components/download.sh`脚本。

   **注意**：下载需要`Linux`主机可以访问互联网，并安装`git`工具。如果使用`LiteOS Studio`使能该Demo，需要在`组件配置`中手动设置`FATFS`文件系统为`SD`卡模式。

## 编译运行Fastlz Demo

操作步骤如下：

1. 将SD卡插入开发板的卡槽内。
2. 参照上述步骤使能`Fastlz Demo`。
3. 在`LiteOS`源码根目录下执行`make clean ; make -j`命令编译`LiteOS`工程，编译成功后将在`out/${platform}/lib`目录下生成`Fastlz`的库文件`libfastlz.a`和本Demo的库文件`libfastlz_demo.a`，系统镜像文件为`Huawei_LiteOS.bin`。以`Cloud_STM32F429IGTx_FIRE`开发板为例，其目录就是`out/Cloud_STM32F429IGTx_FIRE`。
4. 烧录系统镜像文件到开发板，复位后可以看到串口输出Demo运行结果如下所示。

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
build data : May 14 2021 15:34:24

**********************************
osAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
File system mount success.
Fastlz demo task start to run.
The size of the /fatfs/fastlz_source.txt file before compression is 1600 bytes.
.
The size of the /fatfs/fastlz_source.txt file after compression is 42 bytes, compression ratio is 2%.
The size of the /fatfs/fastlz_cmprs.txt file before decompression is 42 bytes.
.
The size of the /fatfs/fastlz_cmprs.txt file after decompression is 1600 bytes.
Fastlz demo task finished.

Huawei LiteOS # 
```
