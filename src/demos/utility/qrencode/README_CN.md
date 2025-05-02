# Qrencode Demo说明文档
Qrencode是一个C语言编写的用来生成二维码的库，生成的二维码可以通过二维码识别设备扫描出结果。该二维码库可接受字符串或数据块列表，稳定性较强。

本Demo实现的功能：输入字符串信息，将字符串信息生成为`37*37`的二维码图片`liteos.bmp`。

## Demo实现的环境要求

本demo需要FATFS文件系统支持，可运行于已支持该文件系统的开发板上，目前已适配如下开发板：

- Cloud_STM32F429IGTx_FIRE
- STM32F769IDISCOVERY

## 使能Qrencode Demo

1. 拷贝并重命名配置文件

   在`LiteOS`源码根目录下根据实际使用的开发板，拷贝`tools/build/config/`目录下的默认配置文件`${platform}.config`到根目录，并重命名为`.config`。以`Linux`系统和`STM32F769IDISCOVERY`开发板为例，在`LiteOS`源码目录下，执行如下命令完成配置文件的拷贝和重命名：

   ```c
   cp tools/build/config/STM32F769IDISCOVERY.config .config
   ```

2. 使能Qrencode Demo

   在LiteOS源码根目录下运行make menuconfig命令，按如下菜单路径使能Qrencode Demo：

   ```
   Demos    --->
           Media Demo    --->
                   [*] Enable Qrencode Demo (NEW)
   ```

   使能Qrencode Demo后，会自动使能其依赖的Qrencode组件及FATFS文件系统并设置SD卡模式。本Demo中的文件就保存在SD卡中。保存退出menuconfig后，LiteOS会从github上自动下载Qrencode源代码，并从gitee的LiteOS组件仓库`LiteOS_Components`下载对应的patch包，并打入patch，关于自动下载的详细流程，请查阅`components/download.sh`脚本。

   **注意**：下载需要`Linux`主机可以访问互联网，并安装`git`工具。如果使用`LiteOS Studio`使能该Demo，需要在`组件配置`中手动设置`FATFS`文件系统为`SD`卡模式。

## 编译运行Qrencode Demo

操作步骤如下：

1. 将SD卡插入开发板的卡槽内。

2. 参照上述步骤使能`Qrencode Demo`。

3. 在`LiteOS`源码目录下执行`make clean ; make -j`命令编译`LiteOS`工程，编译成功后将在`out/${platform}/lib`目录下生成`Qrencode`的库文件`libqrencode.a`，`Qrencode`的库文件`libpng.a`和本Demo的库文件`libqrencode_demo.a`，系统镜像文件为`Huawei_LiteOS.bin`。以`STM32F769IDISCOVERY`开发板为例，其目录就是`out/STM32F769IDISCOVERY`。

4. 烧录系统镜像文件到开发板，复位后可以看到串口输出Demo运行结果如下所示。

   ```
   ********Hello Huawei LiteOS********
   
   LiteOS Kernel Version : 5.1.0
   build data : Jun  1 2021 17:17:33
   
   **********************************
   OsAppInit
   cpu 0 entering scheduler
   app init!
   Hello, welcome to liteos demo!
   File system mount success.
   Qrencode demo task start to run.
   Qrencode demo task ok.
   
   Huawei LiteOS # 
   ```

5. 从开发板取下SD卡，然后将SD卡插入手机或电脑等，即可查看生成的二维码图片`liteos.bmp`，该图片位于SD卡的根目录下。
