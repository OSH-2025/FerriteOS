/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Openssl Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-03-10
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

#include "openssl_demo.h"
#include "stdio.h"
#include "stdlib.h"
#include "los_base.h"
#include "los_task.h"
#include "los_config.h"
#include "los_typedef.h"
#include "openssl/md5.h"
#include "openssl/des.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define OPENSSL_TASK_PRIORITY      6
#define OPENSSL_TASK_STACK_SIZE    0x2000
#define MD5_ENCRYDATA              "MD5 test for liteos"
#define MD5_OUTLEN                 16
#define DESCBC_ENCRYDATA           "DES-CBC test for liteos"
#define DESCBC_DATALEN             23
#define SECTION_SIZE               8

STATIC UINT32 g_demoTaskId;

VOID MD5_Demo(VOID)
{
    UINT32 i;
    MD5_CTX md;
    UINT8 strOut[MD5_OUTLEN];

    (VOID)MD5_Init(&md);
    (VOID)MD5_Update(&md, MD5_ENCRYDATA, strlen(MD5_ENCRYDATA));
    (VOID)MD5_Final(strOut, &md);
    printf("Data:%s\nEncrypt:", MD5_ENCRYDATA);
    for (i = 0; i < MD5_OUTLEN; i++) {
        printf("%02X", strOut[i]);
    }
}

STATIC INT32 DES_CBC_Demo(VOID)
{
    INT32 i;
    INT32 ret;
    INT32 len;
    INT32 strKey = {0};
    DES_key_schedule ks;
    UINT8 src[] = DESCBC_ENCRYDATA;
    DES_cblock ivec;

    ret = memset_s(ivec, sizeof(ivec), 0, sizeof(ivec));
    if (ret != EOK) {
        return ret;
    }

    DES_set_key_unchecked((const_DES_cblock*)strKey, &ks);
    len = (DESCBC_DATALEN / SECTION_SIZE + (DESCBC_DATALEN % SECTION_SIZE ? 1 : 0)) * SECTION_SIZE;
    UINT8 dst[len];
    (void)memset_s(dst, sizeof(dst), 0, sizeof(dst));
    DES_cbc_encrypt(src, dst, (INT64)sizeof(src), &ks, (DES_cblock*)ivec, DES_ENCRYPT);
    printf("\nData:%s\nEncrypt:", DESCBC_ENCRYDATA);
    for (i = 0; i < len; i++) {
        printf("%02X", dst[i]);
    }
    ret = memset_s(src, sizeof(src), 0, sizeof(src));
    if (ret != EOK) {
        return ret;
    }
    DES_cbc_encrypt(dst, src, len, &ks, (DES_cblock*)ivec, DES_DECRYPT);
    printf("\nDecrypt:%s\n", src);

    return LOS_OK;
}

STATIC INT32 DemoTaskEntry(VOID)
{
    INT32 ret;

    printf("Openssl demo task start to run.\n");
    MD5_Demo();

    ret = DES_CBC_Demo();
    if (ret != LOS_OK) {
        printf("DES-CBC demo failed.\n");
    }

    printf("Openssl demo task finished.\n");
    return LOS_OK;
}

VOID OpensslDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = (UINT32)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = OPENSSL_TASK_STACK_SIZE;
    taskInitParam.pcName = "OpensslDemoTask";
    taskInitParam.usTaskPrio = OPENSSL_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create Openssl demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
