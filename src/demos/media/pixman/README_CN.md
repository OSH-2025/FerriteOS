# Pixman Demo说明文档
Pixman是一个提供低级像素操作的库，具有图像合成图形变化以及光栅化等功能。

本Demo实现的功能：新建一个宽高为`20*20`的像素图片，将目标图片划分为4个`10*10`的区域。第一个区域填充蓝色，第二个区域填充蓝粉渐变色，第三个区域填充径向渐变色，第四个区域填充圆形渐变色，输出图形到`pixman.png`。

## Demo实现的环境要求

本demo需要FATFS文件系统和LIBPNG支持，可运行于已支持该文件系统的开发板上，目前已适配如下开发板：

- STM32F769IDISCOVERY

## 使能Pixman Demo

1. 拷贝并重命名配置文件

   在`LiteOS`源码根目录下根据实际使用的开发板，拷贝`tools/build/config/`目录下的默认配置文件`${platform}.config`到根目录，并重命名为`.config`。以`Linux`系统和`STM32F769IDISCOVERY`开发板为例，在`LiteOS`源码目录下，执行如下命令完成配置文件的拷贝和重命名：

   ```c
   cp tools/build/config/STM32F769IDISCOVERY.config .config
   ```

   

2. 使能Pixman Demo

   在LiteOS源码根目录下运行make menuconfig命令，按如下菜单路径使能Pixman Demo：

   ```
   Demos    --->
           Media Demo    --->
                   [*] Enable Pixman Demo (NEW)
   ```

   使能Pixman Demo后，会自动使能其依赖的Pixman组件，Libpng组件及FATFS文件系统并设置SD卡模式。本Demo中的文件就保存在SD卡中。保存退出menuconfig后，LiteOS会从github上自动下载Pixman源代码，并从gitee的LiteOS组件仓库`LiteOS_Components`下载对应的patch包，并打入patch，关于自动下载的详细流程，请查阅`components/download.sh`脚本。

   **注意**：下载需要`Linux`主机可以访问互联网，并安装`git`工具。如果使用`LiteOS Studio`使能该Demo，需要在`组件配置`中手动设置`FATFS`文件系统为`SD`卡模式。

## 编译运行Pixman Demo

操作步骤如下：

1. 将SD卡插入开发板的卡槽内。

2. 参照上述步骤使能`Pixman Demo`。

3. 在`LiteOS`源码目录下执行`make clean ; make -j`命令编译`LiteOS`工程，编译成功后将在`out/${platform}/lib`目录下生成`Pixman`的库文件`libpixman.a`，`Libpng`的库文件`libpng.a`和本Demo的库文件`libpixman_demo.a`，系统镜像文件为`Huawei_LiteOS.bin`。以`STM32F769IDISCOVERY`开发板为例，其目录就是`out/STM32F769IDISCOVERY`。

4. 烧录系统镜像文件到开发板，复位后可以看到串口输出Demo运行结果如下所示。

   ```
   ********Hello Huawei LiteOS********
   
   LiteOS Kernel Version : 5.0.0
   build data : May 14 2021 16:05:30
   
   **********************************
   osAppInit
   cpu 0 entering scheduler
   app init!
   Hello, welcome to liteos demo!
   File system mount success.
   Pixman demo task start to run.
   Pixman demo task finished.
   
   Huawei LiteOS # 
   ```

   

5. 查看SD卡根目录，可以看到生成的图片文件为`pixman.png`。
