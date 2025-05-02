# FreeType Demo说明文档
FreeType是一个C语言实现的面向对象的轻量级字体引擎，可以根据字体格式提取字符的字形数据。FreeType可以被轻易移植到各种嵌入式设备应用中。

## FreeType Demo简介
FreeType Demo的主要功能是加载某类字体库文件，然后显示字符的点阵模式数据。
目前支持运行`FreeType Demo`的开发板有以下几类：
- STM32F429
- STM32F769

`FreeType Demo`的调用入口在`targets/bsp/common/demo_entry.c`内，如下所示：
```c
#ifdef LOSCFG_DEMOS_FREETYPE
    FreeTypeDemoTask();
#endif
```

`FreeType Demo`的代码实现在`demos/utility/freetype/freetype_demo.c`文件中，如下所示，用户可在此扩展FreeType的功能。
```c
STATIC VOID DemoTaskEntry(VOID)
{
    FtHandle ftHandle;
    FtCharParam charParam;
    FT_Error ftError;
    INT32 ret;
    INT32 driver;
    charParam.charWidth = FT_CHAR_WIDTH;
    charParam.charHeight = FT_CHAR_HEIGHT;
    charParam.horzResolution = FT_HORZ_RESOLUTION;
    charParam.vertResolution = FT_VERT_RESOLUTION;

    printf("Freetype demo task start to run.\n");
    ret = memset_s(&ftHandle, sizeof(FtHandle), 0, sizeof(FtHandle));
    if (ret != EOK) {
        printf("Ft handle memset failed.\n");
        return;
    }
    driver = hal_fatfs_init(0);
    if (driver != LOS_OK) {
        (VOID)fatfs_unmount(FATFS_PATH, (UINT8)driver);
        printf("Fatfs init failed.\n");
        return;
    }
    ftError = LoadFtNewFace(&ftHandle, &charParam, TTF_FILE);
    if (ftError != LOS_OK) {
        printf("Load new face failed.\n");
        FtHandleFree(&ftHandle);
    }
    ShowFtBitmap(&ftHandle, 'H');
    FtHandleFree(&ftHandle);
    (VOID)fatfs_unmount(FATFS_PATH, (UINT8)driver);
    printf("Freetype demo task finished.\n");
}
```
```c
VOID FreeTypeDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S task_init_param = {0};
    
    task_init_param.usTaskPrio = FREETYPE_TASK_PRIORITY;
    task_init_param.pcName = "FreeTypeDemoTask";
    task_init_param.pfnTaskEntry = (TSK_ENTRY_FUNC)FreeTypeDemoEntry;
    task_init_param.uwStackSize = FREETYPE_TASK_STACK_SIZE;
    ret = LOS_TaskCreate(&g_freeTypeTaskHandle, &task_init_param);   // 创建FreeTypeDemoTask任务
    if (ret != LOS_OK) {
        printf("Freetype demo task create failed.\n");
    }
}
```

## 使能FreeType Demo

本文档以`Cloud_STM32F429IGTx_FIRE`开发板为例来演示FreeType的功能和效果。
首先设置`Cloud_STM32F429IGTx_FIRE`的默认配置，在Linux系统的LiteOS源码根目录下，执行如下命令：
```
~/Huawei_LiteOS$ cp tools/build/config/Cloud_STM32F429IGTx_FIRE.config .config
```

本Demo使用SD存储用于测试的字体文件，所以需要使能FATFS文件系统并设置为SD卡模式。无需手动使能FreeType组件，因为使能`FreeType Demo`后系统会自动使能`FreeType`。在LiteOS源码根目录下运行`make menuconfig`命令，按如下菜单路径使能各组件：
```
Components  --->
        FileSystem  --->
               -*- Enable FATFS (NEW)
                     Choose Peripheral Driver (SD Card)
Demos  --->
       Utility Demo  --->
               [*] Enable FreeType Demo (NEW)
```

保存退出后，LiteOS会从github自动下载FreeType源代码，并从gitee LiteOS组件仓库里下载对应的patch包，并打入patch，关于自动下载的详细流程，请查阅`components/download.sh`脚本。  

**注意**：下载需要Linux主机可以访问互联网，并安装`git`。

## 编译运行FreeType Demo

操作步骤如下：
1. 将`demos/utility/freetype`目录中用于测试的`inkree.ttf`字体文件，拷贝到SD卡的根目录内。

2. 将SD卡插入开发板的板载SD卡槽内。

3. 参照上述步骤使能FreeType Demo。

4. 在LiteOS源码根目录下执行`make clean ; make -j`命令编译LiteOS工程，编译成功后将在`out/Cloud_STM32F429IGTx_FIRE/lib`目录下生成组件库文件`libfreetype.a`和此Demo库文件`libfreetype_demo.a`，系统镜像文件为Huawei_LiteOS.bin。

5. 烧录系统镜像文件到开发板，程序运行时，通过加载字库文件`inkree.ttf`，根据传入的字符 `H` 参数，生成了该字符的点阵数据，由串口输出数据，可以看到串口输出Demo运行结果如下所示：

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
build data : Mar 13 2021 01:49:05

**********************************
osAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
Freetype demo task start to run.
file system mount success

FamilyName: freetype, GlyphsNum: 236, FacesNum: 1.
Input charcode is character ['H'].
Output bitmap data of charcode 'H':
1 1 1 1 1 0 0 0 1 1 1 1 1 0 0 1 
1 1 1 1 1 0 0 0 1 1 1 1 1 0 0 0 
1 1 1 1 1 0 0 1 1 1 1 1 1 0 0 0 
1 1 1 1 1 0 0 1 1 1 1 1 1 1 0 0 
1 1 1 1 1 0 0 1 1 1 1 1 1 1 0 0 
1 1 1 1 1 0 0 1 1 1 1 1 1 1 0 0 
1 1 1 1 0 0 0 1 1 1 1 1 1 1 0 0 
1 1 1 1 0 0 0 1 1 1 1 0 0 0 0 0 
1 1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 
1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 
1 1 1 0 0 0 1 1 1 1 1 1 1 0 0 1 
1 1 1 0 0 1 1 1 1 1 1 1 1 0 0 1 
1 1 0 0 0 1 1 1 1 1 1 1 0 0 0 1 
1 1 0 0 0 1 1 1 1 1 1 1 0 0 0 1 
1 1 0 0 0 1 1 1 1 1 1 1 0 0 1 1 
1 1 0 0 1 1 1 1 1 1 1 0 0 0 1 1 
1 0 0 0 1 1 1 1 1 1 1 0 0 0 1 1 
1 0 0 0 1 1 1 1 1 1 1 0 0 1 1 1 
1 0 0 0 1 1 1 1 1 1 1 0 0 1 1 1 
1 0 0 1 1 1 1 1 1 1 1 0 0 1 1 1 
1 0 0 1 1 1 1 1 1 1 0 0 0 1 1 1 
0 0 0 1 1 1 1 1 1 1 0 0 0 1 1 1 
1 0 0 1 1 1 1 1 1 1 0 0 0 1 1 1 

Freetype demo task finished.

Huawei LiteOS #
```
