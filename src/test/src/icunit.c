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

#include "icunit.h"
#include "los_task.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

BOOL g_iCunitInitSuccess = FALSE;

UINT16 g_iCunitErrLineNo;
UINT32 g_iCunitErrCode;

UINT32 g_failResult;
UINT32 g_passResult;

/* g_strModule is name of enum LiteOsTestModule must one-to-one correspondence */
char *g_strModule[] = {
    "TASK", "MEM", "SEM", "MUX", "EVENT", "QUE", "SWTMR", "HWI", \
    "ATO", "TEST_MISC", "ALL" \
};

/* g_strLayer is name of enum LiteOsTestLayer must one-to-one correspondence */
char *g_strLayer[] = {"LOS", "SHELL", "ALL"};

UINT32 g_modelNum = sizeof(g_strModule) / sizeof(g_strModule[0]);

#ifdef LOSCFG_TEST_MODULE_CHECK
UINT32 g_failModelResult[sizeof(g_strModule) / sizeof(g_strModule[0])] = {0};
UINT32 g_passModelResult[sizeof(g_strModule) / sizeof(g_strModule[0])] = {0};
UINT32 g_executModelNum[sizeof(g_strModule) / sizeof(g_strModule[0])] = {0};
ICUNIT_CASE_S g_errorCase[50] = {0}; // 50 is max ErrorCase num
#endif

char *g_strLevel[] = {"LEVEL0", "LEVEL1", "LEVEL2", "LEVEL3", "LEVEL4", "ALL"};
char *g_strTypepe[] = {"FUNCTION", "PRESSURE", "PERFORMANCE", "ALL"};

INT32 g_cunitInitFlag = 0;

#ifdef LOSCFG_KERNEL_SMP
LITE_OS_SEC_BSS SPIN_LOCK_INIT(g_testSuitSpin);
#endif

VOID ICunitSaveErr(UINT32 uwLine, UINT32 uwRetCode)
{
#ifdef LOSCFG_KERNEL_SMP
    UINT32 intSave;
    TESTSUIT_LOCK(intSave);
#endif
    g_iCunitErrCode = ((g_iCunitErrCode == 0) && (g_iCunitErrLineNo == 0)) ? (UINT32)uwRetCode : g_iCunitErrCode;
    g_iCunitErrLineNo = (g_iCunitErrLineNo == 0) ? uwLine : g_iCunitErrLineNo;
#ifdef LOSCFG_KERNEL_SMP
    TESTSUIT_UNLOCK(intSave);
#endif
}

UINT32 ICunitAddCase(ICUNIT_CASE_S *psubCase)
{
    UINT32 ret;
    if (!g_iCunitInitSuccess) {
        return (UINT32)ICUNIT_UNINIT;
    }

    ret = ICunitRunF(psubCase);
    if (ret != ICUNIT_SUCCESS) {
        return -1;
    }
    LOS_TaskResRecycle();

    return (UINT32)ICUNIT_SUCCESS;
}

UINT32 ICunitInit(VOID)
{
#ifdef LOSCFG_KERNEL_SMP
    if (LOS_AtomicCmpXchg32bits(&g_cunitInitFlag, 1, 0)) {
        PRINTK("other core already done ICunitInit\n");
        return (UINT32)ICUNIT_SUCCESS;
    }
#endif
    g_iCunitInitSuccess = TRUE;

    return (UINT32)ICUNIT_SUCCESS;
}

UINT32 ICunitRunF(ICUNIT_CASE_S *psubCase)
{
    UINT32 caseRet;
    UINT32 curTestTaskId;
    g_iCunitErrLineNo = 0;
    g_iCunitErrCode = 0;

    curTestTaskId = LOS_CurTaskIDGet();
    dprintf("# T:%d Enter:%s \n", curTestTaskId, psubCase->pcCaseID);
    caseRet = psubCase->pstCaseFunc();

    psubCase->errLine = g_iCunitErrLineNo;
    psubCase->retCode = (g_iCunitErrLineNo == 0) ? (caseRet) : (g_iCunitErrCode);

#ifdef LOSCFG_TEST_MODULE_CHECK
    g_executModelNum[psubCase->testcaseModule]++;
#endif
    if (psubCase->errLine == 0 && caseRet == 0) {
        g_passResult++;

#ifdef LOSCFG_TEST_MODULE_CHECK
        g_passModelResult[psubCase->testcaseModule]++;
#endif

        dprintf(" T:%d [Passed]-%s-%s-%s-%s-%s\n", curTestTaskId, psubCase->pcCaseID,
                g_strLayer[psubCase->testcaseLayer],
                g_strModule[psubCase->testcaseModule],
                g_strLevel[psubCase->testcaseLevel],
                g_strTypepe[psubCase->testcaseType]);
    } else {
#ifdef LOSCFG_TEST_MODULE_CHECK
        if (g_failResult < 50) { // max failresult is 50.
            g_errorCase[g_failResult] = *psubCase;
        }
        g_failModelResult[psubCase->testcaseModule]++;
#endif

        g_failResult++;
        dprintf(" T:%d [Failed]-%s-%s-%s-%s-%s-[Errline: %d RetCode:0x%04X%04X]\n", curTestTaskId, psubCase->pcCaseID,
                g_strLayer[psubCase->testcaseLayer],
                g_strModule[psubCase->testcaseModule],
                g_strLevel[psubCase->testcaseLevel],
                g_strTypepe[psubCase->testcaseType],
                psubCase->errLine, (UINT16)((psubCase->retCode) >> 16), (UINT16)(psubCase->retCode)); // Shift Right 16.
    }

    return (UINT32)ICUNIT_SUCCESS;
}

UINT32 ICunitPrintReport(VOID)
{
#if defined(LOSCFG_TEST_MODULE_CHECK) && defined(LOSCFG_TEST)
    for (int i = 0; i < g_modelNum - 1; i++) {
        if (g_executModelNum[i] != 0) {
            dprintf("\nExecuted Model: %s, Executed Model_Num: %d ,failed_count: %d , success_count :%d",
                g_strModule[i], g_executModelNum[i], g_failModelResult[i], g_passModelResult[i]);
        }
        for (int j = 0; j < g_failResult && j < 50; j++) {  // max failresult is 50.
            if (i == g_errorCase[j].testcaseModule) {
                LOS_Msleep(0xc8);
                dprintf(" \n [Failed]-%s-%s-%s-%s-%s-[Errline: %d RetCode:0x%04X%04X]", g_errorCase[j].pcCaseID,
                    g_strLayer[g_errorCase[j].testcaseLayer], g_strModule[g_errorCase[j].testcaseModule],
                    g_strLevel[g_errorCase[j].testcaseLevel], g_strTypepe[g_errorCase[j].testcaseType],
                    g_errorCase[j].errLine, (UINT16)((g_errorCase[j].retCode) >> 0x10),
                    (UINT16)(g_errorCase[j].retCode));
            }
        }
    }
    dprintf("\nNot Executed Model: ");
    for (int i = 0; i < g_modelNum - 1; i++) {
        if (g_executModelNum[i] == 0) {
            LOS_Msleep(40); // sleep 40 ms.
            dprintf("%s    ", g_strModule[i]);
        }
    }
#endif
    dprintf("\n\nfailed count: %d, success count: %d\n", g_failResult, g_passResult);
    return LOS_OK;
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

