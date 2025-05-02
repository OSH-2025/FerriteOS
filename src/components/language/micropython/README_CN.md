# micropyth使用手册

  `MicroPython`旨在在微控制器和小型嵌入式系统上实现Python 3.x。MicroPython不依赖底层硬件，理论上可运行在所有的开发板上，但其运行时需要占用一定得内存大小（该大小可通过menuconfig菜单中的`Micropython Head Size`设置，默认为4096字节），开发板内存太小可能导致其无法运行。

  本文档以Linux开发环境及`STM32F769IDISCOVERY`开发板为例，简单介绍`LiteOS`上`micropython`命令的使用。

## 开启micropython

  首先设置`STM32F769IDISCOVERY`的默认配置，在Linux系统的LiteOS源码根目录下，执行如下命令：
  ```
  ~/Huawei_LiteOS$ cp tools/build/config/STM32F769IDISCOVERY.config .config
  ```
  执行如下命令打开LiteOS menuconfig菜单打开micropython
  ```
  ~/Huawei_LiteOS$ make menuconfig
  ```
  按如下路径开启micropython组件及micropython命令
  ```
  Components  --->
      FileSystem  --->
            [*] Enable RAMFS
      Language  --->
            [*] Enable Micropython
            (4096)  Micropython Head Size
  ```
  **注：** RAMFS为可选组件，如选择该组件，可生成一个`/ramfs/hello.py`示例文件，在LiteOS的shell中执行`micropython /ramfs/hello.py`命令可查看其运行结果，
  `micropython`组件依赖vfs，开启`micropython`组件后，vfs即自动开启。

  保存退出，此时LiteOS会从github自动下载`micropython`源代码，关于在线组件的自动下载，请查阅<a href="https://gitee.com/LiteOS/LiteOS_Components#在线组件下载流程" target="_blank">在线组件下载流程</a>。待下载完成后，执行如下命令编译LiteOS工程。
  ```
  ~/Huawei_LiteOS$ make clean;make -j
  ```
  编译成功后将在`out/STM32F769IDISCOVERY/lib`目录下生成组件静态库文件`libmicropython.a`，生成的系统镜像文件为`Huawei_LiteOS.bin`。
  
  **注意**：下载需要Linux主机可以访问互联网，并安装`git`。

## micropython命令
   查看`micropython`命令是否生效：

   按上述方法使能`micropython`后，`micropython`命令会被编译进系统镜像文件中。使用串口工具，进入shell，输入help命令后，如果显示`micropython`命令，则表明已经生效。
  ```
   Huawei LiteOS # help
   *******************shell commands:*************************
   date          free          help          hwi           log           memcheck      micropython   swtmr         
   systeminfo    task          uname         watch      
   Huawei LiteOS # 
  ```

  查看`micropython`命令的使用帮助信息:

   进入shell， 输入`micropython -h`
   ```
   Huawei LiteOS # micropython -h
   Usage: micropython [options]
   Options:
     -h, --help      print this help
     -v, --version   print tool and library version
     FILE            input python file
   Huawei LiteOS # 
   ```

  运行hello.py文件
  ```
   Huawei LiteOS # micropython /ramfs/hello.py
   This test run the following script code: 
   print("Hello, wlorld! I'm Micropython.")
   a = 8
   b = 512
   c = a + b
   print(c)
   print(b"bytes 3344")
   for i in range(4):
       print(i)

   Hello, wlorld! I'm Micropython.
   520
   b'bytes 3344\x01'
   0
   1
   2
   3

   Huawei LiteOS #
  ```
  micropython当前的支持的功能还比较有限，后续将持续丰富及改进，以方便使用。

