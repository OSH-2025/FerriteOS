/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Membox Test Case
 * Author: Huawei LiteOS Team
 * Create: 2021-06-02
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

#include "it_los_membox.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

VOID *g_boxAddr = NULL;
UINT32 g_blkSize = 0;

VOID MEMBOX_START(VOID)
{
    g_boxAddr = LOS_MemAlloc(OS_SYS_MEM_ADDR, TEST_MBOX_SIZE);
    if ((g_boxAddr == NULL)) {
        PRINT_ERR("alloc membox failed , mem testcase will be failed!!!!\n");
    }
}

VOID MEMBOX_END(VOID)
{
    UINT32 ret;
    ret = LOS_MemFree(OS_SYS_MEM_ADDR, g_boxAddr);
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);
}

VOID MEMBOX_INIT(VOID)
{
    UINT32 ret;

    ret = LOS_MemboxInit(g_boxAddr, TEST_MBOX_SIZE, g_blkSize);
    ICUNIT_ASSERT_EQUAL_VOID(ret, LOS_OK, ret);
}

VOID MEMBOX_FREE(VOID)
{
    VOID *p = NULL;

    p = memset(g_boxAddr, 0, TEST_MBOX_SIZE);
    ICUNIT_ASSERT_EQUAL_VOID(p, g_boxAddr, p);
}

VOID ItSuiteLosMembox(VOID)
{
#ifdef LOSCFG_KERNEL_MEMBOX_STATIC
    ItLosMembox001();
    ItLosMembox002();
    ItLosMembox003();
#endif
    ItLosMembox004();
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */
