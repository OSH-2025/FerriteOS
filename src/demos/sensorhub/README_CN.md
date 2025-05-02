# SENSORHUB说明文档
Sensorhub智能传感集线器的主要功能是连接并处理来自各种传感器设备的数据， Sensorhub Demo展示了其使用示例。   
本文使用`Cloud_STM32F429IGTx_FIRE开发板`和内置的`mpu6050加速陀螺仪`介绍LiteOS的sensorhub的使用。

## Sensorhub Demo 配置及简介
### menuconfig配置
通过`make menuconfig`开启sensorhub组件及demo编译选项
```c
    Targets  --->
        Target (Cloud_STM32F429IGTx_FIRE)  --->
                                           (X) Cloud_STM32F429IGTx_FIRE
    Components --> 
               Sensorhub --> 
                         [*] Enable Sensorhub
    Demos --> 
          Sensorhub Demo --> 
                           *** only support Cloud_STM32F429IGTx_FIRE ***
                         [*] Enable Sensorhub Demo

```
### Sensorhub Demo代码简介
开启Sensorhub Demo示例功能入口在 `targets/bsp/common/demo_entry.c`内。
```c
#ifdef LOSCFG_DEMOS_SENSORHUB
    SensorHubDemoTask();
#endif
```
Sensorhub Demo的实现功能在`demos\sensorhub\gyro\src\sensorhub_demo.c`中。
```c
STATIC VOID DemoTaskEntry(VOID)
{
    printf("SensorHub demo task start to run.\n");
    MX_I2C1_Init();
    SensorManagerInit();
    LOS_TaskDelay(1000);
    GyroSensorRegister();
    InitGyro();
    OpenGyro();
    LOS_TaskDelay(20000);
    CloseGyro();
    OpenGyro();
    LOS_TaskDelay(100000);
    CloseGyro();
    printf("SensorHub demo task finished.\n");
}

VOID SensorHubDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam ;

    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = SENSORHUB_TASK_STACK_SIZE;
    taskInitParam.pcName = "SensorHubDemoTask";
    taskInitParam.usTaskPrio = SENSORHUB_TASK_PRIORITY;  /* 1~7 */
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;   /* task is detached, the task can deleteself */
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create sensorhub demo task failed.\n");
    }
}
```
以上所述是调用ensorhub Demo示例的主要内容，参照上述menuconfig配置之后，经编译后，固件为`out/Cloud_STM32F429IGTx_FIRE/Huawei_LiteOS.bin`。 
## Sensorhub Demo测试
把编译生成的固件烧录到开发板内， 串口输出mpu6050传感器实时数据，运行效果如下所示：   
```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
build data : Mar 13 2021 00:43:03

**********************************
osAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!
SensorHub demo task start to run.

Huawei LiteOS #
Gypo init.
Get mpu6050 id = 104
Read gypo id successflly.
Gyro on.
Read data
Acc:      -398     336   15566
Gyro:      -14     -24      -3
Temp:       30
Tag 2 report

Tag 2 report

Read data
Acc:      -392     350   15560
Gyro:      -14     -25      -3
Temp:       30
Tag 2 report

Tag 2 report

Read data
Acc:      2368     352   15384
Gyro:       -7     214     -11
Temp:       30
Tag 2 report

Read data
Acc:      -430     268   15614
Gyro:      -15     -23      -3
Temp:       30
Tag 2 report

Tag 2 report

Read data
Acc:      -428     250   15570
Gyro:      -14     -24      -3
Temp:       30
Tag 2 report
```
