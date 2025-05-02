/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Minmea Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-06-29
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

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#include "los_task.h"
#include "minmea.h"

#define MINMEA_TASK_PRIORITY   2
#define MINMEA_TASK_STACK_SIZE 2048

STATIC UINT32 g_demoTaskId;

STATIC VOID RMCDemo(VOID)
{
    printf("RMCDemo start.\n");
    UINT32 ret = MINMEA_INVALID;
    struct minmea_sentence_rmc rmcFrame = { 0 };
    CHAR *rmcData = "$GPRMC,062320,V,3537.8333,N,13944.6667,E,000.0,000.0,030222,,*0D";
    ret = minmea_sentence_id(rmcData);
    if (ret != MINMEA_SENTENCE_RMC) {
        printf("RMCDemo failed.\n");
        return;
    }

    if (minmea_parse_rmc(&rmcFrame, rmcData)) {
        printf("$xxRMC: raw coordinates and speed: (%d/%d,%d/%d) %d/%d\n", rmcFrame.latitude.value,
            rmcFrame.latitude.scale, rmcFrame.longitude.value, rmcFrame.longitude.scale, rmcFrame.speed.value,
            rmcFrame.speed.scale);
    }
}

STATIC VOID GGADemo(VOID)
{
    printf("GGADemo start.\n");
    UINT32 ret = MINMEA_INVALID;
    struct minmea_sentence_gga ggaFrame = { 0 };
    CHAR *ggaData = "$GPGGA,123204.00,5106.94086,N,01701.51680,E,1,06,3.86,127.9,M,40.5,M,,*51";
    ret = minmea_sentence_id(ggaData);
    if (ret != MINMEA_SENTENCE_GGA) {
        printf("GGADemo failed.\n");
        return;
    }

    if (minmea_parse_gga(&ggaFrame, ggaData)) {
        printf("$xxGGA: raw coordinates and fix quality: (%d/%d,%d/%d) %d/%d\n", ggaFrame.latitude.value,
            ggaFrame.latitude.scale, ggaFrame.longitude.value, ggaFrame.longitude.scale, ggaFrame.height.value,
            ggaFrame.height.scale);
    }
}

STATIC VOID GSTDemo(VOID)
{
    printf("GSTDemo start.\n");
    UINT32 ret = MINMEA_INVALID;
    struct minmea_sentence_gst gstFrame = { 0 };
    CHAR *gstData = "$GPGST,024603.00,3.2,6.6,4.7,47.3,5.8,5.6,22.0*58";
    ret = minmea_sentence_id(gstData);
    if (ret != MINMEA_SENTENCE_GST) {
        printf("GSTDemo failed.\n");
        return;
    }

    if (minmea_parse_gst(&gstFrame, gstData)) {
        printf("$xxGST: raw latitude,longitude and altitude error deviation: (%d/%d,%d/%d,%d/%d)\n",
            gstFrame.latitude_error_deviation.value, gstFrame.latitude_error_deviation.scale,
            gstFrame.longitude_error_deviation.value, gstFrame.longitude_error_deviation.scale,
            gstFrame.altitude_error_deviation.value, gstFrame.altitude_error_deviation.scale);
    }
}

STATIC VOID GSVDemo(VOID)
{
    printf("GSVDemo start.\n");
    UINT32 ret = MINMEA_INVALID;
    struct minmea_sentence_gsv gsvFrame = { 0 };
    CHAR *gsvData = "$GPGSV,4,1,13,02,28,259,33,04,12,212,27,05,34,305,30,07,79,138,*7F";
    ret = minmea_sentence_id(gsvData);
    if (ret != MINMEA_SENTENCE_GSV) {
        printf("GSVDemo failed.\n");
        return;
    }

    if (minmea_parse_gsv(&gsvFrame, gsvData)) {
        printf("$xxGSV: message %d of %d\n", gsvFrame.msg_nr, gsvFrame.total_msgs);
        printf("$xxGSV: sattelites in view: %d\n", gsvFrame.total_sats);
        for (int i = 0; i < 4; i++) {    // 4 : Gsv mode minmea_sat_info's array size.
            printf("$xxGSV: sat nr %d, elevation: %d, azimuth: %d, snr: %d dbm\n", gsvFrame.sats[i].nr,
                gsvFrame.sats[i].elevation, gsvFrame.sats[i].azimuth, gsvFrame.sats[i].snr);
        }
    }
}

STATIC VOID DemoTaskEntry(VOID)
{
    printf("Minmea demo task start to run.\n");
    RMCDemo();
    GGADemo();
    GSTDemo();
    GSVDemo();
    printf("Minmea demo task finished.\n");
}

VOID MinmeaDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = (UINT32)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = MINMEA_TASK_STACK_SIZE;
    taskInitParam.pcName = "MinmeaDemoTask";
    taskInitParam.usTaskPrio = MINMEA_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create minmea demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
