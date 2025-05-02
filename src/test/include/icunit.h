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

#ifndef _UNI_ICUNIT_H
#define _UNI_ICUNIT_H

#include "los_typedef.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define ICUNIT_OK 1

typedef UINT32 (*CASE_FUNCTION)(VOID);

typedef struct {
    const CHAR       *pcCaseID;
    CASE_FUNCTION    pstCaseFunc;
    UINT16           testcaseLayer;
    UINT16           testcaseModule;
    UINT16           testcaseLevel;
    UINT16           testcaseType;
    UINT32           retCode;
    UINT16           errLine;
} ICUNIT_CASE_S;

typedef enum {
    TEST_TASK = 0,
    TEST_MEM,
    TEST_SEM,
    TEST_MUX,
    TEST_EVENT,
    TEST_QUE,
    TEST_SWTMR,
    TEST_HWI,
    TEST_ATO,
    TEST_MISC,
    TEST_MODULE_ALL,
} LiteOsTestModule;

typedef enum {
    TEST_LOS = 0,
    TEST_SHELL,
    TEST_LAYER_ALL,
} LiteOsTestLayer;

typedef enum {
    TEST_LEVEL0 = 0,
    TEST_LEVEL1,
    TEST_LEVEL2,
    TEST_LEVEL3,
    TEST_LEVEL4,
    TEST_LEVEL_ALL,
} LiteOsTestLevel;

typedef enum {
    TEST_FUNCTION = 0,
    TEST_PRESSURE,
    TEST_PERFORMANCE,
    TEST_TYPE_ALL,
} LiteOsTestType;

extern UINT16 g_iCunitErrLineNo;  /* Error Line Number */
extern UINT32 g_iCunitErrCode;    /* Error Code */

extern VOID ICunitSaveErr(UINT32 uwLine, UINT32 uwRetCode);

/* The framework is not initialized. */
#define ICUNIT_UNINIT               0x0EF00000
/* Failed to add the case. */
#define ICUNIT_CASE_FULL            0x0EF00003
/* Operation succeeded. */
#define ICUNIT_SUCCESS              0x00000000

/* If param not equal value, Do not return an error immediately */
#define ICUNIT_TRACK_EQUAL(param, value, retcode) \
    do { \
        if ((param) != (value)) { \
            ICunitSaveErr(__LINE__, (UINT32)(retcode)); \
        } \
    } while (0)

/* Assert param equal value. Otherwise, return VOID. */
#define ICUNIT_ASSERT_EQUAL_VOID(param, value, retcode) \
    do { \
        if ((param) != (value)) { \
            ICunitSaveErr(__LINE__, (UINT32)(retcode)); \
            return; \
        } \
    } while (0)

/* Assert param not equal value. Otherwise, return VOID. */
#define ICUNIT_ASSERT_NOT_EQUAL_VOID(param, value, retcode) \
    do { \
        if ((param) == (value)) { \
            ICunitSaveErr(__LINE__, (UINT32)(retcode)); \
            return; \
        } \
    } while (0)

/* Assert param equal value. Otherwise, return 1. */
#define ICUNIT_ASSERT_EQUAL(param, value, retcode) \
    do { \
        if ((param) != (value)) { \
            ICunitSaveErr(__LINE__, (UINT32)(retcode)); \
            return ICUNIT_OK; \
        } \
    } while (0)

/* Assert param not equal value. Otherwis, return 1. */
#define ICUNIT_ASSERT_NOT_EQUAL(param, value, retcode) \
    do { \
        if ((param) == (value)) { \
            ICunitSaveErr(__LINE__, (UINT32)(retcode)); \
            return ICUNIT_OK; \
        } \
    } while (0)

/* Assert param is between value1 and value2. Otherwise,return 1. */
#define ICUNIT_ASSERT_WITHIN_EQUAL(param, value1, value2, retcode) \
    do { \
        if ((param) < (value1) || (param) > (value2)) { \
            ICunitSaveErr(__LINE__, (UINT32)(retcode)); \
            return ICUNIT_OK; \
        } \
    } while (0)

/* Assert param and value are the same. Otherwise,go to label. */
#define ICUNIT_GOTO_EQUAL(param, value, retcode, label) \
    do { \
        if ((param) != (value)) { \
            ICunitSaveErr(__LINE__, (UINT32)(retcode)); \
            goto label; \
        } \
    } while (0)

/* Assert param not equal value. Otherwise,go to label. */
#define ICUNIT_GOTO_NOT_EQUAL(param, value, retcode, label) \
    do { \
        if ((param) == (value)) { \
            ICunitSaveErr(__LINE__, (UINT32)(retcode)); \
            goto label; \
        } \
    } while (0)

#ifdef LOSCFG_KERNEL_SMP
extern SPIN_LOCK_S g_testSuitSpin;
#define TESTSUIT_LOCK(state)                       LOS_SpinLockSave(&g_testSuitSpin, &(state))
#define TESTSUIT_UNLOCK(state)                     LOS_SpinUnlockRestore(&g_testSuitSpin, state)
#endif

/* Add a test case. */
extern UINT32 ICunitAddCase(ICUNIT_CASE_S *psubCase);

/* iCunit Framework Initialization */
extern UINT32 ICunitInit(VOID);
extern UINT32 ICunitRunF(ICUNIT_CASE_S *psubCase);

/* Print the test result. */
extern UINT32 ICunitPrintReport(VOID);

#define ICUNIT_CASE_S_INIT(caseInfo, name, casefunc, caseLayer, caseModule, caseLevel, caseType) \
    do { \
            (&(caseInfo))->pcCaseID = (name); \
            (&(caseInfo))->pstCaseFunc = (CASE_FUNCTION)(casefunc); \
            (&(caseInfo))->testcaseLayer = (caseLayer); \
            (&(caseInfo))->testcaseModule = (caseModule); \
            (&(caseInfo))->testcaseLevel = (caseLevel); \
            (&(caseInfo))->testcaseType = (caseType); \
    } while (0)

#define TEST_ADD_CASE(name, casefunc, caseLayer, caseModule, caseLevel, caseType) \
    do { \
            ICUNIT_CASE_S caseInfo; \
            ICUNIT_CASE_S_INIT(caseInfo, name, casefunc, caseLayer, caseModule, caseLevel, caseType); \
            UINT32 uwRet = ICunitAddCase(&caseInfo); \
            ICUNIT_ASSERT_EQUAL_VOID(uwRet, ICUNIT_SUCCESS, uwRet); \
    } while (0)

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

#endif /* _UNI_ICUNIT_H */
