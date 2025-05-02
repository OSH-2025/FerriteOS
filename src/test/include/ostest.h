/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: iCunit Test Frame
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

#ifndef _OSTEST_H
#define _OSTEST_H

#include "icunit.h"
#include "test_common.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define HWI_NUM_INT17    17
#define HWI_NUM_INT18    18
#define HWI_NUM_INT19    19
#define HWI_NUM_INT21    21
#define HWI_NUM_INT22    22

#define TESTCASE_SEQUENCE_MAX_LEN 16
#define TESTCASE_NUM_MAX_LEN 16
#define TESTCASE_LAYER_MAX_LEN 32
#define TESTCASE_MODULE_MAX_LEN 32
#define TESTCASE_LEVEL_MAX_LEN 16
#define TESTCASE_TYPE_MAX_LEN 16
#define TESTCASE_ID_MAX_LEN 128

#if defined(LOSCFG_PLATFORM_STM32F769IDISCOVERY) \
	|| defined(LOSCFG_PLATFORM_STM32F429IGTX) \
    || defined(LOSCFG_PLATFORM_STM32L431_BearPi) \
    || defined(LOSCFG_PLATFORM_STM32F407_ATK_EXPLORER) \
    || defined(LOSCFG_PLATFORM_STM32F103_FIRE_ARBITRARY) \
    || defined(LOSCFG_PLATFORM_STM32F072_Nucleo)
#define HWI_NUM_TEST0    HWI_NUM_INT17
#define HWI_NUM_TEST     HWI_NUM_INT18
#define HWI_NUM_TEST1    HWI_NUM_INT19
#define HWI_NUM_TEST2    HWI_NUM_INT21
#define HWI_NUM_TEST3    HWI_NUM_INT22
#endif

#ifdef LOSCFG_KERNEL_SMP
#define HWI_NUM_TEST_IPI     HWI_NUM_INT11
#endif

#define TEST_TASKDELAY_2TICK     2
#define TEST_TASKDELAY_10TICK    10

extern VOID ItSuiteLosQueue(VOID);
extern VOID ItSuiteLosAtomic(VOID);
extern VOID ItSuiteLosSwtmr(VOID);
extern VOID ItSuiteLosTask(VOID);
extern VOID ItSuiteLosHwi(VOID);
extern VOID ItSuiteLosEvent(VOID);
extern VOID ItSuiteLosMembox(VOID);
extern VOID ItSuiteLosMux(VOID);
extern VOID ItSuiteLosSem(VOID);
extern VOID ItSuiteLosMem(VOID);
extern VOID ItSuiteLosMisc(VOID);

typedef struct TestrunParam {
    CHAR    testcase_sequence[TESTCASE_SEQUENCE_MAX_LEN];
    CHAR    testcase_num[TESTCASE_NUM_MAX_LEN];
    CHAR    testcaseLayer[TESTCASE_LAYER_MAX_LEN];
    CHAR    testcaseModule[TESTCASE_MODULE_MAX_LEN];
    CHAR    testcaseLevel[TESTCASE_LEVEL_MAX_LEN];
    CHAR    testcaseType[TESTCASE_TYPE_MAX_LEN];
    CHAR    testcase_id[TESTCASE_ID_MAX_LEN];
} TEST_RUN_PARAM;

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _OSTEST_H */
