# PDFGen Demo说明文档

`PDFGen`是一个pdf文档生成库。

本demo实现的功能为：生成一份名为`PDFGen_demo.pdf`的文档，文档内容为乘法表、直线、正方形、圆、椭圆、曲线等，具体内容可查看`PDFGen_demo.pdf`。本demo依赖FATFS文件系统，并且对开发板内存有一定要求，目前可在如下开发板运行：
- STM32F769IDISCOVERY


## PDFGen Demo

1. 在`LiteOS`源码根目录下根据实际使用的开发板，拷贝`tools/build/config/`目录下的默认配置文件`${platform}.config`到根目录，并重命名为`.config`。以`Linux`系统和`STM32F769IDISCOVERY/`开发板为例，在`LiteOS`源码根目录下，执行如下命令完成配置文件的拷贝和重命名：
   ```
   cp tools/build/config/STM32F769IDISCOVERY/.config .config
   ```

2. 使能`PDFGen Demo`。在`LiteOS`源码根目录下运行`make menuconfig`命令，按如下路径使能demo：
   ```
   Demos  --->
        Utility Demo  --->
               [*] Enable PDFGen Demo
   ```

   使能本demo后，会自动使能其依赖的`PDFGen`组件及`FATFS`文件系统并设置为`SD`卡模式。本Demo生成文件`PDFGen_demo.pdf`就保存在`SD`卡中。保存退出`menuconfig`后，`LiteOS`会从`github`上自动下载`PDFGen`源代码，关于自动下载的详细流程，请查阅`components/download.sh`脚本。

   **注意**：下载需要`Linux`主机可以访问互联网，并安装`git`工具。如果使用`LiteOS Studio`使能该Demo，需要在`组件配置`中手动设置`FATFS`文件系统为`SD`卡模式。

## 编译运行PDFGen Demo

操作步骤如下：

1. 将SD卡插入开发板的卡槽内。
2. 参照上述步骤使能`PDFGen Demo`。
3. 在`LiteOS`源码根目录下执行`make clean ; make -j`命令编译`LiteOS`工程，编译成功后将在`out/${platform}/lib`目录下生成`PDFGen`的库文件`libpdfgen.a`和本Demo的库文件`libpdfgen_demo.a`，系统镜像文件为`Huawei_LiteOS.bin`。以`STM32F769IDISCOVERY/`开发板为例，其目录就是`out/STM32F769IDISCOVERY/`。
4. 烧录系统镜像文件到开发板，复位后可以看到串口输出Demo运行结果如下所示。

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.1.0
build data : Jul  9 2021 11:46:40

**********************************
OsAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
File system mount success.
Pdfgen demo task start to run.
Paper size: 595.275574 * 841.889771.
Pdfgen demo task finished.

Huawei LiteOS # 
```
更多内容及示列可参考：https://github.com/AndreRenaud/PDFGen
