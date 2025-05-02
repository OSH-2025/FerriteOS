# ESP32 使用教程

## 1. 简介

ESP32目录包含esp32单板相关代码及脚本。

## 2. 环境搭建

### 2.1. Linux（Windows10 + VMware Workstation 16 Pro + Ubuntu 20.04.3 LTS）

  开发平台采用 Windows10 + ubuntu 虚拟机的方式。

  **2.1.1. 安装 esp-idf**

  安装准备：
  ```shell
  $ cd ~
  $ sudo apt-get install git wget flex bison gperf python3 python3-pip python3-setuptools cmake ninja-build ccache libffi-dev libssl-dev dfu-util libusb-1.0-0
  $ python3 -m pip install --upgrade pip setuptools wheel
  $ mkdir esp32
  $ cd esp32
  ```

  esp-idf 安装：
  ```shell
  $ git clone -b v4.3.1 --recursive https://github.com/espressif/esp-idf.git
  $ cd esp-idf
  $ export IDF_GITHUB_ASSETS="dl.espressif.com/github_assets"
  $ ./install.sh
  ```

 * 注，关于下载过程中的部分问题说明：
    > 上述操作下载的软件源大部分位于国外，国内下载速度非常慢，大概率会下载失败，下载失败可重复执行，也可替换为如下操作：
      ```shell
      $ git clone https://gitee.com/EspressifSystems/esp-gitee-tools.git
      $ git clone -b v4.3.1 https://gitee.com/EspressifSystems/esp-idf.git
      $ cd ~/esp32/esp-gitee-tools/
      $ export EGT_PATH=$(pwd)
      $ cd ~/esp32/esp-idf
      $ $EGT_PATH/submodule-update.sh
      $ pip config set global.index-url http://mirrors.aliyun.com/pypi/simple
      $ pip config set global.trusted-host mirrors.aliyun.com
      $ $EGT_PATH/install.sh
      ```
    > - 执行 $EGT_PATH/submodule-update.sh 时，如果仓库中的 submodules 没有增加或减少，可使用 `git submodule update --init --recursive` 来更新。
    > - install.sh 安装过程中，会使用 pip 安装所需的包，默认情况下 pip 使用的是国外的软件源，使得其安装过程非常慢，为解决该问题，可切换pip 的源。此处设置为阿里的源，其他的源可参考 <a href="https://cloud.tencent.com/developer/article/1601851" target="_blank">pip 源配置</a>。
    > - 在执行`./install.sh`时，可能因 python、pip 版本过低导致失败，可将其更新到 python3.8、pip 21.2.4。此外，在 `install.sh` 运行过程中，下载virtualenv时，失败的概率较大，可执行 `pip3 install virtualenv` 单独进行安装，再重新执行 `./install.sh`。
    > - 关于安装过程的详细说明，可参考ESP32官方文档：<a href="https://docs.espressif.com/projects/esp-idf/zh_CN/v4.3.1/esp32/get-started/index.html" target="_blank">ESP-IDF编程指南 - 快速入门</a>。
    > - 用户如遇到其他问题，可通过向<a href="https://gitee.com/LiteOS/LiteOS/issues" target="_blank">LiteOS开源仓库</a>提issues、上网查询、查看ESP32官方文档等方式解决。



  **2.1.2. install.sh 成功后，执行以下命令设置临时环境变量**

  install.sh 执行完毕后会出现提示，根据提示执行如下操作：
  ```shell
  $ . ./export.sh
  ```



  **2.1.3. 编译工具链安装，建议安装8.4.0版本（已验证）。一般来说，在成功执行`install.sh`后，所需编译工具xtensa-esp32-elf-gcc、esptoos.py已经安装完成。执行`export.sh`后，会为编译工具链设置临时环境变量，可通过执行如下命令查看安装位置及版本**

  查看xtensa-esp32-elf-gcc安装位置及版本
  ```shell
  $ which xtensa-esp32-elf-gcc
  $HOME/.espressif/tools/xtensa-esp32-elf/esp-2021r1-8.4.0/xtensa-esp32-elf/bin/xtensa-esp32-elf-gcc
  # $HOME: 表示家目录，以自己的为准，可执行 echo $HOME 查看自己的家目录
  $ xtensa-esp32-elf-gcc -dumpversion  # 也可执行 xtensa-esp32-elf-gcc -v
  8.4.0                                # 8.4.0版本已验证，其余版本可能编译失败
  ```
  查看esptool.py安装位置及版本
  ```shell
  $ which esptool.py
  $HOME/esp32/esp-idf/components/esptool_py/esptool/esptool.py
  # $HOME: 表示家目录，以自己的为准，可执行 echo $HOME 查看自己的家目录
  $ esptool.py version
  esptool.py v3.1-dev
  3.1-dev
  ```
  * 注：执行`. ./export.sh`设置的环境变量为临时环境变量，只在当前shell窗口中有效，切换窗口后需要重新执行，为方便后续进行LiteOS代码的编译，建议在此处将xtensa-esp32-elf-gcc、esptool.py添加到用户环境变量中。具体操作方式为：

    ```shell
    # 打开配置文件
    $ vim ~/.bashrc
    # 在文件末尾输入以下内容：
    export PATH=$PATH:$HOME/.espressif/tools/xtensa-esp32-elf/esp-2021r1-8.4.0/xtensa-esp32-elf/bin
    export PATH=$PATH:$HOME/esp32/esp-idf/components/esptool_py/esptool
    ```

  如编译工具xtensa-esp32-elf-gcc未安装或版本较低，也可通过如下命令进行手动安装：
  ```shell
  $ cd ~/esp32
  $ mkdir tools
  $ cd tools
  $ wget https://github.com/espressif/crosstool-NG/releases/download/esp-2021r1/xtensa-esp32-elf-gcc8_4_0-esp-2021r1-linux-amd64.tar.gz
  $ tar -xzf xtensa-esp32-elf-gcc8_4_0-esp-2021r1-linux-amd64.tar.gz
  ```

  解压完成后进入解压文件，找到编译工具 xtensa-esp32-elf-gcc 的位置，添加环境变量：
  ```
  $ vim ~/.bashrc
  ```

  在文件末尾输入以下内容：
  ```shell
  export PATH=$PATH:$HOME/esp32/tools/xtensa-esp32-elf/bin
  ```

  使环境变量生效：
  ```
  $ source ~/.bashrc
  ```

  查看编译工具版本（如输入命令时，使用tab键无法自动补全或联想，可尝试退出当前shell终端，重新打开）：
  ```
  $ xtensa-esp32-elf-gcc -v
  ```

  安装 esptool，此工具用于生成 esp32 二进制烧录文件：
  ```shell
  $ python -m pip install esptool
  ```

  添加环境变量：
  ```shell
  $ vim ~/.bashrc
  ```

  在文件末尾输入以下内容：
  ```shell
  export PATH=$HOME/.local/bin:$PATH
  export PATH=$HOME/esp32/tools:$PATH
  ```

  使环境变量生效：
  ```shell
  $ source ~/.bashrc
  ```

  查看esptool是否安装成功：
  ```shell
  $ esptool.py version
  esptool.py v3.1   # 用户安装的版本可能与此处不一致，有返回版本信息即可，没有版本要求
  3.1
  ```



  **2.1.4. 利用esp32的examples工程生成bootloader.bin引导文件、分区表文件**

  ```shell
  $ cd ~/esp32/esp-idf
  $ . ./export.sh
  $ cd examples/get-started/hello_world
  $ make -j
  ```
  编译成功后，将在当前 build 目录下生成分区表文件 partitions_singleapp.bin、引导文件 bootloader.bin，具体信息可查看输出信息。
  * 注：此处为引导文件和分区表的简便生成方式，若要详细了解，可查看esp32官方文档：
    > <a href="https://docs.espressif.com/projects/esp-idf/zh_CN/v4.3.1/esp32/api-guides/bootloader.html" target="_blank">ESP-IDF编程指南 - API指南 - 引导加载程序</a>  
    > <a href="https://docs.espressif.com/projects/esp-idf/zh_CN/v4.3.1/esp32/api-guides/partition-tables.html" target="_blank">ESP-IDF编程指南 - API指南 - 分区表</a>



### 2.2. Windows（Windows10 + HUAWEI-LiteOS-Studio V1.45.9 Beta）

  HUAWEI-LiteOS-Studio 暂未适配 ESP32，本文档暂不做介绍。



## 3. 编译烧录LiteOS ESP32工程

  ### 3.1. Ubuntu下的编译

  从<a href="https://gitee.com/LiteOS/LiteOS.git" target="_blank"> LiteOS 仓库 </a>拉取LiteOS源码：

  ```shell
  $ cd ~/esp32
  $ git clone https://gitee.com/LiteOS/LiteOS.git
  ```

  进入 LiteOS 工程，拷贝 esp32 默认配置文件替换当前配置文件：
  ```shell
  $ cd ~/esp32/LiteOS
  $ cp tools/build/config/ESP32.config .config
  ````

  编译LiteOS ESP32工程：
  ```shell
  $ make clean; make -j
  ```
  编译完成后会在当前路径下生成文件 out/ESP32/Huawei_LiteOS.bin，该文件即为 esp32 的烧录文件。


  ### 3.2. windows下的编译

  HUAWEI-LiteOS-Studio 暂未适配 ESP32，本文档暂不做介绍。


## 4. 烧录

  **4.1. 安装烧录软件**

  烧录软件下载地址：<a href="https://www.espressif.com/sites/default/files/tools/flash_download_tool_3.9.0_0.zip" target="_blank">ESP32 FLASH DOWNLOAD TOOLS</a>，直接点击即可下载，该软件为 Windows 版本的，下载完成后解压，打开 flash_download_tool.exe。

  **4.2 配置及烧录**

  此处作简单说明，详细图文说明参考上述解压目录目录下的文档 doc/Flash_Download_Tool__cn.pdf 中的“2.下载示例”。

  - 打开软件，芯片类型 chipType 选择 ESP32，点击 OK 进入下一步。
  - 选择 SPIDownload。
  - 在SPIDownload页面下的空白栏，按顺序填入引导文件 bootloader.bin 的路径及下载地址、分区表文件 partitions_singleapp.bin 的路径及下载地址、esp32的烧录文件Huawei_LiteOS.bin的路径及下载地址并勾选，如下：
    > | 文件路径 | 下载地址  |
    > | ------------------------------------------------------------ | ------- |
    > | esp32/esp-idf/examples/get-started/hello_world/build/bootloader/bootloader.bin | 0x1000 |
    > | esp32/esp-idf/examples/get-started/hello_world/build/partitions_singleapp.bin | 0x8000 |
    > |esp32/LiteOS/out/ESP32/Huawei_LiteOS.bin                                                                   |0x10000|
  - SpiFlashConfig 部分 SPI SPIEED 选择 40MHz，SPIMODE 选择 DOUT，勾选 DoNoChgBin。
  - 按实际情况选择 COM，波特率 BAUD 选择 115200。
  - 点击START开始下载。



## 5. 查看运行结果

  使用串口工具查看 esp32 的串口输出，内容如下：
  ```
  ...... 引导程序内容，此处省略
 
  ********Hello Huawei LiteOS********
 
  LiteOS Kernel Version : 5.1.0
  build data : Sep 24 2021 14:56:29
  
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

  date          free          help          hwi           memcheck      swtmr         systeminfo    task
  uname         watch

  Huawei LiteOS #
  ```

  输入命令 date 查看时间
  ```
  Huawei LiteOS # date
  Thu Jan  1 00:11:20 1970

  Huawei LiteOS #
  ```
