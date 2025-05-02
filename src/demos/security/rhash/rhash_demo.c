/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: RHash Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-08-10
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

#include "los_task.h"
#include "librhash/md5.h"
#include "librhash/sha256.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define RHASH_TASK_STACK_SIZE  0X1000
#define RHASH_TASK_PRIORITY    5

#define MD5_LEN                16
#define SHA256_LEN             32

#define DEMO_STR               "This is Huawei LiteOS RHash Demo.\n"
#define ORI_STR                "7990beea258031735be06254cea58a89"
#define SHA256_SUM             "d56910304664d3d3a439968b020bb465d5de33715cfb109d71704b91f85db30c"

STATIC UINT32 g_demoTaskId;

STATIC VOID RHashMd5Demo(VOID)
{
    UINT8 result[MD5_LEN] = {0};
    md5_ctx ctx = {0};
    rhash_md5_init(&ctx);
    rhash_md5_update(&ctx, (UINT8 *)DEMO_STR, strlen(DEMO_STR));
    rhash_md5_final(&ctx, result);
    printf("Original md5:\n%s\n", ORI_STR);
    printf("RHash calculate md5:\n");
    for (UINT32 i = 0; i < MD5_LEN; i++) {
        printf("%02x", (UINT32)result[i]);
    }
    printf("\n");
}

STATIC VOID RHashSha256Demo(VOID)
{
    UINT8 result[SHA256_LEN] = {0};
    sha256_ctx ctx = {0};
    rhash_sha256_init(&ctx);
    rhash_sha256_update(&ctx, (UINT8 *)DEMO_STR, strlen(DEMO_STR));
    rhash_sha256_final(&ctx, result);
    printf("Original sha256:\n%s\n", SHA256_SUM);
    printf("RHash calculate sha256:\n");
    for (UINT32 i = 0; i < SHA256_LEN; i++) {
        printf("%02x", (UINT32)result[i]);
    }
    printf("\n");
}

STATIC VOID DemoTaskEntry(VOID)
{
    printf("RHash demo start to run.\n");
    RHashMd5Demo();
    RHashSha256Demo();
    printf("RHash demo finished.\n");
}

VOID RhashDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = (UINT32)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = RHASH_TASK_STACK_SIZE;
    taskInitParam.pcName = "RhashDemoTask";
    taskInitParam.usTaskPrio = RHASH_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create rhash demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
