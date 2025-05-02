# Libpng Demo说明文档
Png（Portable Network Graphics，便携式网络图形）图片是一种无损压缩的位图图形。libpng是一套C语言编写的程序库，支持png文件的创建、读写等操作。

LiteOS中提供了`LibpngDemoTask`来演示如何使用`Libpng组件`。该demo参考了libpng源代码包中pngtest.c文件的测试接口，演示了libpng的格式转换功能。由于此组件需要较大内存资源和外部存储空间，暂时只支持在以下开发板上运行`LibpngDemoTask`。

- STM32F769


## Libpng Demo代码简介

Libpng Demo的调用入口在`targets/bsp/common/demo_entry.c` 内。

```c
#ifdef LOSCFG_DEMOS_LIBPNG
    LibpngDemoTask();
#endif
```

函数实现在`demos/media/libpng/libpng_demo.c`文件中。

```c
STATIC INT32 DemoTaskEntry(VOID)
{
    printf("Libpng demo task start to run.\n");
    INT32 driver = hal_fatfs_init(0);
    if (driver < 0) {
        (VOID)fatfs_unmount(FATFS_MAIN_DIR, (UINT8)driver);
        printf("Fatfs init failed.\n");
        return LOS_NOK;
    }

    PngTranscode(SRC_FILE, DEST_FILE);
    fatfs_unmount(FATFS_MAIN_DIR, (UINT8)driver);
    printf("Libpng demo task finished.\n");

    return LOS_OK;
}

VOID LibpngDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = PNG_TASK_STACK_SIZE;
    taskInitParam.pcName = "LibpngDemoTask";
    taskInitParam.usTaskPrio = PNG_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create libpng demo task failed.\n");
    }
}
```

Png Demo 创建了png读写结构体和输入输出流，获取png图片信息（像素点 长宽深），设置png文件转换标识，将源png图片转换为新图片输出。源文件与目标文件可在`demos/media/libpng/libpng_demo.c`中，宏定义`SRC_FILE`及`DEST_FILE`修改。

## 使能Png Demo

1. 在LiteOS源码根目录下执行`cp tools/build/config/STM32F769IDISCOVERY.config .config`设置相应开发板的默认配置。

2. 使能Png Demo。本Demo使用SD存放png图片，所以需要使能FATFS文件系统并设置为SD卡模式`FileSystem --> Enable FATFS --> Choose Peripheral Driver`，使能libpng组件`Media --> Enable Libpng`后，最后再使能Demo`Media Demo --> Enable Libpng Demo`。在LiteOS源码根目录下运行`make menuconfig`命令，按如下菜单路径使能Png Demo：

    ```
    Components    --->
            FileSystem    --->
                    -*- Enable FATFS (NEW)
                        Choose Peripheral Driver (SD Card)  --->
                                                        (X) SD Card
            Media    --->
                    [*] Enable Libpng (NEW)
    Demos    --->
            Media Demo    --->
                    [*] Enable Libpng Demo (NEW)
    ```

保存退出后，LiteOS会从github上自动下载libpng源代码，并从gitee上下载适配于LiteOS系统的patch包，并打入patch。
详细流程在`components/download.sh`文件中。

**注意**：下载需要Linux主机可以访问互联网，并安装`git`。


## 编译运行Png Demo

操作步骤如下：

1. 将待转换的源png图片存入SD卡，默认路径为`/fatfs`，图片名为`liteos_png.png` 。
2. 将SD卡插入开发板的卡槽内。
3. 参照上述步骤使能Png Demo。
4. 在LiteOS源码根目录下执行`make clean ; make -j`命令编译LiteOS工程，编译成功后将在`out/STM32F769IDISCOVERY/lib`目录下生成组件库文件libpng.a和此Demo库文件libpng_demo.a，系统镜像文件为Huawei_LiteOS.bin。
5. 烧录系统镜像文件到开发板，复位后可以看到串口输出Demo运行结果如下所示。查看SD卡，可以看到转换后生成的图片文件`/fatfs/new_png.png`。

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
build data : Mar 13 2021 17:48:40

**********************************
osAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
Libpng demo task start to run.
file system mount success

Libpng demo task finished.

Huawei LiteOS #
```