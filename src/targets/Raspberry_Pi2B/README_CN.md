# Raspberry Pi 使用教程

## 1.简介

Raspberry相关目录下包含Raspberry启动代码及配置文件。

## 2.环境搭建
环境搭建参考 https://gitee.com/LiteOS/LiteOS/blob/master/doc/LiteOS_Build_and_IDE.md 的"搭建Linux编译环境"目录

 * 注: 若下载最新的aarch64-none-elf编译器出现编译错误的情况，可使用低一些的版本，如 <a href="https://developer.arm.com/-/media/Files/downloads/gnu-a/10.2-2020.11/binrel/gcc-arm-10.2-2020.11-x86_64-aarch64-none-elf.tar.xz?revision=79f65c42-1a1b-43f2-acb7-a795c8427085&hash=61BBFB526E785D234C5D8718D9BA8E61" target="_blank">gcc-arm-10.2-2020.11-x86_64-aarch64-none-elf</a>版本。
	
## 3. 编译LiteOS Raspberry工程
  从<a href="https://gitee.com/LiteOS/LiteOS.git" target="_blank"> LiteOS 仓库 </a>拉取LiteOS源码：

  ```shell
  $ mkdir ~/raspberry
  $ cd ~/raspberry
  $ git clone https://gitee.com/LiteOS/LiteOS.git
  ```

  进入 LiteOS 工程，根据 Raspberry Pi 的开发板类型拷贝对应的 Raspberry 配置文件替换当前配置文件：
  
  ```shell
  $ cd ~/raspberry/LiteOS
  $ cp tools/build/config/Raspberry_Pi2B.config .config
  ```

  编译LiteOS raspberry工程：
  ```shell
  $ make clean && make -j
  ```
  编译完成后会在 out/Raspberry_Pi2B/ 路径下生成 kernel7.img 文件，该文件即为 raspberry 的烧录文件。
  * 注: 32位编译得到的镜像名为kernel7.img，64位编译得到的镜像名为kernel8.img。

## 4. 烧录

  **4.1. 安装烧录软件**
  
  烧录软件下载地址：<a href="https://www.raspberrypi.org/software/" target="_blank">Raspberry Pi Imager Download Tool</a>，打开下载地址点击 "Download for Windows"，下载完成后解压，双击 imager_XXX.exe 安装。
  
  **4.2 烧录 Raspberry 系统**
  - 准备一张空的SD卡，并将SD卡插入电脑。
  - 打开 Raspberry Pi Imager 软件，点击"Operating System"的选项框，选择RASPBERRY PI OS(32-BIT)；点击"Storage"选项框，选择需要烧写的SD卡，点击WRITE，等待烧写完成。
  
  ![Image text](https://gitee.com/wcc0/LiteOS/raw/raspberry/doc/figures/raspberry/Raspberry_Pi_imager.png)
  
  **4.3 烧录镜像**
  - 将编译得到的kernel7.img或kernel8.img，替换SD卡中的kernel7.img或kernel8.img。
  - 修改 Raspberry Pi 的配置文件：
    > - 若烧写32位，镜像为kernel7.img，在config.txt中增加kernel=kernel7.img。
	> - 若烧写64位，镜像为kernel8.img，在config.txt中增加kernel=kernel8.img。
	
## 5. 查看运行结果

  **5.1. 连接串口线**
  
  按照下图方式连接好串口线
  
  ![Image text](https://gitee.com/wcc0/LiteOS/raw/raspberry/doc/figures/raspberry/raspberrypi_serial.png)
  
  **5.2. 串口工具**
  
  用串口工具打开对应串口，重启 Raspberry Pi 即可看到如下打印信息：
  
  ```
  ********Hello Huawei LiteOS********

  LiteOS Kernel Version : 5.1.0
  build data : Oct  8 2021 17:00:07

  **********************************
  OsAppInit
  cpu 0 entering scheduler
  app init!
  Hello, welcome to liteos demo!

  Huawei LiteOS #
  ```

## 6. shell 命令

  输入命令 help 查看支持的 shell 命令
  
  ```
  Huawei LiteOS # help
  *******************shell commands:*************************

  free          help          hwi           memcheck      stack         swtmr         systeminfo    task          uname    

  Huawei LiteOS #
  ```
