# jerry使用手册

  `jerryscript`是一个轻量级的JavaScript引擎，它可以运行在内存资源受到限制的嵌入式设备上。目前LiteOS已适配的开发板中可以支持其运行的开发板有：
  - realview-pbx-a9

  本文档以Linux开发环境及`realview-pbx-a9`开发板为例，简单介绍`LiteOS`上`jerry`命令的使用。

## 使能jerryscript

  首先设置`realview-pbx-a9`的默认配置，在Linux系统的LiteOS源码根目录下，执行如下命令：
  ```
  ~/Huawei_LiteOS$ cp tools/build/config/realview-pbx-a9.config .config
  ```
  执行如下命令打开LiteOS menuconfig菜单启用jerryscript
  ```
  ~/Huawei_LiteOS$ make menuconfig
  ```
  按如下路径开启jerryscript组件及jerry命令
  ```
  Components  --->
      FileSystem  --->
            [*] Enable RAMFS
      Language  --->
            [*] Enable Jerryscript
  ```
  **注：** RAMFS为可选组件，如选择该组件，可生成一个`/ramfs/hello.js`示例文件，在LiteOS的shell中执行`jerry /ramfs/hello.js`命令可查看其运行结果，
  `jerryscript`组件依赖vfs，开启`jerryscript`组件后，vfs即自动开启。

  保存退出，此时LiteOS会从github自动下载`jerryscript`源代码，关于在线组件的自动下载，请查阅<a href="https://gitee.com/LiteOS/LiteOS_Components#在线组件下载流程" target="_blank">在线组件下载流程</a>。待下载完成后，执行如下命令编译LiteOS工程。
  ```
  ~/Huawei_LiteOS$ make clean;make -j
  ```
  编译成功后将在`out/realview-pbx-a9/lib`目录下生成组件静态库文件`libjerryscript.a`，生成的系统镜像文件为`Huawei_LiteOS.bin`。
 
 **注意**：下载需要Linux主机可以访问互联网，并安装`git`。

## jerry命令
   查看`jerry`命令是否生效：

   按上述方法使能`jerryscript`后，`jerry`命令会被编译进系统镜像文件中。使用串口工具，进入shell，输入help命令后，如果显示`jerry`命令，则表明已经生效。
  ```
   Huawei LiteOS # help
   *******************shell commands:*************************
   date          free          help          hwi           jerry         log           memcheck      stack         
   swtmr         systeminfo    task          uname         watch         
   Huawei LiteOS # 
  ```

  查看`jerry`命令的使用帮助信息：

   进入shell， 输入`jerry -h`
   ```
   Huawei LiteOS # jerry -h
   Usage: jerry [options]
   Options:
      -h, --help      print this help
      -v, --version   print tool and library version
      FILE            input JS file
   Huawei LiteOS # 
   ```

  运行hello.js文件
  ```
   Huawei LiteOS # jerry /ramfs/hello.js
   Hello, wlorld! I'm JerryScript.

   Huawei LiteOS #
  ```

  jerryscript目前适配接口有限，仅支持运行本地js文件，后续将持续适配更多接口以方便使用。
