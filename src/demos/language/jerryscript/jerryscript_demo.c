/* ----------------------------------------------------------------------------
 * Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
 * Description: Jerryscript Demo Implementation
 * Author: Huawei LiteOS Team
 * Create: 2021-07-15
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

#include "jerryscript_demo.h"
#include "stdio.h"
#include "los_task.h"
#include "jerryscript.h"
#include "jerryscript-ext/handler.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cplusplus */
#endif /* __cplusplus */

#define JERRYSCRIPT_TASK_STACK_SIZE 0x1000
#define JERRYSCRIPT_TASK_PRIORITY   8

STATIC UINT32 g_demoTaskId;

STATIC INT32 JerryTest(VOID)
{
    INT32 retCode = 0;
    jerry_value_t retValue = jerry_create_undefined();
    const jerry_char_t script[] = "print(\"Hello, World! I am JerryScript.\");";

    printf("This test run the following script code: %s\n", script);
    jerry_init(JERRY_INIT_EMPTY);   /* Initialize engine */

    /* Register the print function in the global object. */
    (VOID)jerryx_handler_register_global((const jerry_char_t *)"print", jerryx_handler_print);
    /* Setup Global scope code */
    retValue = jerry_parse(NULL, 0, script, (sizeof(script) - 1), JERRY_PARSE_NO_OPTS); 
    if (!jerry_value_is_error(retValue)) {
        retValue = jerry_run(retValue);     /* Execute the parsed source code in the Global scope */
    }

    if (jerry_value_is_error(retValue)) {
        printf("Script error!");
        retCode = -1;
    }

    jerry_release_value(retValue);
    jerry_cleanup();
    return retCode;
}

STATIC INT32 DemoTaskEntry(VOID)
{
    printf("Jerryscript demo task start to run.\n");
    JerryTest();
    printf("Jerryscript demo task finished.\n");
    return 0;
}

VOID JerryscriptDemoTask(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S taskInitParam;

    ret = (UINT32)memset_s(&taskInitParam, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    if (ret != EOK) {
        return;
    }
    taskInitParam.usTaskPrio = JERRYSCRIPT_TASK_PRIORITY;
    taskInitParam.pcName = "JerryscriptDemoTask";
    taskInitParam.pfnTaskEntry = (TSK_ENTRY_FUNC)DemoTaskEntry;
    taskInitParam.uwStackSize = JERRYSCRIPT_TASK_STACK_SIZE;
    ret = LOS_TaskCreate(&g_demoTaskId, &taskInitParam);
    if (ret != LOS_OK) {
        printf("Create Jerryscript demo task failed.\n");
    }
}

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cplusplus */
#endif /* __cplusplus */

