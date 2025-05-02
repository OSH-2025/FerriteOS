# regex使用手册

  `regex`是一个用C语言实现的小型正则表达式(regex)库，regex组件不依赖硬件，可在所有已支持的开发板上运行。

  本文档以Linux开发环境及`STM32F769IDISCOVERY`开发板为例，简单介绍`LiteOS`上`regex`命令的使用。

## 开启regex

  首先设置`STM32F769IDISCOVERY`的默认配置，在Linux系统的LiteOS源码根目录下，执行如下命令：
  ```
  ~/Huawei_LiteOS$ cp tools/build/config/STM32F769IDISCOVERY.config .config
  ```
  执行如下命令打开LiteOS menuconfig菜单打开regex
  ```
  ~/Huawei_LiteOS$ make menuconfig
  ```
  按如下路径开启regex组件及regex命令
  ```
  Components  --->
      Utility  --->
            [*] Enable Regex
  ```

  保存退出，此时LiteOS会从github自动下载`regex`源代码，关于在线组件的自动下载，请查阅<a href="https://gitee.com/LiteOS/LiteOS_Components#在线组件下载流程" target="_blank">在线组件下载流程</a>。待下载完成后，执行如下命令编译LiteOS工程。
  ```
  ~/Huawei_LiteOS$ make clean;make -j
  ```
  编译成功后将在`out/STM32F769IDISCOVERY/lib`目录下生成组件静态库文件`libregex.a`，生成的系统镜像文件为`Huawei_LiteOS.bin`。
  
  **注意**：下载需要Linux主机可以访问互联网，并安装`git`。

## regex命令
   查看`regex`命令是否生效：

   按上述方法使能`regex`后，`regex`命令会被编译进系统镜像文件中。使用串口工具，进入shell，输入help命令后，如果显示`regex`命令，则表明已经生效。
  ```
   Huawei LiteOS # help
   *******************shell commands:*************************

   date          free          help          hwi           memcheck      regex         swtmr         systeminfo    
   task          uname         watch  
  ```

  查看`regex`命令的使用帮助信息:

   进入shell， 输入`regex -h`
   ```
   Huawei LiteOS # regex -h

   Usage: regex [options]
   Options:
     h, --help      print this help
     <PATTERN> <TEXT>

     eg: regex \D+ Liteos-regex-test-97
         regex \D Liteos-regex-test-97
         regex \d Liteos-regex-test-97

  Huawei LiteOS # 
   ```

  执行正则表达式：
  ```
   Huawei LiteOS # regex \D+ Liteos-regex-test-97
   regex run success [18].

   Huawei LiteOS # regex \D Liteos-regex-test-97
   regex run success [1].

   Huawei LiteOS # regex \d Liteos-regex-test-97
   regex run success [1].

   Huawei LiteOS # 
  ```

  **注** 上面示例中的[18]、[1]表示匹配长度（根据正则表达式条件匹配得到的长度），本组件源码由`https://github.com/kokke/tiny-regex-c`下载，关于regex库支持的正则表达运算符可访问该地址进行查看。

