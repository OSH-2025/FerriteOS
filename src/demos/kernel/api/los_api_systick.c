/*----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2013-2021. All rights reserved.
 * Description: LiteOS Kernel Systick Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2013-01-01
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

#include "los_api_systick.h"
#include "los_sys.h"
#include "los_task.h"
#include "los_inspect_entry.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define DEMO_NUM_OF_MS      1000
#define DEMO_NUM_OF_TICK    1000
#define DELAY_INTERVAL      200
#define USE_TICK            200

STATIC VOID TransformTime(VOID)
{
    UINT32 ms;
    UINT32 tick;

    tick = LOS_MS2Tick(DEMO_NUM_OF_MS);
    printf("LOS_MS2Tick tick = %d.\n", tick);
    ms = LOS_Tick2MS(DEMO_NUM_OF_TICK);
    printf("LOS_Tick2MS ms = %d.\n", ms);
}

UINT32 GetTickDemo(VOID)
{
    UINT32 ret;
    UINT32 cyclePerTick;
    UINT64 tickCount1;
    UINT64 tickCount2;

    printf("Kernel systick demo start to run.\n");

    /* conversion between tick and time */
    TransformTime();

    cyclePerTick  = LOS_CyclePerTickGet();
    if (cyclePerTick != 0) {
        printf("LOS_CyclePerTickGet = %d.\n", cyclePerTick);
    }

    tickCount1 = LOS_TickCountGet();
    if (tickCount1 != 0) {
        printf("LOS_TickCountGet = %d.\n", (UINT32)tickCount1);
    }
    LOS_TaskDelay(DELAY_INTERVAL);
    tickCount2 = LOS_TickCountGet();
    if (tickCount2 != 0) {
        printf("LOS_TickCountGet after delay = %d.\n", (UINT32)tickCount2);
    }

    if ((tickCount2 - tickCount1) >= USE_TICK) {
        ret = InspectStatusSetById(LOS_INSPECT_SYSTIC, LOS_INSPECT_STU_SUCCESS);
        if (ret != LOS_OK) {
            printf("Set inspect status failed.\n");
        }
        return LOS_OK;
    } else {
        ret = InspectStatusSetById(LOS_INSPECT_SYSTIC, LOS_INSPECT_STU_ERROR);
        if (ret != LOS_OK) {
            printf("Set inspect status failed.\n");
        }
        return LOS_NOK;
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
