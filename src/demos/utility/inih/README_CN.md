# Inih Demo说明文档

Inih是一个用C语言开发的简单的INI文件解析器，设计小巧紧凑，可以方便地移植到嵌入式设备应用程序中。

LiteOS中提供了`InihDemoTask`来演示如何使用`Inih`组件。目前只支持在以下开发板上运行`InihDemoTask`。

- STM32F769
- STM32F429
- realview-pbx-a9
- qemu-virt-a53

## 解析Inih Demo

在inih_demo.c中主要实现两个功能：

### 对ini文件解析

```c
int ini_parse(const char* filename, ini_handler handler, void* user);
```

​	通过调用`ini_parse`函数实现对`test.ini`文件内容进行解析，文件内容如下：

```ini
#
# This is an example of ini file
#

[INIH]

components  = inih ;
author      = tester001 ;
date        = 20210708 ;
description = Inih Demo Implementation ;
```

### 对字符串解析

```c
int ini_parse_string(const char* string, ini_handler handler, void* user);
```

通过调用`ini_parse_string`函数实现对字符串`INI_STRING_DATE`和`INI_STRING_TIME`进行解析。

```c
#define INI_STRING_DATE       "[date]\nauthor=2021\nmonth=07\nday=12"
#define INI_STRING_TIME       "[time]\nhour=16\nmin=02\nsec=31"
```


## 使能Inih Demo

在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能Inih Demo。 

```
Demos  --->
       Utility Demo  --->
               [*] Enable Inih Demo (NEW)
```

- 使能Inih Demo后会自动使能Inih组件和ramfs文件系统。  

保存退出后，会从github上自动下载Inih源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。 

## 编译运行Inih Demo

操作步骤如下：

1. 参照上述使能inih demo。

2. 在Linux主机的LiteOS源码根目录下执行`make clean ; make -j`命令编译LiteOS工程，编译成功后将在~/Huawei_LiteOS/out/${platform}/lib目录下生成inih的库文件libinih.a和此Demo库文件libinih_demo.a，系统镜像文件为Huawei_LiteOS.bin。以`Cloud_STM32F429IGTx_FIRE/`开发板为例，其目录就是`out/Cloud_STM32F429IGTx_FIRE/`。

3. 烧录系统镜像文件到开发板，复位后可以看到串口输出Demo运行结果如下所示。

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.1.0
build data : Jul 15 2021 10:45:15

**********************************
OsAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
Inih demo task start to run.
[INIH]
components = inih
author = tester001
date = 20210708
description = Inih Demo Implementation

[date]
author = 2021;
month = 07;
day = 12;
[time]
hour = 16;
min = 02;
sec = 31;
Inih demo task finished.

Huawei LiteOS # 
```
