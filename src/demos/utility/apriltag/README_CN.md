# Apriltag Demo说明文档

Apriltag是一个机器人研究中流行的视觉基准系统。由于运行时对内存大小有要求，目前只支持在STM32F769开发板上运行。


## 解析Apriltag Demo

本demo通过对tag36h11格式图片进行解析，获取图片信息。
**注意：**由于内存原因，Apriltag demo解析的图片不能大于20K。

##### 接口介绍：

```c
STATIC VOID ApriltagCreate(getopt_t **getOpt, apriltag_family_t **tf, apriltag_detector_t **td);
```

功能：初始化getOpt、tf和td。

```c
STATIC VOID ApriltagDestory(getopt_t *getOpt, apriltag_detector_t *td, apriltag_family_t *tf);
```

功能：释放由getOpt、td和tf申请的内存。

```c
STATIC VOID SetApriltagDetector(apriltag_detector_t *td, getopt_t *getopt);
```

功能：设置命令行参数。

```c
STATIC VOID ApriltagGetOpt(getopt_t *getOpt);
```

功能：添加命令行参数。


## 使能Apriltag Demo

1. 在LiteOS源码根目录下根据实际使用的开发板，拷贝tools/build/config/目录下的默认配置文件${platform}.config到根目录，并重命名为.config。

2. 继续在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能Apriltag组件和Apriltag Demo。

    ```
    Demos  --->
        Utility Demo  --->
                [*] Enable Apriltag Demo
    ```

保存退出后，LiteOS会从github上自动下载Apriltag源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。
详细流程请参考`components/download.sh`。

**注意：** 下载需要Linux主机可以访问互联网，并安装`git`。

## 编译运行Apriltag Demo

操作步骤如下：

1. 将/demos/utility/apriltag目录下的tag36h11_20k_586.jpg图片存入SD卡，默认路径为`/fatfs`；
2. 将SD卡插入769开发板的卡槽内；
3. 参照上述步骤使能Apriltag Demo；
4. 在LiteOS源码根目录下执行`make clean ; make -j`命令编译LiteOS工程，编译成功后会在`out/${platform}/lib`路径下生成`libapriltag.a`和`libapriltag_demo.a`文件，系统镜像文件为Huawei_LiteOS.bin。

**注意：** 选择不同开发板out路径会有不同。

结果如下：

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.1.0
build data : Nov 17 2021 14:10:19

**********************************
OsAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
File system mount success.
width = 605, height = 783
detection   0: id (36x11)-586 , hamming 0, margin  236.165

Huawei LiteOS # 

```
