/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Openexif Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-04-01
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

#include "openexif_demo.h"
#include "ExifImageFile.h"
#include <iomanip>
#ifdef LOSCFG_LIB_CPP_EXTEND
#include <thread>
#endif /* LOSCFG_LIB_CPP_EXTEND */

#ifndef LOSCFG_LIB_CPP_EXTEND
#include "los_task.h"

#define OPENEXIF_TASK_STACK_SIZE    0x1000
#define OPENEXIF_TASK_PRIORITY      5

static uint32_t g_demoTaskId;
#endif /* LOSCFG_LIB_CPP_EXTEND */

using namespace std;

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

const char *g_demoFilename = "/fatfs/test.jpg";

static void DemoTaskEntry(void)
{
    cout << "Openexif demo task start to run." << endl;

    ExifImageFile demoFile;
    uint32_t ret = demoFile.open(g_demoFilename, "r");
    if (ret != EXIF_OK) {
        cout << "Error: could not open " << g_demoFilename << " with ExifImageFile." << endl;
        return;
    }

    cout << "\"Exif\" entries:" << endl;
    ExifPathsTags pathTag;
    /* 0xFFE1: application segment marker */
    ret = demoFile.getAllTags(0xFFE1, "Exif", pathTag);
    if (ret != EXIF_OK) {
        cout << "Error: could not get tags from exif." << endl;
        return;
    }

    ExifPathsTagsIter pathTagIter;
    for (pathTagIter = pathTag.begin(); pathTagIter != pathTag.end(); ++pathTagIter) {
        ExifIFDPathIter ifdPathIter;
        for (ifdPathIter = (*pathTagIter).first.begin(); ifdPathIter != (*pathTagIter).first.end(); ++ifdPathIter) {
            cout << "IFD: " << (*ifdPathIter).first;
            cout << "  Idx: " << (*ifdPathIter).second << endl;
        }

        cout << "Tag#\tType\tCount\tValue" << endl;
        ExifTagsIter tagIter;
        for (tagIter = (*pathTagIter).second.begin(); tagIter != (*pathTagIter).second.end(); ++tagIter) {
            ExifTagEntry *tag = *(tagIter);
            tag->print();
            cout << endl;
        }
    }

    ret = demoFile.close();
    if (ret != EXIF_OK) {
        cout << "Error: Could not close" << g_demoFilename << endl;
        return;
    }
    cout << "Openexif demo task finished." << endl;
}


void OpenExifDemoTask(void)
{
#ifdef LOSCFG_LIB_CPP_EXTEND
    std::thread openExifDemo(DemoTaskEntry);
    openExifDemo.join();
#else
    uint32_t ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = OPENEXIF_TASK_STACK_SIZE;
    taskInitParam.pcName = "OpenExifDemoTask";
    taskInitParam.usTaskPrio = OPENEXIF_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        cout << "Create openexif demo task failed." << endl;
    }
#endif /* LOSCFG_LIB_CPP_EXTEND */
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
