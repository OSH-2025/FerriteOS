# Minmea Demo 说明文档

Minmea是一个轻量级的GPS NMEA 0183协议解析库。详情参考<a href="https://github.com/kosma/minmea" target="_blank">minmea</a>。

Minmea不依赖硬件驱动，能运行在LiteOS支持的所有开发板中。

## NMEA 0183协议

NMEA 0183协议是为了在不同的GPS导航设备中建立统一的标准而制定的，详情参考<a href="https://baike.baidu.com/item/GPS%E5%8D%8F%E8%AE%AE/306564?fr=aladdin" target="_blank">NMEA-0183</a>。


## 使能Minmea Demo

1. 在LiteOS源码根目录下根据实际使用的开发板，拷贝tools/build/config/目录下的默认配置文件${platform}.config到根目录，并重命名为.config。

2. 继续在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能Minmea Demo。

    ```
    Demos  --->
        Utility Demo  --->
                [*] Enable Minmea Demo
    ```
使能Minmea Demo后，LiteOS会自动使能Minmea组件。


保存退出后，LiteOS会从github上自动下载Minmea源代码。
详细流程在`components/download.sh`文件中。

## 编译运行Minmea Demo

使能Minmea Demo后，在LiteOS源码根目录下执行
```
$ make clean; make -j
```
命令编译LiteOS工程，编译成功后会在`out/${platform}/lib`路径下生成`minmea.a`和`minmea_demo.a`文件，系统镜像文件为Huawei_LiteOS.bin。

**注意：** 选择不同开发板out路径会有不同。

以realview-pbx-a9为例，根据`targets/realview-pbx-a9/README_CN.md`文档执行对应命令
```
$ qemu-system-arm -machine realview-pbx-a9 -smp 4 -m 512M -kernel out/realview-pbx-a9/Huawei_LiteOS.bin -nographic
```
可以看到LiteOS启动成功，并执行Minmea Demo。

本demo演示了如何使用minmea组件解析四种格式的GPS数据，输出结果如下：
```
LiteOS Kernel Version : 5.1.0
Processor   : Cortex-A9 * 4
Run Mode    : SMP
GIC Rev     : GICv1
build time  : Jun 29 2021 17:13:07

**********************************

main core booting up...
OsAppInit
releasing 3 secondary cores
cpu 0 entering scheduler
cpu 3 entering scheduler
cpu 1 entering scheduler
cpu 2 entering scheduler
app init!
Hello, welcome to liteos demo!
Minmea demo task start to run.
RMCDemo start.
$xxRMC: raw coordinates and speed: (35378333/10000,139446667/10000) 0/10
GGADemo start.
$xxGGA: raw coordinates and fix quality: (510694086/100000,170151680/100000) 405/10
GSTDemo start.
$xxGST: raw latitude,longitude and altitude error deviation: (58/10,56/10,220/10)
GSVDemo start.
$xxGSV: message 1 of 4
$xxGSV: sattelites in view: 13
$xxGSV: sat nr 2, elevation: 28, azimuth: 259, snr: 33 dbm
$xxGSV: sat nr 4, elevation: 12, azimuth: 212, snr: 27 dbm
$xxGSV: sat nr 5, elevation: 34, azimuth: 305, snr: 30 dbm
$xxGSV: sat nr 7, elevation: 79, azimuth: 138, snr: 0 dbm
Minmea demo task finished.

Huawei LiteOS # 
```
