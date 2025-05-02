# CAN Demo说明文档

本demo仅支持野火STM32F429开发板，使用CAN总线需要硬件支持，在运行本demo前需要配置开发板硬件环境并配置代码中的发送/接收/筛选器标识符。

对于其他STM32开发板，可以参考本demo进行适配修改。

## CAN Demo解析

本demo主要包括如下代码：

[1.CAN总线硬件初始化](#1)

[2.配置发送/接收/筛选器](#2)

[3.创建CAN总线发送任务](#3)

[4.调用中断接收函数](#4)

<h3 id="1">1.CAN总线硬件初始化</h3>

CAN总线的硬件初始化函数`MX_CAN1_Init()`定义在[can.c](/targets/Cloud_STM32F429IGTx_FIRE/Src/can.c)文件中，函数声明在[can.h](/targets/Cloud_STM32F429IGTx_FIRE/Inc/can.h)文件中，这两个文件通过STM32CubeMX软件生成。
并在开发板[main.c](/targets/Cloud_STM32F429IGTx_FIRE/Src/main.c)文件的硬件初始化函数`HardwareInit()`中调用该CAN总线初始化函数。

<h3 id="2">2.配置发送/接收/筛选器</h3>

[can_demo.c](/demos/drivers/can/can_demo.c)文件中配置了CAN总线的发送、接收、筛选。

默认配置如下：

```c
#define CAN_TX_STDID            0x00                    /* 发送标准标识符0x00 */
#define CAN_RX_STDID            0x00                    /* 接收标准标识符0x00 */
#define CAN_TX_EXTID            0x1234                  /* 发送扩展标识符0x1234 */
#define CAN_RX_EXTID            0x1234                  /* 接收扩展标识符0x1234 */
#define CAN_RX_FILERID          0x1234                  /* 筛选器的扩展标识符0x1234 */
#define CAN_TXRX_DATE_SIZE      8                       /* 发送接收的数据长度 */
#define CAN_RX_FILERMASK_ALL    0xffffffff              /* 筛选器筛选所有位，表示只识别CAN_RX_FILERID */
#define CAN_RX_FILERMASK_NONE   0x00000000              /* 筛选器筛选为空，表示识别所有ID */
#define CAN_RX_FILERMASK        CAN_RX_FILERMASK_NONE   /* 筛选器要筛选的标识位 */
```

**注意：**
- 本demo在CAN发送配置函数`CAN_SendConfig()`和接收配置函数`CAN_ReceiveConfig()`中配置为使用扩展标识符（CAN_ID_EXT），所以识别的是扩展标识符，标准标识符可以直接设置为0x00。

- 本demo在CAN总线筛选器配置函数`CAN_FilterConfig()`中配置筛选器为列表模式（CAN_FILTERMODE_IDMASK），即筛选器会存储筛选的ID。

用户也可以参考下列结构体成员变量的含义对该demo的代码进行修改（下列结构体注释参考《STM32 HAL 库开发实战指南—基于F429_挑战者V2》）。

筛选器结构体：

```c
typedef struct {
    uint32_t FilterIdHigh;          /* CAN_FxR1 寄存器的高16 位 */
    uint32_t FilterIdLow;           /* CAN_FxR1 寄存器的低16 位 */
    uint32_t FilterMaskIdHigh;      /* CAN_FxR2 寄存器的高16 位 */
    uint32_t FilterMaskIdLow;       /* CAN_FxR2 寄存器的低16 位 */
    uint32_t FilterFIFOAssignment;  /* 设置经过筛选后数据存储到哪个接收FIFO */
    uint32_t FilterNumber;          /* 筛选器编号，范围0-27 */
    uint32_t FilterMode;            /* 筛选器模式 */
    uint32_t FilterScale;           /* 设置筛选器的尺度 */
    uint32_t FilterActivation;      /* 是否使能本筛选器 */
    uint32_t BankNumber;            /* 扇区序号 */
} CAN_FilterInitTypeDef;
```

发送结构体：

```c
typedef struct {
    uint32_t StdId;     /* 存储报文的标准标识符11 位，0-0x7FF */
    uint32_t ExtId;     /* 存储报文的扩展标识符29 位，0-0x1FFFFFFF */
    uint8_t IDE;        /* 存储IDE 扩展标志 */
    uint8_t RTR;        /* 存储RTR 远程帧标志 */
    uint8_t DLC;        /* 存储报文数据段的长度，0-8 */
    uint8_t Data[8];    /* 存储报文数据段的内容 */
} CanTxMsgTypeDef;
```

接收结构体：

```c
typedef struct {
    uint32_t StdId;     /* 存储了报文的标准标识符11 位，0-0x7FF */
    uint32_t ExtId;     /* 存储了报文的扩展标识符29 位，0-0x1FFFFFFF */
    uint8_t IDE;        /* 存储了IDE 扩展标志 */
    uint8_t RTR;        /* 存储了RTR 远程帧标志 */
    uint8_t DLC;        /* 存储了报文数据段的长度，0-8 */
    uint8_t Data[8];    /* 存储了报文数据段的内容 */
    uint8_t FMI;        /* 存储了 本报文是由经过筛选器存储进FIFO 的，0-0xFF */
    uint8_t FIFONumber; /* 配置接收FIFO 编号，可以是CAN_FIFO0 或者CAN_FIFO1 */
} CanRxMsgTypeDef;
```

<h2 id="3">3.创建CAN总线发送任务</h2>

CAN总线循环发送任务`CanDemoTask`的任务入口函数为`DemoTaskEntry()`，定义在[can_demo.c](/demos/drivers/can/can_demo.c)文件中，该任务通过`HAL_CAN_AddTxMessage()`函数向指定扩展标识符发送数据。

```c
HAL_CAN_AddTxMessage(&hcan1, &g_demoTxHeader, txData, &txMailbox);
```

<h2 id="4">4.调用中断接收函数</h2>

[can_demo.c](/demos/drivers/can/can_demo.c)文件的`CAN_HwiCreate()`函数使能并创建CAN总线中断。

同时参考了hal库代码，重定向hal库的CAN总线中断回调函数`HAL_CAN_RxFifo0MsgPendingCallback()`。

在`HAL_CAN_RxFifo0MsgPendingCallback()`函数中使用`HAL_CAN_GetRxMessage()`函数接收CAN总线收到的数据。

```c
VOID HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
    UINT8 rxData[CAN_TXRX_DATE_SIZE];
    INT32 i;
    if (hcan->Instance == CAN1) {
        HAL_CAN_GetRxMessage(&hcan1, CAN_RX_FIFO0, &g_demoRxHeader, rxData);
        printf("RxID:0x%lx RxData:", g_demoRxHeader.ExtId);
        for (i = 0; i < CAN_TXRX_DATE_SIZE; i++) {
            printf("%02x ", rxData[i]);
        }
        printf("\n");
    }
}
```

## 运行CAN Demo

### 硬件配置

本demo需要使用两块野火STM32F429开发板，或者一块野火STM32F429开发板和一个CAN总线分析仪。

#### 两块野火STM32F429开发板的硬件连接方式

- CAN总线连接：将两块开发板引出的CANH和CANL两条总线的端子一一对应连接。

- CAN收发器供电连接：将CAN接线端子旁边的“CAN/485_3V3”使用跳线帽和“3V3”排针连接。

#### 使用一块野火STM32F429开发板和一个CAN总线分析仪：

- CAN总线连接：将开发板引出的CANH和CANL两条总线连接到CAN总线分析仪上，并用串口将CAN总线分析仪连接到电脑上（CAN总线分析仪使用请参考其使用指南）。

- CAN收发器供电连接：将CAN接线端子旁边的“CAN/485_3V3”使用跳线帽和“3V3”排针连接。

**说明：**

- 本文档使用的是两块野火STM32F429开发板相连的硬件环境。

- 后面的操作步骤，包括使能和编译运行CAN Demo，需要在两块开发板上同时进行。

### 使能CAN Demo

1.在LiteOS源码根目录下拷贝tools/build/config/Cloud_STM32F429IGTx_FIRE.config到根目录，并重命名为.config。

2.在LiteOS源码根目录下执行`make menuconfig`命令，按如下菜单路径使能CAN Demo并保存退出。

```
Demos  --->
        Drivers Demo  --->
                [*] Enable Can Demo
```

### 编译运行CAN Demo

1.使能CAN Demo后，在LiteOS源码根目录下执行`make clean; make -j`命令编译LiteOS工程，编译成功后会生成系统镜像文件`Huawei_LiteOS.bin`。

2.烧录系统镜像文件到开发板中，复位重启开发板后，可以看到如下运行结果。

```
********Hello Huawei LiteOS********

LiteOS Kernel Version : 5.0.0
build data : Mar 30 2021 10:26:28

**********************************
osAppInit
cpu 0 entering scheduler
app init!
Hello, welcome to liteos demo!

Huawei LiteOS # Can bus demo task start to run.
TxID:0x1234 TxData:00 01 02 03 04 05 06 07 
RxID:0x1234 RxData:00 01 02 03 04 05 06 07 
TxID:0x1234 TxData:00 01 02 03 04 05 06 07 
RxID:0x1234 RxData:00 01 02 03 04 05 06 07 
TxID:0x1234 TxData:00 01 02 03 04 05 06 07 
RxID:0x1234 RxData:00 01 02 03 04 05 06 07 
TxID:0x1234 TxData:00 01 02 03 04 05 06 07 
RxID:0x1234 RxData:00 01 02 03 04 05 06 07 
TxID:0x1234 TxData:00 01 02 03 04 05 06 07 
RxID:0x1234 RxData:00 01 02 03 04 05 06 07 
TxID:0x1234 TxData:00 01 02 03 04 05 06 07 
RxID:0x1234 RxData:00 01 02 03 04 05 06 07 
TxID:0x1234 TxData:00 01 02 03 04 05 06 07 
RxID:0x1234 RxData:00 01 02 03 04 05 06 07 
TxID:0x1234 TxData:00 01 02 03 04 05 06 07 
RxID:0x1234 RxData:00 01 02 03 04 05 06 07 
TxID:0x1234 TxData:00 01 02 03 04 05 06 07 
RxID:0x1234 RxData:00 01 02 03 04 05 06 07 
TxID:0x1234 TxData:00 01 02 03 04 05 06 07 
RxID:0x1234 RxData:00 01 02 03 04 05 06 07 
Can bus demo task finished.
```

运行结果中的`TxID`为本开发板的发送扩展标识符CAN_TX_EXTID，`TxData`为本开发板发送的数据。`RxID`是与本开发板连接的另一块开发板的发送扩展标识符CAN_TX_EXTID，`RxData`为本开发板接收到的数据，即另一块开发板发送给本开发板的数据。
