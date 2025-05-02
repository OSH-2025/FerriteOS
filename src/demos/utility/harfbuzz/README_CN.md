# Harfbuzz Demo说明文档

Harfbuzz是一个文本整形引擎，用于文字塑形，可以将`Unicode`文本转换为格式和位置正确的字形输出。本示例参考`harfbuzz`源代码中`test/api`的相关实现，由于`C++`编译和板子内存资源问题，目前只支持在以下开发板上运行。

- STM32F769IDISCOVERY

## 使能Harfbuzz Demo

1. 在`LiteOS`源码根目录下根据实际使用的开发板，拷贝`tools/build/config/`目录下的默认配置文件`${platform}.config`到根目录，并重命名为`.config`。

2. 继续在`LiteOS`源码根目录下执行`make menuconfig`命令，按如下菜单路径使能`Harfbuzz Demo`。

   ```
   Demos  --->
       Utility Demo  --->
               [*] Enable Harfbuzz Demo
   ```

   保存退出后，会自动使能Demo所依赖的`Harfbuzz`组件、`Freetype`组件以及`fatfs`文件系统并将其设置为`SD`卡模式，并会从`github`上自动下载`Harfbuzz`和`Freetype`的源代码，从`gitee`上下载适配于`LiteOS`系统的`patch`包，并将`patch`打入组件源码中。
   详细流程见`components/download.sh`文件。

   **注意：** 下载需要`Linux`主机可以访问互联网，并安装`git`。

## 编译运行Harfbuzz Demo

操作步骤如下：

1. 将`demos/utility/harfbuzz`目录中用于测试的[inkree.ttf](inkree.ttf)字体库文件，拷贝到`SD`卡的根目录`/fatfs`内。可以在`PC`端通过读卡器拷贝，或者先将`SD`卡插入开发板的板载`SD`卡槽内，然后通过`tftp server`服务传输到`SD`卡中，具体操作见[README_CN.md](/components/net/tftp_server/README_CN.md)。

2. 使能`Harfbuzz Demo`后，在`LiteOS`源码根目录下执行`make clean; make -j`命令编译`LiteOS`工程，编译成功后会在`out/${platform}/lib`路径下生成`harfbuzz.a`和`harfbuzz_demo.a`文件，系统镜像文件为`Huawei_LiteOS.bin`。

3. 烧录系统镜像文件，复位开发板，可以在串口中看到如下输出： 

   ```
   ********Hello Huawei LiteOS********
   
   LiteOS Kernel Version : 5.0.0
   build data : Apr 30 2021 17:48:33
   
   **********************************
   osAppInit
   cpu 0 entering scheduler
   app init!
   Hello, welcome to liteos demo!
   File system mount success.
   Harfbuzz demo start to run.
   opened font file : /fatfs/inkree.ttf bytes long : 40968
   cluseter 0 glyph 14 at (0,0) + (1014,0)
   cluseter 1 glyph 15 at (0,0) + (1014,0)
   cluseter 2 glyph 16 at (0,0) + (1014,0)
   cluseter 3 glyph 44 at (0,0) + (922,0)
   cluseter 4 glyph 45 at (0,0) + (995,0)
   cluseter 5 glyph 46 at (0,0) + (905,0)
   cluseter 6 glyph 24 at (0,0) + (1540,0)
   cluseter 7 glyph 25 at (0,0) + (1219,0)
   cluseter 8 glyph 26 at (0,0) + (1399,0)
   cluseter 9 glyph 5b at (0,0) + (1010,0)
   cluseter 10 glyph 5c at (0,0) + (868,0)
   cluseter 11 glyph 5d at (0,0) + (727,0)
   cluseter 12 glyph 3b at (0,0) + (1350,0)
   cluseter 13 glyph 3c at (0,0) + (1233,0)
   cluseter 14 glyph 3d at (0,0) + (1124,0)
   Harfbuzz demo finished.
   
   Huawei LiteOS #
   ```

   `Harfbuzz Demo`通过加载字体库`inkree.ttf`，得到输入的`utf-8`字符串`"123abcABCxyzXYZ"`中每个字符对应于字体库内的字形信息，包括原文中对应字符的索引`（cluseter 0）`，字形索引`（glyph 14）`，绘制字形之前在X轴及Y轴上移动的程度`（0,0）`，绘制此字形之后在X轴及Y轴上的前进幅度`（1014,0）`等信息。