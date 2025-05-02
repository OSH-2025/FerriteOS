# Huawei LiteOS Shell使用教程

## 基本概念
Huawei LiteOS提供shell命令行，它能够以命令行交互的方式访问操作系统的功能或服务：它接收并解析用户输入的命令，并处理操作系统的输出结果。

## 开发指导

### 使用场景
Huawei LiteOS提供的Shell作为在线调试工具，可以通过串口工具输入输出，支持常用的基本调试功能。同时用户可以新增定制的命令，新增命令需重新编译烧录后才能执行。

### 功能
1. Huawei LiteOS提供的Shell命令参见后面[命令参考](#系统命令参考)章节。
2. Huawei LiteOS的Shell模块为用户提供下面几个接口，接口详细信息可以查看API参考。
   | 接口名          | 描述                     |
   | --------------- | ------------------------ |
   | SHELLCMD_ENTRY  | 静态注册命令             |
   | osCmdReg        | 动态注册命令             |

    >![](public_sys-resources/icon-note.gif) **说明：** 
    >静态注册命令方式一般用于注册系统常用命令，动态注册命令方式一般用于注册用户命令。
    >静态注册命令有5个入参，动态注册命令有4个入参。下面除去第一个入参是静态注册独有的，剩余的四个入参两个注册命令是一致的。
    >-   第一个入参：命令变量名，用于设置链接选项（build/mk/liteos\_tables\_ldflags.mk的LITEOS\_TABLES\_LDFLAGS变量）。例如变量名为ls\_shellcmd，链接选项就应该设置为：LITEOS\_TABLES\_LDFLAGS += -uls\_shellcmd。这个入参是静态注册独有的，动态注册中没有这个入参。
    >-   第二个入参：命令类型，目前支持两种命令类型。
    >      -   CMD\_TYPE\_EX：不支持标准命令参数输入，会把用户填写的命令关键字屏蔽掉。例如：输入ls /ramfs，传入给命令处理函数的参数只有/ramfs，对应于命令处理函数中的argv\[0\]，而ls命令关键字并不会被传入。
    >      -   CMD\_TYPE\_STD：支持的标准命令参数输入，所有输入的字符都会通过命令解析后被传入。例如：输入ls /ramfs，ls和/ramfs都会被传入命令处理函数，分别对应于命令处理函数中的argv\[0\]和argv\[1\]。
    >-   第三个入参：命令关键字，是命令处理函数在Shell中对应的名称。**命令关键字必须唯一**，即两个不同的命令项不能有相同的命令关键字，否则只会执行其中一个。Shell在执行用户命令时，如果存在多个命令关键字相同的命令，只会执行在“help”命令中排在最前面的那个。
    >-   第四个入参：命令处理函数的入参最大个数。
    >      -   静态注册命令暂不支持设置。
    >      -   动态注册命令支持设置不超过32的入参最大个数，或者设置为XARGS（其在代码中被定义为0xffffffff）表示不限制参数个数。
    >-   第五个入参：命令处理函数名，即在Shell中执行命令时被调用的函数。


### 配置项

可以通过make menuconfig配置Shell，菜单路径为：Debug ---\> Enable a Debug Version ---\> Enable Shell。

| 配置项          | 含义                     | 取值范围     | 默认值     | 依赖        |
| --------------- | ------------------------ | ------------ | ---------- | ------------ |
| LOSCFG_SHELL    | Shell功能的裁剪开关      | YES/NO       | YES        | LOSCFG_DEBUG_VERSION=y && LOSCFG_DRIVERS_UART=y |
| LOSCFG_SHELL_CONSOLE（开源版本无该配置项） | 设置Shell直接与Console交互 | YES/NO     | YES     | LOSCFG_SHELL=y |
| LOSCFG_SHELL_UART    | 设置Shell直接与uart驱动交互 | YES/NO   | NO     | LOSCFG_DRIVERS_UART=y |


### 新增命令开发流程

下面以注册系统命令ls为例，介绍新增Shell命令的典型开发流程。

1.  定义Shell命令处理函数。

    Shell命令处理函数用于处理注册的命令。例如定义一个命令处理函数osShellCmdLs，处理ls命令，并在头文件中声明命令处理函数原型。

    ```c
    int osShellCmdLs(int argc, const char **argv);
    ```

    >![](public_sys-resources/icon-notice.gif) **须知：** 
    >命令处理函数的参数与C语言中main函数的参数类似，包括两个入参：
    >-   argc：Shell命令的参数个数。个数中是否包括命令关键字，和注册命令时的命令类型有关。
    >-   argv：为指针数组，每个元素指向一个字符串，该字符串就是执行shell命令时传入命令处理函数的参数。参数中是否包括命令关键字，和注册命令时的命令类型有关。

2.  注册命令。

    有静态注册命令和系统运行时动态注册命令两种注册方式。

    -   静态注册ls命令：

        ```c
        #include "shcmd.h"
        SHELLCMD_ENTRY(ls_shellcmd, CMD_TYPE_EX, "ls", XARGS, (CMD_CBK_FUNC)osShellCmdLs);
        ```

    -   动态注册ls命令：

        ```c
        #include "shell.h"
        osCmdReg(CMD_TYPE_EX, "ls", XARGS, (CMD_CBK_FUNC)osShellCmdLs);
        ```

3.  对于静态注册命令方式，在build/mk/liteos\_tables\_ldflags.mk中设置链接选项（LITEOS\_TABLES\_LDFLAGS变量）。

4.  通过make menuconfig使能Shell，详见[配置项](#配置项)。
5.  编译烧录系统后，可以执行新增的Shell命令。


### 执行Shell命令

1. 通过串口终端工具（比如xShell），连接串口。

2. 选择SERIAL协议，然后配置SERIAL：选择对应的串口号，设置波特率为115200。

3. 连接后重启开发板，系统启动后可以在串口工具中看到“Huawei LiteOS #”的提示符。
   ```
   Huawei LiteOS #
   ```

   这时可以输入命令，按`回车键`执行。

   - 按`Tab键`可自动联想补全，若有多个匹配项则补全共同字符，并打印多个匹配项。

   - 按`退格键`可删除一个字符，如果在UTF8格式下输入了中文字符，只能通过回退三次来删除。默认模式为英文输入。

   - 按`方向键上下`可选择历史命令。


## 静态注册编程实例

### 实例描述

本实例演示如何使用静态注册命令方式新增一个名为test的Shell命令。

1.  定义一个新增命令所要调用的命令处理函数cmd\_test。
2.  使用SHELLCMD\_ENTRY函数添加新增命令项。
3.  在liteos\_tables\_ldflags.mk中添加链接该新增命令项参数。
4.  通过make menuconfig使能Shell。
5.  重新编译代码后运行。

### 编程示例

1.  定义命令所要调用的命令处理函数cmd\_test：

    ```c
    #include "shell.h"
    #include "shcmd.h"
    
    int cmd_test(void)
    {
        printf("hello everybody!\n");
        return 0;
    }
    ```

2.  添加新增命令项：

    ```c
    SHELLCMD_ENTRY(test_shellcmd, CMD_TYPE_EX, "test", 0, (CMD_CBK_FUNC)cmd_test);
    ```

3.  在链接选项中添加链接该新增命令项参数：

    在build/mk/liteos\_tables\_ldflags.mk中LITEOS\_TABLES\_LDFLAGS项下添加-utest\_shellcmd。

4.  通过make menuconfig使能Shell，即设置LOSCFG\_SHELL=y。
5.  重新编译代码：

    ```
    make clean;make
    ```

### 结果验证

烧录新系统镜像后，重启系统。使用help命令查看当前系统中所有注册的命令，可以看到test命令已经注册。


## 动态注册编程实例

### 实例描述

本实例演示如何使用动态注册命令方式新增一个名为test的Shell命令。

1.  定义一个新增命令所要调用的命令处理函数cmd\_test。
2.  使用osCmdReg函数添加新增命令项。
3.  通过make menuconfig使能Shell。
4.  重新编译代码后运行。

### 编程示例

1.  定义命令所要调用的命令处理函数cmd\_test：

    ```c
    #include "shell.h"
    #include "shcmd.h"
    
    int cmd_test(void)
    {
        printf("hello everybody!\n");
        return 0;
    }
    ```

2.  在app\_init函数中调用osCmdReg函数动态注册命令：

    ```c
    void app_init(void)
    {
         ....
         ....
         osCmdReg(CMD_TYPE_EX, "test", 0,(CMD_CBK_FUNC)cmd_test);
         ....
    }
    ```

3.  通过make menuconfig使能Shell，即设置LOSCFG\_SHELL=y。
4.  重新编译代码：

    ```
    make clean;make
    ```

### 结果验证

烧录新系统镜像后，重启系统。使用help命令查看当前系统中所有注册的命令，可以看到test命令已经注册。


## 系统命令参考

-   **[help](#help)**

-   **[date](#date)**

-   **[uname](#uname)**

-   **[task](#task)**

-   **[free](#free)**

-   **[memcheck](#memcheck)**

-   **[memused](#memused)**

-   **[hwi](#hwi)**

-   **[queue](#queue)**

-   **[sem](#sem)**

-   **[mutex](#mutex)**

-   **[dlock](#dlock)**

-   **[swtmr](#swtmr)**

-   **[systeminfo](#systeminfo)**

-   **[log](#log)**

-   **[dmesg](#dmesg)**

-   **[stack](#stack)**

-   **[cpup](#cpup)**

-   **[watch](#watch)**

使用Shell中的系统命令前，需要先通过make menuconfig使能Shell，详见[配置项](#配置项)。

### help

#### 命令功能
显示当前操作系统内所有Shell命令。

#### 命令格式
```
help
```

#### 使用实例

举例：

输入help。

#### 输出说明

执行help，输出当前系统内的所有Shell命令：
```
Huawei LiteOS # help
*******************shell commands:*************************

cpup          date          dlock         dmesg         free          help          hwi
log           memcheck      mutex         queue         sem           stack         swtmr
systeminfo    task          uname         watch
```

### date
#### 命令功能
查询及设置系统时间。

#### 命令格式
```
date

date --help

date +Format

date -s YY/MM/DD

date -s hh:mm:ss

date  -r  Filename（开源版本暂不支持该命令）
```

#### 参数说明
参数|参数说明|取值范围|
|:---:|:---|:---:|
|--help|使用帮助|N/A|
|+Format|根据Format格式打印时间|--help中列出的占位符|
|-s YY/MM/DD|设置系统时间，用“/”分割的年月日|>= 1970/01/01|
|-s hh\:mm\:ss|设置系统时间，用“:”分割的时分秒|N/A|
|-r|查询文件修改时间，需要使能LOSCFG_FS_VFS|N/A|

#### 使用指南
- 该命令依赖于`LOSCFG_SHELL_EXTENDED_CMDS`，该宏开关可以通过`make menuconfig`命令在菜单项中开启`Enable Shell Ext CMDs`使能。
   ```
   Debug  ---> Enable a Debug Version ---> Enable Shell ---> Enable Shell Ext CMDs
   ```
-  date参数缺省时，默认显示当前系统时间。
-  --help、+Format、-s、-r不能混合使用。

#### 使用实例

举例：

输入date +%Y--%m--%d。

#### 输出说明

执行date +%Y--%m--%d，按其指定格式打印当前系统时间：

```
Huawei LiteOS # date +%Y--%m--%d
2021--01--20
```

### uname
#### 命令功能
显示操作系统的名称，系统编译时间，版本信息等。

#### 命令格式
```
uname [-a | -s | -t | -v | --help]
```

#### 参数说明
参数|参数说明
|:---:|:---|
|-a|显示全部信息|
|-s|显示操作系统名称|
|-t|显示系统的编译时间|
|-v|显示版本信息|
|--help|显示uname的帮助信息|

#### 使用指南
-  参数缺省时，默认显示操作系统名称。
-  `uname`的参数不能混合使用。

#### 使用实例

举例：

输入uname -a。

#### 输出说明

执行uname -a，获取系统信息：

```
Huawei LiteOS # uname -a
Huawei LiteOS - Huawei LiteOS 5.0.0-rc1 Nov  2 2020 00:50:54
```

### task
#### 命令功能
查询系统的任务信息。

#### 命令格式
```
task [ID]
```

#### 参数说明
|参数|参数说明|取值范围|
|:---:|:---|:---:|
|ID|任务ID号，输入形式以十进制表示或十六进制表示皆可|[0, 0xFFFFFFFF]|

#### 使用指南
-  参数缺省时，默认打印全部运行任务信息。
-  task后加ID，当ID参数在\[0, 64\]范围内时，返回指定ID号任务的任务名、任务ID、任务的调用栈信息（最大支持15层调用栈），其他取值时返回参数错误的提示。如果指定ID号对应的任务未创建，则提示。
-  如果在task命令中，发现任务是Invalid状态，请确保pthread\_create创建函数时有进行如下操作之一，否则资源无法正常回收。
    -  选择的是阻塞模式应该调用pthread\_join\(\)函数。
    -  选择的是非阻塞模式应该调用pthread\_detach\(\)函数。
    -  如果不想调用前面两个接口，就需要设置pthread\_attr\_t状态为PTHREAD\_STATE\_DETACHED，将attr参数传入pthread\_create，此设置和调用pthread\_detach函数一样，都是非阻塞模式。


#### 使用实例

举例1：输入task  6。

举例2：输入task。

#### 输出说明

执行task 0xb，查询ID号为b的任务信息：

```
Huawei LiteOS # task 0xb
TaskName = SerialEntryTask
TaskId = 0xb
*******backtrace begin*******
traceback 0 -- lr = 0x1d804    fp = 0xa86bc
traceback 1 -- lr = 0x1da40    fp = 0xa86e4
traceback 2 -- lr = 0x20154    fp = 0xa86fc
traceback 3 -- lr = 0x258e4    fp = 0xa8714
traceback 4 -- lr = 0x242f4    fp = 0xa872c
traceback 5 -- lr = 0x123e4    fp = 0xa8754
traceback 6 -- lr = 0x2a9d8    fp = 0xb0b0b0b
```

执行task，查询所有任务信息：

```
Huawei LiteOS # task
Name          TaskEntryAddr    TID   Priority  Status     StackSize  WaterLine  StackPoint  TopOfStack EventMask SemID  CPUUSE  CPUUSE10s  CPUUSE1s MEMUSE
----          ------------     ---   -------   --------   ---------  --------   ----------  ---------- -------  -----  -------  ---------  ------  -------
Swt_Task          0x40002770   0x0    0        QueuePend  0x6000     0x2cc     0x4015a318  0x401544e8  0x0      0xffff    0.0       0.0     0.0     0
IdleCore000       0x40002dc8   0x1    31       Ready      0x400      0x15c     0x4015a7f4  0x4015a550  0x0      0xffff   98.6      98.2    99.9     0
system_wq         0x400b80fc   0x3    1        Pend       0x6000     0x244     0x40166928  0x40160ab8  0x1      0xffff    0.0       0.0     0.0     0
SerialShellTask   0x40090158   0x5    9        Running    0x3000     0x55c     0x40174918  0x40171e70  0xfff    0xffff    1.2       1.7     0.0     48
SerialEntryTask   0x4008fe30   0x6    9        Pend       0x1000      0x2c4    0x40175c78  0x40174e88  0x1      0xffff    0.0       0.0     0.0     72
```

>![](public_sys-resources/icon-note.gif) **说明：** 
>1.  输出项说明：
>     -   Name：任务名。
>     -   TID：任务ID。
>     -   Priority：任务的优先级。
>     -   Status：任务当前的状态。
>     -   StackSize：任务栈大小。
>     -   WaterLine：该任务栈已经被使用的内存大小。
>     -   StackPoint：任务栈指针，表示栈的起始地址。
>     -   TopOfStack：栈顶的地址。
>     -   EventMask：当前任务的事件掩码，没有使用事件，则默认任务事件掩码为0（如果任务中使用多个事件，则显示的是最近使用的事件掩码）。
>     -   SemID：当前任务拥有的信号量ID，没有使用信号量，则默认0xFFFF（如果任务中使用了多个信号量，则显示的是最近使用的信号量ID）。
>     -   CPUUSE：系统启动以来的CPU占用率。
>     -   CPUUSE10s：系统最近10秒的CPU占用率。
>     -   CPUUSE1s：系统最近1秒的CPU占用率。
>     -   MEMUSE：截止到当前时间，任务所申请的内存大小，以字节为单位显示。MEMUSE仅针对系统内存池进行统计，不包括中断中处理的内存和任务启动之前的内存。
>        任务申请内存，MEMUSE会增加，任务释放内存，MEMUSE会减小，所以MEMUSE会有正值和负值的情况。
>        1）MEMUSE为0：说明该任务没有申请内存，或者申请的内存和释放的内存相同。
>        2）MEMUSE为正值：说明该任务中有内存未释放。
>        3）MEMUSE为负值：说明该任务释放的内存大于申请的内存。
>2.  系统任务说明（Huawei LiteOS系统初始任务有以下几种）：
>     -   Swt\_Task：软件定时器任务，用于处理软件定时器超时回调函数。
>     -   IdleCore000：系统空闲时执行的任务。
>     -   system\_wq：系统默认工作队列处理任务。
>     -   SerialEntryTask：从底层buf读取用户的输入，初步解析命令，例如tab补全，方向键等。
>     -   SerialShellTask：接收命令后进一步解析，并查找匹配的命令处理函数，进行调用。
>3.  任务状态说明：
>     -   Ready：任务处于就绪状态。
>     -   Pend：任务处于阻塞状态。
>     -   PendTime：阻塞的任务处于等待超时状态。
>     -   Suspend：任务处于挂起状态。
>     -   Running：该任务正在运行。
>     -   Delay：任务处于延时等待状态。
>     -   SuspendTime：挂起的任务处于等待超时状态。
>     -   Invalid：非上述任务状态。

### free
#### 命令功能
显示系统内存的使用情况，同时显示系统的text段、data段、rodata段、bss段大小。

#### 命令格式
```
free [-k | -m]
```

#### 参数说明
|参数|参数说明|取值范围|
|:---:|:---|:---:|
|无参数|以Byte为单位显示|N/A|
|-k|以KByte为单位显示|N/A|
|-m|以MByte为单位显示|N/A|

#### 使用指南
-  输入free显示内存使用情况，total表示系统动态内存池的总大小，used表示已使用的内存大小，free表示空闲的内存大小。text表示代码段大小，data表示数据段大小，rodata表示只读数据段大小，bss表示未初始化全局变量占用的内存大小。
-  free命令能够以三种单位来显示内存使用情况，包括Byte、KByte和MByte。

#### 使用实例

举例：

分别输入free、free -k、free -m。

#### 输出说明

以三种单位显示内存使用情况的输出：

```
Huawei LiteOS # free

        total        used          free
Mem:    117631744    31826864      85804880

        text         data          rodata        bss
Mem:    4116480      423656        1204224       6659316

Huawei LiteOS # free -k

        total        used          free
Mem:    114874       31080         83793

        text         data          rodata        bss
Mem:    4020         423           1176         6503

Huawei LiteOS # free -m

        total        used          free
Mem:    112          30            81

        text         data          rodata        bss
Mem:    3            0             1             6
```

### memcheck
#### 命令功能
检查动态申请的内存块是否完整，是否存在内存越界造成的节点损坏。

#### 命令格式
```
memcheck
```

#### 使用指南

-   打开内存完整性检查开关。
    -   目前只有bestfit内存管理算法支持该命令，需要使能LOSCFG\_KERNEL\_MEM\_BESTFIT：

        ```
        Kernel ---> Memory Management ---> Dynamic Memory Management Algorithm ---> bestfit
        ```

    -   该命令依赖于LOSCFG\_BASE\_MEM\_NODE\_INTEGRITY\_CHECK，使用时需要在配置项中开启"Enable integrity check or not"。

        ```
        Debug  ---> Enable a Debug Version ---> Enable MEM Debug ---> Enable integrity check or not
        ```

-   当内存池所有节点完整时，输出"memcheck over, all passed!"。
-   当内存池存在节点不完整时，输出被损坏节点的内存块信息。

#### 使用实例

举例：

输入memcheck。

#### 输出说明

-   没有内存越界时，执行memcheck输出内容如下：

    ```
    Huawei LiteOS # memcheck
    system memcheck over, all passed!
    ```

-   发生内存越界时，执行memcheck输出内容如下：

    ```
    Huawei LiteOS # memcheck
    [ERR][OsMemIntegrityCheck], 1145, memory check error!
    freeNodeInfo.pstPrev:(nil) is out of legal mem range[0x80353540, 0x84000000]
    
    broken node head: (nil)  (nil)  (nil)  0x0, pre node head: 0x7fc6a31b  0x8  0x80395ccc  0x80000110
    
    ---------------------------------------------
     dump mem tmpNode:0x80395df4 ~ 0x80395e34
    
     0x80395df4 :00000000 00000000 00000000 00000000
     0x80395e04 :cacacaca cacacaca cacacaca cacacaca
     0x80395e14 :cacacaca cacacaca cacacaca cacacaca
     0x80395e24 :cacacaca cacacaca cacacaca cacacaca
    
    ---------------------------------------------
     dump mem :0x80395db4 ~ tmpNode:0x80395df4
    
     0x80395db4 :00000000 00000000 00000000 00000000
     0x80395dc4 :00000000 00000000 00000000 00000000
     0x80395dd4 :00000000 00000000 00000000 00000000
     0x80395de4 :00000000 00000000 00000000 00000000
    
    ---------------------------------------------
    cur node: 0x80395df4
    pre node: 0x80395ce4
    pre node was allocated by task:SerialShellTask
    cpu0 is in exc.
    cpu1 is running.
    excType:software interrupt
    taskName = SerialShellTask
    taskId = 8
    task stackSize = 12288
    system mem addr = 0x80353540
    excBuffAddr pc = 0x80210b78
    excBuffAddr lr = 0x80210b7c
    excBuffAddr sp = 0x803b2d50
    excBuffAddr fp = 0x80280368
    R0         = 0x59
    R1         = 0x600101d3
    R2         = 0x0
    R3         = 0x8027a300
    R4         = 0x1
    R5         = 0xa0010113
    R6         = 0x80395e04
    R7         = 0x80317254
    R8         = 0x803b2de4
    R9         = 0x4
    R10        = 0x803afca4
    R11        = 0x80280368
    R12        = 0x1
    CPSR       = 0x600101d3
    ```

    以上各关键输出项含义如下：
    - “mem check error”，表示检测到了内存节点被破坏。
    - “cur node：”，表示该节点内存被踩，并打印内存地址。
    - “pre node：”，表示被踩节点前面的节点，并打印节点地址。
    - “pre node was allocated by task:SerialShellTask”表示在SerialShellTask任务中发生了踩内存。
    - “taskName”和“taskId”，分别表示发生异常的任务名和任务ID。

### memused
#### 命令功能
查看当前系统used节点中保存的函数调用栈LR信息。通过分析数据可检测内存泄露问题。

#### 命令格式
```
memused
```

#### 使用指南

通过make menuconfig打开内存泄漏检测。

-   目前只有bestfit内存管理算法支持该功能，需要使能LOSCFG\_KERNEL\_MEM\_BESTFIT。

    ```
    Kernel ---> Memory Management ---> Dynamic Memory Management Algorithm ---> bestfit
    ```

-   该命令依赖于LOSCFG\_MEM\_LEAKCHECK，可以在menuconfig中配置“Enable Function call stack of Mem operation recorded”：

    ```
    Debug  ---> Enable a Debug Version ---> Enable MEM Debug ---> Enable Function call stack of Mem operation recorded
    ```

    此配置项打开时，会在内存操作时，将函数调用关系LR记录到内存节点中，若相同调用栈的内存节点随时间增长而不断增多，则可能存在内存泄露，通过LR可以追溯内存申请的位置。重点关注LR重复的节点。

    >![](public_sys-resources/icon-notice.gif) **须知：** 
    >此配置打开时，会影响内存占用和内存操作性能，因此仅在检测内存泄露问题时打开。


#### 使用实例

举例：

输入memused。

#### 输出说明

```
Huawei LiteOS # memused
node         LR[0]       LR[1]       LR[2]
0x802d79e4:  0x8011d740  0x8011a990  0x00000000
0x802daa0c:  0x8011d5ec  0x8011d740  0x8011a990
0x802dca28:  0x8006e6f4  0x8006e824  0x8011d5ec
0x802e2a48:  0x8014daac  0x8014db4c  0x800f6da0
0x802e8a68:  0x8011d274  0x8011d654  0x8011d740
0x802e8a94:  0x8014daac  0x8014db4c  0x8011d494
0x802eeab4:  0x8011d4e0  0x8011d658  0x8011d740
0x802f4ad4:  0x8015326c  0x80152a20  0x800702bc
0x802f4b48:  0x8015326c  0x80152a20  0x800702bc
0x802f4bac:  0x801504d8  0x801505d8  0x80150834
0x802f4c08:  0x801504d8  0x801505d8  0x80150834
0x802f4c74:  0x801504d8  0x801505d8  0x80150834
0x802f4e08:  0x801504d8  0x801505d8  0x80150834
0x802f4e60:  0x801030e8  0x801504d8  0x801505d8
0x802f4e88:  0x80103114  0x801504d8  0x801505d8
0x802f4eb4:  0x801504d8  0x801505d8  0x80150834
0x802f4f7c:  0x801504d8  0x801505d8  0x80150834
0x802f5044:  0x801504d8  0x801505d8  0x80150834
0x802f510c:  0x800702bc  0x80118f24  0x00000000
```

### hwi
#### 命令功能
查询当前中断信息。

#### 命令格式
```
hwi
```

#### 使用指南
-  输入hwi即显示当前中断号、中断次数及注册中断名称。
-  若打开宏开关`LOSCFG_CPUP_INCLUDE_IRQ`，还会显示各个中断的处理时间（cycles）、CPU占用率以及中断类型。该宏开关可以通过`make menuconfig`在配置项中开启`Enable Cpup include irq`使能。
   ```
   Kernel --> Enable Cpup --> Enable Cpup include irq
   ```

    关于该宏开关更详细的介绍，参见[CPU占用率开发流程](/doc/LiteOS_Maintenance_Guide.md#开发流程)。


#### 使用实例

举例：

输入hwi。

#### 输出说明

1.  显示中断信息（LOSCFG\_CPUP\_INCLUDE\_IRQ关闭）

    ```
    Huawei LiteOS # hwi
    InterruptNo     Count     Name
    35:             1364:
    36:             0:
    40:             79:       uart_pl011
    ```

2.  显示中断信息（LOSCFG\_CPUP\_INCLUDE\_IRQ打开）

    ```
    Huawei LiteOS # hwi
    InterruptNo Count   Name   CYCLECOST  CPUUSE CPUUSE10s CPUUSE1s mode
            3:  1333            122       0.0    0.0       0.0    normal
            4:  0               0         0.0    0.0       0.0    normal
            5:  59   uart_pl011 305       0.0    0.0       0.0    normal
           12:  96      ETH     131       0.0    0.0       0.0    normal
    ```

### queue
#### 命令功能
查看队列的使用情况。

#### 命令格式
```
queue
```

#### 使用指南

该命令依赖于LOSCFG\_DEBUG\_QUEUE，使用时需要在配置项中开启"Enable Queue Debugging"。
```
Debug  ---> Enable a Debug Version---> Enable Debug LiteOS Kernel Resource ---> Enable Queue Debugging
```

#### 使用实例

举例：

输入queue。

#### 输出说明

执行queue后得到队列的使用情况：

```
Huawei LiteOS # queue
used queues information:
Queue ID <0x0> may leak, queue len is 0x10, readable cnt:0x0, writeable cnt:0x10, TaskEntry of creator:0x0x80007d5, Latest operation time: 0x614271
```

以上各输出项含义如下：
- Queue ID：队列编号
- queue len：队列消息节点个数
- readable cnt：队列中可读的消息个数
- writeable cnt：队列中可写的消息个数
- TaskEntry of creator：创建队列的接口地址
- Latest operation time：队列最后操作时间

### sem
#### 命令功能
查询系统内核信号量的相关信息。

#### 命令格式
```
sem [ID] 
sem fulldata
```

#### 参数说明
参数|参数说明|取值范围
|:---:|:---|:---:|
|ID|信号ID号，输入形式以十进制表示或十六进制表示皆可|[0, 0xFFFFFFFF]|
|fulldata|查询所有在用的信号量信息，打印信息包括：SemID, Count, Original Count, Creater TaskEntry, Last Access Time|N/A|

#### 使用指南
- 参数缺省时，显示所有信号量的使用数及信号量总数。
-   sem后加ID，当ID参数在[0, 1023]范围内时，返回指定ID号的信号量使用数（如果指定ID号的信号量未被使用则提示），其他取值时返回参数错误的提示。
-   sem的参数ID和fulldata不可以混用。
-   参数fulldata依赖LOSCFG\_DEBUG\_SEMAPHORE，使用时需要在配置项中开启"Enable Semaphore Debugging"。

    ```
    Debug ---> Enable a Debug Version ---> Enable Debug LiteOS Kernel Resource ---> Enable Semaphore Debugging
    ```

#### 使用实例

举例1：输入sem 1。

举例2：输入sem和sem fulldata。

#### 输出说明

执行sem 1，查询指令结果：

```
Huawei LiteOS # sem 1
   SemID       Count
   ----------  -----
   0x00000001    0x1
No task is pended on this semphore!
```

执行sem和sem fulldata，查询所有在用的信号量信息：

```
Huawei LiteOS # sem
   SemID       Count
   ----------  -----
   0x00000000  1
   SemID       Count
   ----------  -----
   0x00000001  1
   SemID       Count
   ----------  -----
   0x00000002  1
   SemUsingNum   : 3

Huawei LiteOS # sem fulldata
Used Semaphore List:
   SemID   Count   OriginalCout   Creater(TaskEntry)   LastAccessTime
   ------  -----   ------------   -----------------    --------------
   0x2     0x1     0x1            0x80164d70           0x3
   0x0     0x1     0x1            0x0                  0x3
   0x1     0x1     0x1            0x0                  0x3
```

### mutex
#### 命令功能

查看互斥锁的使用情况。

#### 命令格式
```
mutex
```

#### 使用指南
该命令依赖于LOSCFG\_DEBUG\_MUTEX，使用时需要在配置项中开启"Enable Mutex Debugging"。
```
Debug ---> Enable a Debug Version ---> Enable Debug LiteOS Kernel Resource ---> Enable Mutex Debugging
```

#### 使用实例

举例：

输入mutex。

#### 输出说明

执行mutex输出互斥锁使用情况：

```
Huawei LiteOS # mutex
used mutexs information: 
Mutex ID <0x0> may leak, Owner is null, TaskEntry of creator: 0x8000711,Latest operation time: 0x0
```

以上各输出项含义如下：
- Mutex ID：互斥锁序号
- TaskEntry of creator：创建互斥锁的接口地址
- Latest operation time：任务最后调度时间

### dlock
#### 命令功能
检查系统中的任务是否存在互斥锁（Mutex）死锁，输出系统中所有任务持有互斥锁的信息。

#### 命令格式
```
dlock
```

#### 使用指南
-  该命令需使能`LOSCFG_DEBUG_DEADLOCK`，使能方式可以通过`make menuconfig`在配置项中开启`Enable Mutex Deadlock Debugging`。
   ```
   Debug ---> Enable a Debug Version ---> Enable Debug LiteOS Kernel Resource ---> Enable Mutex Deadlock Debugging
   ```
- dlock检测输出的是在超过时间阈值（默认为10min）内没有获取到互斥锁的任务信息，并不能代表这些任务都发生了死锁，需要通过互斥锁信息及任务调用栈信息进一步确认。

#### 使用实例

举例：

输入dlock。

#### 输出说明

当系统中的任务无死锁时，dlock输出如下：

```
Huawei LiteOS # dlock
Start mutexs deadlock check:
Task_name:agenttiny_task, ID:0x0, holds the Mutexs below:
null
*******backtrace begin*******
********backtrace end********

Task_name:SerialShellTask, ID:0x4, holds the Mutexs below:
null
*******backtrace begin*******
********backtrace end********

-----------End-----------
```

当系统出现死锁时，dlock输出如下：

```
Huawei LiteOS # dlock
Start mutexs deadlock check:
Task_name:SendToSer, ID:0x0, holds the Mutexs below:
null
*******backtrace begin*******
********backtrace end********

Task_name:WowWriteFlashTask, ID:0x3, holds the Mutexs below:
null
*******backtrace begin*******
********backtrace end********

Task_name:system_wq, ID:0x4, holds the Mutexs below:
null
*******backtrace begin*******
********backtrace end********

Task_name:app_Task, ID:0x5, holds the Mutexs below:
<Mutex0 info>
Ptr handle:0x8036104c
Owner:app_Task
Count:1
Pended task: 0. name:mutexDlock_Task, id:0xc
*******backtrace begin*******
********backtrace end********

Task_name:Swt_Task, ID:0x6, holds the Mutexs below:
null
*******backtrace begin*******
*******backtrace begin*******
traceback 0 -- lr = 0x4    fp = 0x0
********backtrace end********

Task_name:IdleCore000, ID:0x7, holds the Mutexs below:
null
*******backtrace begin*******
********backtrace end********

Task_name:eth_irq_Task, ID:0xb, holds the Mutexs below:
null
*******backtrace begin*******
********backtrace end********

Task_name:mutexDlock_Task, ID:0xc, holds the Mutexs below:
<Mutex0 info>
Ptr handle:0x80361060
Owner:mutexDlock_Task
Count:1
Pended task: 0. name:app_Task       , id:0x5
*******backtrace begin*******
********backtrace end********

-----------End-----------
```

  以上各输出项含义如下：
  - Task_name:xxx, ID:xxx, holds the Mutexs below：表示疑似死锁的任务名和ID，后面几行信息是该任务持有的各个互斥锁信息，如果为“null”表示该任务并没有持有互斥锁。
  - \<MutexN info\>：后面几行是该互斥锁的详细信息，包括：
    | 输出项        | 说明                                        |
    | ------------- | ------------------------------------------- |
    | Ptr handle    | 该互斥锁的控制块地址                        |
    | Owner         | 持有该互斥锁的任务名                        |
    | Count         | 该互斥锁的引用计数                          |
    | Pended Task   | 正在等待该互斥锁的任务名，如果没有则为null  |

### swtmr
#### 命令功能
查询系统软件定时器相关信息。

#### 命令格式
```
swtmr [ID]
```

#### 参数说明
|参数|参数说明|取值范围|
|:---:|:---|:---:|
|ID|软件定时器ID号，输入形式以十进制表示或十六进制表示皆可|[0, 0xFFFFFFFF]|

#### 使用指南
-  参数缺省时，默认显示所有软件定时器的相关信息。
-  swtmr后加ID号时，当ID在[0, 当前软件定时器个数 - 1]范围内时，返回对应ID的软件定时器的相关信息，其他取值时返回错误提示。

#### 使用实例

举例：

输入swtmr和swtmr 1。

#### 输出说明

执行swtmr查询软件定时器相关信息，输出如下：

```
Huawei LiteOS # swtmr

SwTmrID     State    Mode    Interval  Arg         handlerAddr
----------  -------  ------- --------- ----------  --------
0x00000000  Ticking  Period   1000     0x00000000  0x800442d
```

执行swtmr 0查询ID为0的软件定时器信息，输出如下：

```
Huawei LiteOS # swtmr 0

SwTmrID     State    Mode    Interval  Arg         handlerAddr
----------  -------  ------- --------- ----------  --------
0x00000000  Ticking  Period   1000     0x00000000  0x800442d
```

以上各关键输出项含义如下：
| 输出项      | 说明                   |
| ----------- | --------------------- |
| SwTmrID     | 软件定时器ID           |
| State       | 软件定时器状态         |
| Mode        | 软件定时器模式         |
| Interval    | 软件定时器使用的Tick数 |
| Arg         | 传入的参数             |
| handlerAddr | 回调函数的地址         |

### systeminfo
#### 命令功能
显示当前操作系统的资源使用情况，包括任务、信号量、互斥锁、队列、软件定时器等。
对于信号量、互斥锁、队列和软件定时器，如果在系统镜像中已经裁剪了这些模块，那么说明系统没有使用这些资源，该命令也就不会显示这些资源的情况。

#### 命令格式
```
systeminfo
```

#### 使用实例

举例：

输入systeminfo。

#### 输出说明

执行systeminfo查看系统资源使用情况，输出如下：

```
Huawei LiteOS # systeminfo

   Module    Used      Total
--------------------------------
   Task      5         16
   Sem       0         20
   Mutex     1         20
   Queue     1         10
   SwTmr     1         16
```

以上各输出项含义如下：
| 输出项      | 说明            |
| ---------- | --------------- |
| Module     | 模块名称         |
| Used       | 当前使用量       |
| Total      | 最大可用量       |

### log
#### 命令功能
设置&查询系统的日志打印等级。

#### 命令格式
```
log level [levelNum]
```

#### 参数说明
参数|参数说明|取值范围
|:---:|:---|:---:|
|levelNum|设置日志打印等级|[0x0，0x5]|

>![](public_sys-resources/icon-notice.gif) **须知：** 
>暂不支持设置module、path。

#### 使用指南

-   该命令依赖于LOSCFG\_SHELL\_LK，使用时可以通过make menuconfig在配置项中开启"Enable Shell lk"。
    ```
    Debug --> Enable a Debug Version --> Enable Shell --> Functionality of Shell --> Enable Shell lk
    ```
-  `log level`命令用于设置系统的日志打印等级，包括6个等级。
    ```
    TRACE_EMG = 0,
    TRACE_COMMOM = 1,
    TRACE_ERROR = 2,
    TRACE_WARN = 3,
    TRACE_INFO = 4,
    TRACE_DEBUG = 5
    ```
-  若`log level`命令不加`levelNum`参数，则提示命令的使用方法。

#### 使用实例

举例：

输入log level 4。

#### 输出说明

执行log level 4，设置系统的日志打印等级为INFO级别：

```
Huawei LiteOS # log level 4
Set current log level INFO
```

### dmesg
#### 命令功能
dmesg命令用于控制内核dmesg缓存区。

#### 命令格式
```
dmesg

dmesg [-c | -C | -D | -E | -L | -U]

dmesg -s size

dmesg -l level

dmesg > file
```

#### 参数说明
|参数|参数说明|取值范围|
|:---:|:---|:---|
|-c|打印缓存区内容并清空缓存区，此打印不受-D、-L影响|N/A|
|-C|清空缓存区|N/A|
|-D \| -E|关闭/开启控制台打印，开源版本暂不支持该参数|N/A|
|-L \| -U|关闭/开启串口打印，开源版本暂不支持该参数|N/A|
|-s size|设置缓存区大小|N/A|
|-l level|设置缓存区的日志打印等级|0 -- 5|
|> file|将缓存区内容写入文件，开源版本暂不支持该参数|N/A|

#### 使用指南
-  该命令依赖于LOSCFG\_SHELL\_DMESG，使用时可以通过make menuconfig在配置项中开启"Enable Shell dmesg"。
   ```
   Debug  ---> Enable a Debug Version---> Enable Shell ---> Functionality of Shell ---> Enable Shell dmesg
   ```
-  参数缺省时，默认打印缓存区内容。
-  参数均不能混合使用。

#### 使用实例

举例：

输入dmesg后，接着输入dmesg  -C和dmesg。

#### 输出说明

第一次执行dmesg后，可以看到输出了缓存区内容。接着执行dmesg  -C清空缓存区内容，紧接着再次执行dmesg可以看到之前缓存区中的内容已经被清空：

```
Huawei LiteOS # dmesg


Huawei LiteOS # log level 4

Set current log level INFO

Huawei LiteOS # dmesg


Huawei LiteOS # dmesg -C

Huawei LiteOS # dmesg


Huawei LiteOS # dmesg
```

>![](public_sys-resources/icon-notice.gif) **须知：**
>dmesg -C是日志维测命令，只支持串口输出，不支持通过console输出到其他输出端口（Telnet、USB等）。

### stack

#### 命令功能

stack命令用于显示当前操作系统内所有栈的信息。

#### 命令格式
```
stack
```

#### 使用指南

#### 使用实例

举例：

输入stack。

#### 输出说明

执行stack命令查看系统内所有栈的信息：

```
Huawei LiteOS # stack

 stack name    cpu id     stack addr     total size   used size
 ----------    ------     ---------      --------     --------
  udf_stack      3        0x3c800        0x28         0x0   
  udf_stack      2        0x3c828        0x28         0x0   
  udf_stack      1        0x3c850        0x28         0x0   
  udf_stack      0        0x3c878        0x28         0x0   
  abt_stack      3        0x3c8a0        0x28         0x0   
  abt_stack      2        0x3c8c8        0x28         0x0   
  abt_stack      1        0x3c8f0        0x28         0x0   
  abt_stack      0        0x3c918        0x28         0x0   
  fiq_stack      3        0x3ca40        0x40         0x0   
  fiq_stack      2        0x3ca80        0x40         0x0   
  fiq_stack      1        0x3cac0        0x40         0x0   
  fiq_stack      0        0x3cb00        0x40         0x0   
  svc_stack      3        0x3cb40        0x2000       0x524 
  svc_stack      2        0x3eb40        0x2000       0x524 
  svc_stack      1        0x40b40        0x2000       0x524 
  svc_stack      0        0x42b40        0x2000       0x528 
  irq_stack      3        0x3c940        0x40         0x0   
  irq_stack      2        0x3c980        0x40         0x0   
  irq_stack      1        0x3c9c0        0x40         0x0   
  irq_stack      0        0x3ca00        0x40         0x0   
  exc_stack      3        0x44b40        0x1000       0x0   
  exc_stack      2        0x45b40        0x1000       0x0   
  exc_stack      1        0x46b40        0x1000       0x0   
  exc_stack      0        0x47b40        0x1000       0x0
```

>![](public_sys-resources/icon-notice.gif) **须知：**
>stack是异常维测命令，只支持串口输出，不支持通过console输出到其他输出端口（Telnet、USB等）。

### cpup
#### 命令功能
查询系统CPU的占用率，并以百分比显示占用率。

#### 命令格式
```
cpup [mode] [taskID]
```

#### 参数说明
|参数|参数说明|取值范围|
|:---:|:---|:---:|
|mode|缺省：显示系统最近10s的CPU占用率<br>0：显示系统最近10s的CPU占用率<br>1：显示系统最近1s的CPU占用率<br>其他数字：显示系统启动至今总的CPU占用率| [0,0xFFFF]<br>或0xFFFFFFFF|
|taskID|任务ID号|[0,0xFFFF]或0xFFFFFFFF|

#### 使用指南
-  参数缺省时，显示系统最近10s的CPU占用率。

-  只输入一个参数时，该参数为mode，显示系统相应时间的CPU占用率。

-  输入两个参数时，第一个参数为mode，第二个参数为taskID，显示指定任务ID的任务在相应时间的CPU占用率。
-   该功能需要使能CPU占用率模块，可以通过make menuconfig使能，菜单路径为：

    ```
    Kernel ---> Enable Extend Kernel ---> Enable Cpup
    ```

    想更多的了解CPU占用率模块，参见[CPU占用率](/doc/LiteOS_Maintenance_Guide.md#CPU占用率)。


#### 使用实例

举例：

输入cpup 1 1

#### 输出说明

执行cpup 1 1，显示ID为1的任务最近1s的CPU占用率：

```
Huawei LiteOS # cpup 1 1
TaskId 1 CpuUsage in 1s: 78.7
```

### watch
#### 命令功能
周期性监听一个命令的运行结果。

#### 命令格式
```
watch [-c | -n | -t | --count | --interval | -no-title] [command]
watch --over
```

#### 参数说明

|参数|参数说明|缺省值|取值范围|
|:---:|:---|:---|:---|
|-c / --count|命令执行的总次数|0xFFFFFF|（0,0xFFFFFF]|
|-n / --interval|命令周期性执行的时间间隔（s）|1s|（0,0xFFFFFF]|
|-t / -no-title|关闭顶端的时间显示|N/A|N/A|
|command|需要监听的Shell命令|N/A|N/A|
|--over|关闭当前的监听|N/A|N/A|

#### 使用指南
- 该命令依赖于`LOSCFG_SHELL_EXTENDED_CMDS`，该宏开关可以通过`make menuconfig`命令在菜单项中开启`Enable Shell Ext CMDs`使能。
   ```
   Debug  ---> Enable a Debug Version ---> Enable Shell ---> Enable Shell Ext CMDs
   ```
-  command参数必须是Shell命令，对于非Shell命令，会有错误提示“command is not fount”。
-  如果要监听命令，command是必填参数。
-  --over参数不能与其他参数混合使用。

#### 使用实例

举例1：输入watch -c  5  task 1。

举例2：在不需要watch命令监听的情况下，执行watch --over。

#### 输出说明

每个周期间隔1秒的执行task 1命令，共执行5次，watch命令监听到的结果如下所示：

```
Huawei LiteOS # watch -c 3 task 1

Huawei LiteOS # Thu Jan  1 16:26:26 1970

TaskName = Swt_Task
TaskId = 0x1
*******backtrace begin*******
traceback 1 -- lr = 0x08004006 -- fp = 0x0800045e
traceback 2 -- lr = 0x08004000 -- fp = 0x0800194c
traceback 3 -- lr = 0x080040da -- fp = 0x08003e50
traceback 4 -- lr = 0x080015c2 -- fp = 0x080040a8
traceback 5 -- lr = 0x0800396e -- fp = 0x08001598

Thu Jan  1 16:26:27 1970

TaskName = Swt_Task
TaskId = 0x1
*******backtrace begin*******
traceback 1 -- lr = 0x08004006 -- fp = 0x0800045e
traceback 2 -- lr = 0x08004000 -- fp = 0x0800194c
traceback 3 -- lr = 0x080040da -- fp = 0x08003e50
traceback 4 -- lr = 0x080015c2 -- fp = 0x080040a8
traceback 5 -- lr = 0x0800396e -- fp = 0x08001598

Thu Jan  1 16:26:28 1970

TaskName = Swt_Task
TaskId = 0x1
*******backtrace begin*******
traceback 1 -- lr = 0x08004006 -- fp = 0x0800045e
traceback 2 -- lr = 0x08004000 -- fp = 0x0800194c
traceback 3 -- lr = 0x080040da -- fp = 0x08003e50
traceback 4 -- lr = 0x080015c2 -- fp = 0x080040a8
traceback 5 -- lr = 0x0800396e -- fp = 0x08001598
```

## Trace命令参考

-   **[trace_mask](#trace_mask)**

-   **[trace_start](#trace_start)**

-   **[trace_stop](#trace_stop)**

-   **[trace_dump](#trace_dump)**

-   **[trace_reset](#trace_reset)**

使用Shell中的Trace命令前，需要先通过make menuconfig使能Shell，详见[配置项](#配置项)。
同时配置Trace模块，详见Trace开发流程中的[配置Trace](/doc/LiteOS_Maintenance_Guide.md#开发流程-1)。下文中的“离线模式”在menuconfig中的菜单项为：Kernel ---\> Enable Extend Kernel ---\> Enable Trace Feature ---\> Trace work mode ---\> Offline mode”。
关于Trace模块的详细介绍，详见[Trace](/doc/LiteOS_Maintenance_Guide.md#Trace)。

<h3 id="trace_mask">trace_mask</h3>

#### 命令功能

设置事件过滤掩码。

#### 命令格式
```
trace_mask [MASK]
```

#### 参数说明
参数|参数说明|取值范围|
|:---:|:---|:---:|
|MASK|Trace事件掩码|[0, 0xFFFFFFFF]|

#### 使用指南

-   如果不设置事件掩码，或者执行该命令时参数缺省，则默认仅开启任务和中断事件记录。
-   trace_mask后加MASK，则开启对应模块的事件记录。
-   具体的模块掩码MASK参见los_trace.h中定义的LOS_TRACE_MASK。

#### 使用实例

举例：

1.  输入trace_mask 0
2.  输入trace_mask 0xFFFFFFFF

#### 输出说明

执行trace_mask 0，设置所有模块的事件都不记录，命令执行成功后，不会输出信息。

```
Huawei LiteOS # trace_mask 0

Huawei LiteOS #
```

执行trace_mask 0xFFFFFFFF，设置所有模块的事件都进行记录，命令执行成功后，不会输出信息。

```
Huawei LiteOS # trace_mask 0xFFFFFFFF

Huawei LiteOS #
```

<h3 id="trace_start">trace_start</h3>

#### 命令功能

开启Trace。

#### 命令格式
```
trace_start
```

#### 使用指南

输入trace_start即开启系统Trace功能，离线模式下会记录系统发生的事件并保存在指定buffer中。

#### 使用实例

举例：开启系统Trace功能，输入trace_start。

#### 输出说明

在离线模式下，成功执行trace_start命令后，不会输出信息。

```
Huawei LiteOS # trace_start

Huawei LiteOS #
```

<h3 id="trace_stop">trace_stop</h3>

#### 命令功能

停止Trace。

#### 命令格式
```
trace_stop
```

#### 使用指南

输入trace_stop即终止系统Trace功能， 停止记录事件。

#### 使用实例

举例：停止系统Trace功能，输入trace_stop。

#### 输出说明

成功执行trace_stop命令后，不会输出信息。

```
Huawei LiteOS # trace_stop

Huawei LiteOS #
```

<h3 id="trace_dump">trace_dump</h3>

#### 命令功能

在离线模式下，dump出Trace buffer的信息。

#### 命令格式
```
trace_dump [1 | 0]
```

#### 参数说明
参数|参数说明|取值范围|
|:---:|:---|:---:|
|1|将Trace数据输出到客户端|N/A|
|0|将Trace数据格式化打印|N/A|

#### 使用指南

-   只能在离线模式下使用trace_dump命令。
-   参数缺省时将格式化打印Trace数据。
-   trace_dump命令打印的是trace_start和trace_stop之间的数据，所以需要先执行trace_stop停止Trace后，再执行trace_dump打印Trace buffer的信息。

#### 使用实例

举例：

输入trace_dump。

#### 输出说明

执行trace_dump命令，格式化打印缓存中的数据。

```
Huawei LiteOS # trace_dump
*******TraceInfo begin*******
clockFreq = 180000000
CurEvtIndex = 19
Index   Time(cycles)      EventType      CurTask   Identity      params    
0       0x7da8da5180      0x45           0x5       0x2           0x9          0x20         0x1f
1       0x7dde8c6980      0x45           0x2       0x5           0x1f         0x4          0x9
2       0x7e1431df20      0x45           0x5       0x2           0x9          0x20         0x1f
3       0x7e49e3f720      0x45           0x2       0x5           0x1f         0x4          0x9
4       0x7e7f896cc0      0x45           0x5       0x2           0x9          0x20         0x1f
5       0x7eb53b84c0      0x45           0x2       0x5           0x1f         0x4          0x9
6       0x7eeae0fa60      0x45           0x5       0x2           0x9          0x20         0x1f
7       0x7f20931260      0x45           0x2       0x5           0x1f         0x4          0x9
8       0x7f56388800      0x45           0x5       0x2           0x9          0x20         0x1f
9       0x7f8beaa000      0x45           0x2       0x5           0x1f         0x4          0x9
10      0x7fc19015a0      0x45           0x5       0x2           0x9          0x20         0x1f
11      0x7ff7422da0      0x45           0x2       0x5           0x1f         0x4          0x9
12      0x802ce7a340      0x45           0x5       0x2           0x9          0x20         0x1f
13      0x806299bb40      0x45           0x2       0x5           0x1f         0x4          0x9
14      0x80983f30e0      0x45           0x5       0x2           0x9          0x20         0x1f
15      0x80cdf148e0      0x45           0x2       0x5           0x1f         0x4          0x9
……
24      0x6c560a8d00      0x24           0x2       0x2d          0x0          0x0          0x0
25      0x6c8baf7600      0x25           0x2       0x2d          0x0          0x0          0x0
……
36      0x71fe6f2000      0x24           0x2       0x2d          0x0          0x0          0x0
37      0x7234140900      0x25           0x2       0x2d          0x0          0x0          0x0
38      0x7269c20250      0x45           0x2       0x1           0x1f         0x4          0x0
39      0x734055a650      0x45           0x1       0x2           0x0          0x8          0x1f
40      0x7380b52450      0x45           0x2       0x1           0x1f         0x4          0x0
……
48      0x77a6d3b300      0x24           0x2       0x2d          0x0          0x0          0x0 
49      0x77dc789c00      0x25           0x2       0x2d          0x0          0x0          0x0
50      0x7812269550      0x45           0x2       0x1           0x1f         0x4          0x0
……
*******TraceInfo end*******
```

<h3 id="trace_reset">trace_reset</h3>

#### 命令功能

在离线模式下，清除Trace buffer中的事件数据。

#### 命令格式
```
trace_reset
```

#### 使用指南

只能在离线模式下使用trace_reset命令。

#### 使用实例

举例：

输入 trace_reset。

#### 输出说明

执行trace_reset，清除事件数据：

```
Huawei LiteOS # trace_reset

Huawei LiteOS #
```
