/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Tinycrypt Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-06-20
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

#include "tinycrypt_demo.h"
#include <tinycrypt/cbc_mode.h>
#include <tinycrypt/constants.h>
#include "los_task.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define TINYCRYPT_TASK_PRIORITY      6
#define TINYCRYPT_TASK_STACK_SIZE    0x2000
/* must be divisible by TC_AES_BLOCK_SIZE and greater than it */
#define PLAINTEXT_LEN                32
#define TC_LEN                       16

STATIC UINT32 g_demoTaskId;

/* Encryption and decryption key, can define by yourself */
const UINT8 g_demoKey[TC_LEN] = {
    0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01,
    0x02, 0x02, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03
};

/* Encrypted index, can be any content */
const UINT8 g_demoIv[TC_LEN] = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f
};

const UINT8 g_demoPlainText[PLAINTEXT_LEN] = "hello liteOS !";

/* NIST SP 800-38a CBC Test for encryption and decryption. */
STATIC INT32 EncryptionAndDecryption(VOID)
{
    struct tc_aes_key_sched_struct a = { 0 };
    UINT8 encrypted[TC_LEN + PLAINTEXT_LEN] = { 0 };
    UINT8 decrypted[PLAINTEXT_LEN] = { 0 };
    UINT8 *p = NULL;
    UINT32 length;
    INT32 i;

    (VOID)tc_aes128_set_encrypt_key(&a, g_demoKey);

    if (tc_cbc_mode_encrypt(encrypted, sizeof(g_demoPlainText) + TC_LEN,
        g_demoPlainText, sizeof(g_demoPlainText), g_demoIv, &a) == 0) {
        printf("CBC entrypte failed.\n");
        return LOS_NOK;
    }
    printf("encrypted = [");
    for (i = 0; i < sizeof(encrypted); i++) {
        printf("%x", encrypted[i]);
    }
    printf("]\n");

    (VOID)tc_aes128_set_decrypt_key(&a, g_demoKey);

    p = &encrypted[TC_AES_BLOCK_SIZE];
    length = ((UINT32)sizeof(encrypted)) - TC_AES_BLOCK_SIZE;

    if (tc_cbc_mode_decrypt(decrypted, length - TC_AES_BLOCK_SIZE, p, length, encrypted, &a) == 0) {
        printf("CBC decrypte failed.\n");
        return LOS_NOK;
    }
    printf("decrypted = [");
    for (i = 0; i < PLAINTEXT_LEN; i++) {
        if (decrypted[i] == g_demoPlainText[i]) {
            printf("%c", decrypted[i]);
        } else {
            printf("CBC decrypte failed.\n");
            return LOS_NOK;
        }
    }
    printf("]\n");

    return LOS_OK;
}

STATIC VOID DemoTaskEntry(VOID)
{
    INT32 result;

    printf("Tinycrypt demo task start to run.\n");

    result = EncryptionAndDecryption();
    if (result < 0) {
        printf("CBC encrypted or decrypted failed.\n");
        return;
    }

    printf("Tinycrypt demo task finished.\n");
}

VOID TinycryptDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = (UINT32)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = TINYCRYPT_TASK_STACK_SIZE;
    taskInitParam.pcName = "TinycryptDemoTask";
    taskInitParam.usTaskPrio = TINYCRYPT_TASK_PRIORITY;
    taskInitParam.uwResved = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create tinycrypt demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
