/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Pixman Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-04-29
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

#include "pixman_demo.h"
#include "stdio.h"
#include "stdlib.h"
#include "los_task.h"
#include "utils.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define PIXMAN_TASK_STACK_SIZE   0x7000
#define PIXMAN_TASK_PRIORITY     6
#define WIDTH                    40
#define HEIGHT                   40
#define HALF                     20
#define QUARTER                  10
#define DEST_FILE                "/fatfs/pixman.png"

STATIC UINT32 g_demoTaskId;

STATIC INT32 DemoTaskEntry(VOID)
{
    pixman_image_t *img1 = NULL;
    pixman_image_t *img2 = NULL;
    pixman_image_t *img3 = NULL;
    pixman_image_t *img4 = NULL;
    pixman_image_t *dstImg = NULL;
    pixman_color_t color = {0x0000, 0x0000, 0xffff, 0x0000};
    const pixman_fixed_t rInner = 0;
    pixman_fixed_t rOuter;
    pixman_point_fixed_t center;
    pixman_point_fixed_t p1, p2, p3, p4;
    pixman_gradient_stop_t stops[] = {
        {0x00000, {0x0000, 0x0000, 0xffff, 0xffff} },
        {0x10000, {0xffff, 0x0000, 0xffff, 0xffff} }
    };

    printf("Pixman demo task start to run.\n");
    p1.x = p1.y = pixman_double_to_fixed(0);
    p2.x = p2.y = pixman_double_to_fixed(HALF);
    p3.x = p3.y = pixman_double_to_fixed(QUARTER);
    p4.x = p4.y = pixman_double_to_fixed(QUARTER);
    rOuter = pixman_double_to_fixed(QUARTER);
    center.x = center.y = 0;

    dstImg = pixman_image_create_bits(PIXMAN_x8r8g8b8, WIDTH, HEIGHT, NULL, -1);
    img1 = pixman_image_create_solid_fill(&color);
    img2 = pixman_image_create_linear_gradient(&p1, &p2, stops, ARRAY_LENGTH(stops));
    img3 = pixman_image_create_radial_gradient(&p3, &p4, rInner, rOuter, stops, ARRAY_LENGTH(stops));
    img4 = pixman_image_create_conical_gradient(&center, rInner, stops, ARRAY_LENGTH(stops));

    pixman_image_composite(PIXMAN_OP_SRC, img1, NULL, dstImg, 0, 0, 0, 0, 0, 0, HALF, HALF);
    pixman_image_composite(PIXMAN_OP_SRC, img2, NULL, dstImg, 0, 0, 0, 0, 0, HALF, HALF, HALF);
    pixman_image_composite(PIXMAN_OP_SRC, img3, NULL, dstImg, 0, 0, 0, 0, HALF, 0, HALF, HALF);
    pixman_image_composite(PIXMAN_OP_SRC, img4, NULL, dstImg, 0, 0, 0, 0, HALF, HALF, HALF, HALF);

    write_png(dstImg, DEST_FILE);

    printf("Pixman demo task finished.\n");

    pixman_image_unref(dstImg);
    pixman_image_unref(img1);
    pixman_image_unref(img2);
    pixman_image_unref(img3);
    pixman_image_unref(img4);

    return LOS_OK;
}

VOID PixmanDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = (UINT32)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = PIXMAN_TASK_STACK_SIZE;
    taskInitParam.pcName = "PixmanDemoTask";
    taskInitParam.usTaskPrio = PIXMAN_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create pixman demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
