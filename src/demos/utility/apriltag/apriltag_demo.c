/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Apriltag Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-11-03
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

#include "apriltag_demo.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <ctype.h>
#include <unistd.h>
#include <math.h>

#include "apriltag.h"
#include "tag36h11.h"

#include "common/getopt.h"
#include "common/image_u8.h"
#include "common/image_u8x4.h"
#include "common/pjpeg.h"
#include "common/zarray.h"

#include "los_memory_pri.h"
#include "los_task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define APRILTAG_TASK_PRIORITY    1
#define APRILTAG_TASK_STACK_SIZE  0x1000
#define TAG36H11_PATH             "/fatfs/tag36h11_20k_586.jpg"

STATIC UINT32 g_demoTaskId;

STATIC VOID ApriltagCreate(getopt_t **getOpt, apriltag_family_t **tf, apriltag_detector_t **td)
{
    *getOpt = getopt_create();
    *tf = tag36h11_create();
    *td = apriltag_detector_create();
}

STATIC VOID ApriltagDestory(getopt_t *getOpt, apriltag_detector_t *td, apriltag_family_t *tf)
{
    if (td != NULL) {
        apriltag_detector_destroy(td);
    }
    if (tf != NULL) {
        tag36h11_destroy(tf);
    }
    if (getOpt != NULL) {
        getopt_destroy(getOpt);
    }
}

STATIC VOID SetApriltagDetector(apriltag_detector_t *td, getopt_t *getopt)
{
    td->quad_decimate = getopt_get_double(getopt, "decimate");
    td->quad_sigma = getopt_get_double(getopt, "blur");
    td->nthreads = getopt_get_int(getopt, "threads");
    td->debug = getopt_get_bool(getopt, "debug");
    td->refine_edges = getopt_get_bool(getopt, "refine-edges");
}

STATIC VOID ApriltagGetOpt(getopt_t *getOpt)
{
    getopt_add_bool(getOpt, 'h', "help", 0, "Show this help");
    getopt_add_bool(getOpt, 'd', "debug", 0, "Enable debugging output (slow)");
    getopt_add_bool(getOpt, 'q', "quiet", 0, "Reduce output");
    getopt_add_string(getOpt, 'f', "family", "tag36h11", "Tag family to use");
    getopt_add_int(getOpt, 'i', "iters", "1", "Repeat processing on input set this many times");
    getopt_add_int(getOpt, 't', "threads", "1", "Use this many CPU threads");
    getopt_add_int(getOpt, 'a', "hamming", "1", "Detect tags with up to this many bit errors.");
    getopt_add_double(getOpt, 'x', "decimate", "2.0", "Decimate input image by this factor");
    getopt_add_double(getOpt, 'b', "blur", "0.0", "Apply low-pass blur to input; negative sharpens");
    getopt_add_bool(getOpt, '0', "refine-edges", 1, "Spend more time trying to align edges of tags");
}

STATIC VOID DemoTaskEntry(VOID)
{
    INT32 err = 0;
    image_u8_t *image = NULL;
    getopt_t *getOpt = NULL;
    pjpeg_t *pJpeg = NULL;
    zarray_t *detections = NULL;
    apriltag_family_t *tf = NULL;
    apriltag_detector_t *td = NULL;
    apriltag_detection_t *det = NULL;

    printf("Apriltag demo task start to run.\n");
    ApriltagCreate(&getOpt, &tf, &td);
    ApriltagGetOpt(getOpt);

    apriltag_detector_add_family_bits(td, tf, getopt_get_int(getOpt, "hamming"));
    SetApriltagDetector(td, getOpt);
    INT32 quiet = getopt_get_bool(getOpt, "quiet");
    pJpeg = pjpeg_create_from_file(TAG36H11_PATH, 0, &err);
    if (pJpeg == NULL) {
        ApriltagDestory(getOpt, td, tf);
        printf("Pjpeg create from file error %d\n", err);
        return;
    }
    printf("width = %d, height = %d\n", pJpeg->width, pJpeg->height);
    image = pjpeg_to_u8_baseline(pJpeg);
    if (pJpeg != NULL) {
        pjpeg_destroy(pJpeg);
    }
    if (image == NULL) {
        ApriltagDestory(getOpt, td, tf);
        return;
    }
    detections = apriltag_detector_detect(td, image);
    for (INT32 i = 0; i < zarray_size(detections); i++) {
        zarray_get(detections, i, &det);
        if (!quiet) {
            printf("detection %3d: id (%2dx%2d)-%-4d, hamming %d, margin %8.3f\n",
                   i, det->family->nbits, det->family->h, det->id, det->hamming, det->decision_margin);
	    }
    }

    if (image != NULL) {
        image_u8_destroy(image);
    }
    if (detections != NULL) {
        apriltag_detections_destroy(detections);
    }
    ApriltagDestory(getOpt, td, tf);
    printf("Apriltag demo task finished.\n");
}

VOID ApriltagDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = (UINT32)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.usTaskPrio = APRILTAG_TASK_PRIORITY;
    taskInitParam.pcName = "ApriltagDemoTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = APRILTAG_TASK_STACK_SIZE;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create apriltag demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
