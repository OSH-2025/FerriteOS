/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Can Send Receive Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-03-24
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer in the documentation and/or other materials
 * provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its contributors may be used
 * to endorse or promote products derived from this software without specific prior written
 * permission.
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --------------------------------------------------------------------------- */

#include "can_demo.h"
#include <los_hwi.h>
#include "stm32f4xx_hal.h"
#include "los_task.h"
#include "can.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define CAN_TX_STDID            0x00
#define CAN_RX_STDID            0x00
#define CAN_TX_EXTID            0x1234
#define CAN_RX_EXTID            0x1234
#define CAN_RX_FILERID          0x1234
#define CAN_TXRX_DATE_SIZE      8
#define CAN_RX_FILERMASK_ALL    0xffffffff
#define CAN_RX_FILERMASK_NONE   0x00000000
#define CAN_RX_FILERMASK        CAN_RX_FILERMASK_NONE
#define CAN_GET_HIGHBIT         16
#define CAN_TASK_PRIORITY       6
#define CAN_RX_IRQ_NUM          (CAN1_RX0_IRQn + 16)
#define DELAY_INTERVAL          1000
#define TASK_CYCLETIMES         10

CAN_FilterTypeDef   g_demoFilterConfig;
CAN_TxHeaderTypeDef g_demoTxHeader;
CAN_RxHeaderTypeDef g_demoRxHeader;
STATIC UINT32 g_demoTaskId;

VOID CAN_FilterConfig(VOID)
{
    /* Register map: 0: reserved; 1: RTR flag; 2: IDE flag; 3-31: packet ID. */
    g_demoFilterConfig.FilterIdHigh = ((((UINT32)CAN_RX_FILERID << 3) | CAN_ID_EXT |
                                       CAN_RTR_DATA) & 0xFFFF0000) >> CAN_GET_HIGHBIT;
    /* Register map: 0: reserved; 1: RTR flag; 2: IDE flag; 3-31: packet ID. */
    g_demoFilterConfig.FilterIdLow = (((UINT32)CAN_RX_FILERID << 3) | CAN_ID_EXT | CAN_RTR_DATA) & 0xFFFF;
    g_demoFilterConfig.FilterMaskIdHigh = (CAN_RX_FILERMASK & 0xffff0000) >> CAN_GET_HIGHBIT;
    g_demoFilterConfig.FilterMaskIdLow = CAN_RX_FILERMASK & 0x0000ffff;
    g_demoFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    g_demoFilterConfig.FilterBank = 0;
    g_demoFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    g_demoFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    g_demoFilterConfig.FilterActivation = ENABLE;
    g_demoFilterConfig.SlaveStartFilterBank = 0;
    HAL_CAN_ConfigFilter(&hcan1, &g_demoFilterConfig);
}

VOID CAN_SendConfig(VOID)
{
    g_demoTxHeader.StdId = CAN_TX_STDID;
    g_demoTxHeader.ExtId = CAN_TX_EXTID;
    g_demoTxHeader.RTR = CAN_RTR_DATA;
    g_demoTxHeader.IDE = CAN_ID_EXT;
    g_demoTxHeader.DLC = CAN_TXRX_DATE_SIZE;
    g_demoTxHeader.TransmitGlobalTime = DISABLE;
}

VOID CAN_ReceiveConfig(VOID)
{
    g_demoRxHeader.StdId = CAN_RX_STDID;
    g_demoRxHeader.ExtId = CAN_RX_EXTID;
    g_demoRxHeader.RTR = CAN_RTR_DATA;
    g_demoRxHeader.IDE = CAN_ID_EXT;
    g_demoRxHeader.DLC = CAN_TXRX_DATE_SIZE;
}

VOID CAN1_RX0_IRQHandler(VOID)
{
    HAL_CAN_IRQHandler(&hcan1);
}

VOID CAN_HwiCreate(VOID)
{
    LOS_HwiEnable(CAN_RX_IRQ_NUM);
    LOS_HwiCreate(CAN_RX_IRQ_NUM, 0, 0, CAN1_RX0_IRQHandler, NULL);
    HAL_CAN_ActivateNotification(&hcan1, CAN_IT_RX_FIFO0_MSG_PENDING);
}

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

VOID DemoTaskEntry(VOID)
{
    UINT8 txData[CAN_TXRX_DATE_SIZE];
    UINT32 txMailbox;
    INT32 count = 0;
    INT32 i, j;

    LOS_TaskDelay(DELAY_INTERVAL);
    printf("Can bus demo task start to run.\n");
    HAL_CAN_Start(&hcan1);
    for (i = 0; i < TASK_CYCLETIMES; i++) {
        for (j = 0; j < CAN_TXRX_DATE_SIZE; j++) {
            txData[j] = j;
        }
        HAL_CAN_AddTxMessage(&hcan1, &g_demoTxHeader, txData, &txMailbox);

        while (HAL_CAN_IsTxMessagePending(&hcan1, txMailbox)) {
            __NOP();
            count++;
            /* Wait until the CAN bus is sent. */
            /* After the nop instruction is executed for 10000 times, the sending stops due to timeout. */
            if (count > 10000) {
                break;
            }
        }
        printf("TxID:0x%lx TxData:", g_demoTxHeader.ExtId);
        for (j = 0; j < CAN_TXRX_DATE_SIZE; j++) {
            printf("%02x ", txData[j]);
        }
        printf("\n");
        count = 0;
        LOS_TaskDelay(DELAY_INTERVAL);
    }
    LOS_HwiDisable(CAN_RX_IRQ_NUM);
    printf("Can bus demo task finished.\n");
}

VOID CanDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    CAN_FilterConfig();
    CAN_SendConfig();
    CAN_ReceiveConfig();
    CAN_HwiCreate();
    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    taskInitParam.pcName = "CanDemoTask";
    taskInitParam.usTaskPrio = CAN_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create can bus demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
