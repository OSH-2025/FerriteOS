# Bidireference Demo说明文档

bidirefc是一个C语言实现的轻量级的解析双向字符的算法程序。
注：license未注明，仅限于学习用途。

## 解析Bidireference Demo

本demo参考了bidireference源代码包中bidiref.c文件的测试接口，实现了对0X2000,0X2460双向字符进行"-u63", "-z", "-d3"算法解析，其中-u63为UBA算法类型，-z为静态测试，-d3为算法调试级别。

## 使能Bidireference Demo

由于此组件需要较大的内存资源，暂时只有使用qemu运行的realview-pbx-a9和qemu-virt-a53工程支持，以realview-pbx-a9为例，执行以下操作：

    ~/Huawei_LiteOS$ cp tools/build/config/realview-pbx-a9.config .config

在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能Bidirefc Demo。

```
Kernel  --->
        [ ] Enable Kernel SMP
        
Demos  --->
       Utility Demo  --->
               [*] Enable Bidireference Demo (NEW)
```

- 使能Bidireference Demo后会自动使能Bidireference组件和ramfs文件系统。 
- 由于当前只支持单核模式下运行该组件，需要关闭SMP多核功能。

保存退出后，会从BidirefenceC官网上自动下载bidireference源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。

详细流程在`components/download.sh`和`components/utility/bidirefc/download_bidi.sh`文件中。 

**注意：**如果下载bidireference源代码失败，需要到<a href="http://unicode.org/Public/PROGRAMS/BidiReferenceC/8.0.0/" target="_blank">BidirefenceC官网</a>手动下载：

## 测试结果

在Linux主机的LiteOS源码根目录下执行make clean ; make -j命令编译LiteOS工程，编译成功后将在~/Huawei_LiteOS/out/realview-pbx-a9/lib目录下生成组件库文件libbidirefc.a和此Demo库文件libbidireference_demo.a，系统镜像文件为Huawei_LiteOS.bin。

执行以下命令即可运行LiteOS工程，更详细的信息可以参考[CORTEX A9在QEMU下运行LiteOS](https://gitee.com/LiteOS/LiteOS/blob/master/targets/realview-pbx-a9/README_CN.md)。

    ~/Huawei_LiteOS$ qemu-system-arm -machine realview-pbx-a9 -m 512M -kernel out/realview-pbx-a9/Huawei_LiteOS.bin -nographic

Demo运行结果如下所示，可以看到此Demo根据UBA算法类型解析双向字符。

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
Processor  : Cortex-A9 * 4
Run Mode   : SMP
GIC Rev    : GICv1
build time : Mar 13 2021 01:51:13

**********************************

main core booting up...
osAppInit
releasing 3 secondary cores
cpu 0 entering scheduler
cpu 3 entering scheduler
cpu 1 entering scheduler
cpu 2 entering scheduler
app init!
Hello, welcome to liteos demo!
Bidireference demo task start to run.
Current State: 1
Paragraph Dir: 2 (Dir_Auto), Paragraph Embed Level: 0, TextLen: 2

  Text:        2000 2460
  Bidi_Class:    WS   ON
  Levels:         0    0

Current State: 2
Paragraph Dir: 2 (Dir_Auto), Paragraph Embed Level: 0, TextLen: 2

  Text:        2000 2460
  Bidi_Class:    WS   ON
  Levels:         0    0

Current State: 3
  Text:        2000 2460
  Bidi_Class:    WS   ON
  Levels:         0    0

Current State: 4
  Text:        2000 2460
  Bidi_Class:    WS   ON
  Levels:         0    0

Current State: 5
  Position:       0    1
  Text:        2000 2460
  Bidi_Class:    WS   ON
  Levels:         0    0
  Runs:        <L-----L>

Current State: 6
  Position:       0    1
  Text:        2000 2460
  Bidi_Class:    WS   ON
  Levels:         0    0
  Runs:        <L-----L>
  Seqs (L= 0): <L-----L>

Current State: 7
  Text:        2000 2460
  Bidi_Class:    WS   ON
  Levels:         0    0
  Runs:        <L-----L>

Current State: 8
  Text:        2000 2460
  Bidi_Class:    WS   ON
  Levels:         0    0
  Runs:        <L-----L>

Current State: 9
  Text:        2000 2460
  Bidi_Class:    WS   ON
  Levels:         0    0
  Runs:        <L-----L>

Current State: 10
  Text:        2000 2460
  Bidi_Class:    WS   ON
  Levels:         0    0
  Runs:        <L-----L>

Current State: 11
  Text:        2000 2460
  Bidi_Class:    WS   ON
  Levels:         0    0
  Runs:        <L-----L>

Current State: 12
  Text:        2000 2460
  Bidi_Class:    WS   ON
  Levels:         0    0
  Runs:        <L-----L>

Current State: 13
  Text:        2000 2460
  Bidi_Class:    WS   ON
  Levels:         0    0
  Runs:        <L-----L>

Current State: 14
  Text:        2000 2460
  Bidi_Class:    WS   ON
  Levels:         0    0
  Runs:        <L-----L>

Current State: 15
  Text:        2000 2460
  Bidi_Class:     L    L
  Levels:         0    0
  Runs:        <L-----L>

Current State: 16
  Text:        2000 2460
  Bidi_Class:     L    L
  Levels:         0    0
  Runs:        <L-----L>

Current State: 17
  Text:        2000 2460
  Bidi_Class:     L    L
  Levels:         0    0
  Runs:        <L-----L>

Current State: 18
  Text:        2000 2460
  Bidi_Class:     L    L
  Levels:         0    0
  Runs:        <L-----L>

Current State: 19
  Text:        2000 2460
  Bidi_Class:     L    L
  Levels:         0    0
  Runs:        <L-----L>

  Order:      [0 1]
Bidireference demo task finished.
```