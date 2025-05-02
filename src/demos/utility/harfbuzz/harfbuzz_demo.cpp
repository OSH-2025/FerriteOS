/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Harfbuzz Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-03-26
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

#include "harfbuzz_demo.h"
#include <iostream>
#include <cassert>
#include "hb.hh"

#ifdef HAVE_FREETYPE
#include "hb-ft.h"
#endif

#ifdef HB_NO_OPEN
#define hb_blob_create_from_file(x)  hb_blob_get_empty ()
#endif

#ifdef LOSCFG_LIB_CPP_EXTEND
#include <thread>
#endif /* LOSCFG_LIB_CPP_EXTEND */

#ifndef LOSCFG_LIB_CPP_EXTEND
#include "los_task.h"

#define HARFBUZZ_TASK_PRIORITY 7
#define HARFBUZZ_TASK_STACK    2048

STATIC UINT32 g_harfbuzzTaskID;
#endif

using namespace std;

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

const CHAR *FILE_TTF = "/fatfs/inkree.ttf";
#define HB_ERRNO           -1
const CHAR *UTF8_STRING = "123abcABCxyzXYZ";

STATIC VOID DemoTaskEntry(VOID)
{
    cout << "Harfbuzz demo start to run." << endl;
    UINT32 i = 0;
    hb_blob_t *blob = hb_blob_create_from_file(FILE_TTF);
    cout << "opened font file : " << FILE_TTF;
    cout << " bytes long : " << hb_blob_get_length(blob) << endl;

    /* Create the face */
    hb_face_t *face = hb_face_create(blob, 0);
    hb_blob_destroy(blob);
    UINT32 upem = hb_face_get_upem(face);

    hb_font_t *font = hb_font_create(face);
    hb_font_set_scale(font, (INT32)upem, (INT32)upem);

#ifdef HAVE_FREETYPE
    hb_ft_font_set_funcs(font);
#endif

    hb_buffer_t *buffer = hb_buffer_create();

    hb_buffer_add_utf8(buffer, UTF8_STRING, HB_ERRNO, 0, HB_ERRNO);
    hb_buffer_guess_segment_properties(buffer);

    hb_shape(font, buffer, nullptr, 0);

    UINT32 count = hb_buffer_get_length(buffer);
    hb_glyph_info_t *infos = hb_buffer_get_glyph_infos(buffer, nullptr);
    hb_glyph_position_t *positions = hb_buffer_get_glyph_positions(buffer, nullptr);
    for (i = 0; i < count; i++) {
        hb_glyph_info_t *info = &infos[i];
        hb_glyph_position_t *pos = &positions[i];
        cout << "cluseter " << info->cluster << " glyph " << hex << info->codepoint << " at ";
        cout << "(" << pos->x_offset<<"," << pos->y_offset << ") + ";
        cout << "(" << dec << pos->x_advance << "," << pos->y_advance << ")" << endl;
    }

    hb_buffer_destroy(buffer);
    hb_font_destroy(font);
    hb_face_destroy(face);
    cout << "Harfbuzz demo finished." << endl;
}

VOID HarfbuzzDemoTask(VOID)
{
#ifdef LOSCFG_LIB_CPP_EXTEND
    thread harfbuzzDemo(DemoTaskEntry);
    harfbuzzDemo.join();
#else
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam = {0};
    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }

    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = HARFBUZZ_TASK_STACK;
    taskInitParam.pcName = (CHAR *)"HarfbuzzDemoTask";
    taskInitParam.usTaskPrio = HARFBUZZ_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_harfbuzzTaskID, &taskInitParam);
    if (ret != LOS_OK) {
        cout << "Create harfbuzz demo task failed." << endl;
    }
#endif
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
