/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Opus Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-02-20
 * Redistribution and use inputBuf source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright notice, this list of
 * conditions and the following disclaimer.
 * 2. Redistributions inputBuf binary form must reproduce the above copyright notice, this list
 * of conditions and the following disclaimer inputBuf the documentation and/or other materials
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

#include "opus_demo.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "opus.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define OPUS_DEMO_CHANNELS          1
#define OPUS_DEMO_INPUT_SIZE        137
#define OPUS_DEMO_FRAMESIZE         OPUS_DEMO_INPUT_SIZE
#define OPUS_DEMO_FRAME_MASK        0xFFFF
#define OPUS_DEMO_INPUT_MASK        0xFF
#define OPUS_DEMO_SAMPLING_RATE     8000
#define OPUS_TASK_PRIORITY          4
#define OPUS_TASK_STACK_SIZE        0x2000

STATIC UINT32 g_demoTaskId;

UINT8 g_inputBuf[] = {
    0x00, 0xE0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00,
    0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
    0xE0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
    0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00,
    0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xE0, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xE0,
    0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0xE0
};

STATIC INT32 DemoTaskEntry(VOID)
{
    INT32 ret;
    INT32 error;
    INT32 i;
    INT16 *outputBuf = NULL;
    OpusDecoder *decoder = NULL;

    printf("Opus demo task start to run.\n");
    printf("Opus demo input data:\n");
    for (i = 0; i < OPUS_DEMO_INPUT_SIZE; i++) {
        printf("0x%02x ", (UINT32)(g_inputBuf[i] & OPUS_DEMO_INPUT_MASK));
    }
    printf("\n");

    outputBuf = (INT16 *)malloc(OPUS_DEMO_FRAMESIZE * OPUS_DEMO_CHANNELS * sizeof(INT16));
    if (outputBuf == NULL) {
        printf("Opus demo malloc failed.\n");
        return LOS_NOK;
    }
    ret = memset_s(outputBuf, OPUS_DEMO_FRAMESIZE * sizeof(INT16), 0, OPUS_DEMO_FRAMESIZE * sizeof(INT16));
    if (ret != EOK) {
        free(outputBuf);
        return ret;
    }

    decoder = opus_decoder_create(OPUS_DEMO_SAMPLING_RATE, OPUS_DEMO_CHANNELS, &error);
    if (decoder == NULL) {
        printf("Opus decoder creat failed.\n");
        free(outputBuf);
        return LOS_NOK;
    }
    ret = opus_decode(decoder, g_inputBuf, OPUS_DEMO_INPUT_SIZE, outputBuf, OPUS_DEMO_FRAMESIZE, 0);
    if (ret < 0) {
        printf("Opus demo decode failed.\n");
        opus_decoder_destroy(decoder);
        free(outputBuf);
        return LOS_NOK;
    }

    printf("Opus demo output data:\n");
    for (i = 0; i < OPUS_DEMO_FRAMESIZE; i++) {
        printf("0x%04x ", (UINT32)(((UINT16)outputBuf[i]) & OPUS_DEMO_FRAME_MASK));
    }
    printf("\n");

    opus_decoder_destroy(decoder);
    free(outputBuf);
    printf("Opus demo decode seccussfully.\n");
    printf("Opus demo task finished.\n");

    return LOS_OK;
}

VOID OpusDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = OPUS_TASK_STACK_SIZE;
    taskInitParam.pcName = "OpusDemoTask";
    taskInitParam.usTaskPrio = OPUS_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create Opus demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
