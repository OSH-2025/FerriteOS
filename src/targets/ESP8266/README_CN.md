# ESP8266 使用教程

## 1. 简介

ESP8266目录包含esp8266单板相关代码及脚本。

## 2. 环境搭建

### 2.1. Linux（Windows10 + VMware Workstation 16 Pro + Ubuntu 20.04.3 LTS）

  开发平台采用 Windows10 + ubuntu 虚拟机的方式。

  **2.1.1 安装准备**
  
  ```shell
  $ cd ~
  $ sudo apt-get install git wget flex bison gperf python3 python3-pip python3-setuptools cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0
  $ python3 -m pip install --upgrade pip setuptools wheel
  $ mkdir esp8266
  $ cd esp8266
  ```
  **2.1.2 编译工具链设置**
  
  获取编译工具链并配置环境变量
  ```shell
  $ wget https://dl.espressif.com/dl/xtensa-lx106-elf-gcc8_4_0-esp-2020r3-linux-amd64.tar.gz
  $ tar -zxvf xtensa-lx106-elf-gcc8_4_0-esp-2020r3-linux-amd64.tar.gz
  $ export PATH=$PATH:$HOME/esp8266/xtensa-lx106-elf/bin
  # $HOME: 表示家目录，以自己的为准，可执行 echo $HOME 查看自己的家目录
  ```
 * 注，32位linux对应编译器网址：
      >https://dl.espressif.com/dl/xtensa-lx106-elf-gcc8_4_0-esp-2020r3-linux-i686.tar.gz

  **2.1.3 ESP8266_RTOS_SDK安装**

  获取官方SDK源码并安装要求的python包
  ```shell
  $ cd ~/esp8266
  $ git clone --recursive https://github.com/espressif/ESP8266_RTOS_SDK.git
  $ export IDF_PATH=$HOME/esp8266/ESP8266_RTOS_SDK
  $ python -m pip install --user -r $IDF_PATH/requirements.txt
  ```
  **2.1.4 编译SDK生成bootloader.bin引导文件、分区表文件**
  ```shell
  $ cd ~/esp8266
  $ cp -r $IDF_PATH/examples/get-started/hello_world .
  $ cd hello_world
  $ make all
  ```
  编译成功后，将在当前 build 目录下生成分区表文件 partitions_singleapp.bin、引导文件 bootloader.bin，具体信息可查看输出信息。
* 注，更多信息请参考官方教程：
    >https://docs.espressif.com/projects/esp8266-rtos-sdk/en/latest/get-started/linux-setup.html

### 2.2. Windows（Windows10 + HUAWEI-LiteOS-Studio V1.45.9 Beta）

  HUAWEI-LiteOS-Studio 暂未适配 ESP8266，本文档暂不做介绍。

## 3. 编译烧录LiteOS ESP8266工程

  ### 3.1. Ubuntu下的编译

  从<a href="https://gitee.com/LiteOS/LiteOS.git" target="_blank"> LiteOS 仓库 </a>拉取LiteOS源码：

  ```shell
  $ cd ~/esp8266
  $ git clone https://gitee.com/LiteOS/LiteOS.git
  ```

  设置esptool.py工具环境变量：
  ```shell
  $ export PATH=$PATH:$HOME/esp8266/ESP8266_RTOS_SDK/components/esptool_py/esptool
  ```
  
  进入 LiteOS 工程，拷贝 esp8266 默认配置文件替换当前配置文件：
  ```shell
  $ cd ~/esp8266/LiteOS
  $ cp tools/build/config/ESP8266.config .config
  ````

  编译LiteOS ESP8266工程：
  ```shell
  $ make clean; make -j
  ```
  编译完成后会在当前路径下生成文件 out/ESP8266/Huawei_LiteOS.bin，该文件即为 esp8266 的烧录文件。


  ### 3.2. windows下的编译

  HUAWEI-LiteOS-Studio 暂未适配 ESP8266，本文档暂不做介绍。


## 4. 烧录

  **4.1. 安装烧录软件**

  烧录软件下载地址：<a href="https://www.espressif.com/sites/default/files/tools/flash_download_tool_3.9.0_0.zip" target="_blank">ESP8266 FLASH DOWNLOAD TOOLS</a>，直接点击即可下载，该软件为 Windows 版本的，下载完成后解压，打开 flash_download_tool.exe。

  **4.2 配置及烧录**

  此处作简单说明，详细图文说明参考上述解压目录目录下的文档 doc/Flash_Download_Tool__cn.pdf 中的“2.下载示例”。

  - 打开软件，芯片类型 chipType 选择 ESP8266，点击 OK 进入下一步。
  - 选择 SPIDownload。
  - 在SPIDownload页面下的空白栏，按顺序填入引导文件 bootloader.bin 的路径及下载地址、分区表文件 partitions_singleapp.bin 的路径及下载地址、esp8266的烧录文件Huawei_LiteOS.bin的路径及下载地址并勾选，如下：
    > | 文件路径 | 下载地址  |
    > | ------------------------------------------------------------ | ------- |
    > | hello_world/build/bootloader/bootloader.bin                  | 0x0     |
    > | hello_world/build/partitions_singleapp.bin                   | 0x8000  |
    > | LiteOS/out/ESP3266/Huawei_LiteOS.bin                         | 0x10000 |
  - SpiFlashConfig 部分 SPI SPIEED 选择 40MHz，SPIMODE 选择 DIO，勾选 DoNoChgBin。
  - 按实际情况选择 COM，波特率 BAUD 选择 115200。
  - 点击START开始下载。

## 5. 查看运行结果

  使用串口工具查看 esp8266 的串口输出，内容如下：
  ```
  ...... 引导程序内容，此处省略
 
  ********Hello Huawei LiteOS********

  LiteOS Kernel Version : 5.1.0
  build data : Nov  1 2021 15:26:38

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

  free          help          hwi           memcheck      swtmr         systeminfo    task          uname         

  Huawei LiteOS #
  ```

  输入命令 systeminfo 查看系统信息
  ```
  Huawei LiteOS # systeminfo

   Module    Used      Total
  --------------------------------
   Task      4         16        
   Sem       0         20        
   Mutex     3         20        
   Queue     2         10        
   SwTmr     0         16        

  Huawei LiteOS #
  ```
