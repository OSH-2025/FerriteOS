# Programming Example<a name="EN-US_TOPIC_0312244423"></a>

## Example Description<a name="en-us_topic_0175230409_section3463876395722"></a>

The following functions are implemented:

1.  Create a task for testing the CPUP.
2.  Obtain the CPUP of all tasks or interrupts in the latest 1 second.
3.  Obtain the total CPU usage of the system \(except idle tasks\) in the latest 10 seconds.
4.  Obtain the CPUP of a CPUP test task.

## Example Code<a name="en-us_topic_0175230409_section2811476595722"></a>

Prerequisite: The CPU usage module has been configured by running the  **make menuconfig**  command.

The code is as follows:

```c
#include <unistd.h >
#include "los_task.h"
#include "los_cpup.h"

#define MAXTASKNUM  32

UINT32 cpupUse;
UINT32 g_cpuTestTaskId;

VOID Example_CPUP(VOID)
{
    printf("entry cpup test example\n");
    while(1) {
        usleep(100);
    }
}

UINT32 Example_CPUP_Test(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S cpupTestTask;
    CPUP_INFO_S cpupInfo;

    /* Create a task for testing the CPUP. */
    memset(&cpupTestTask, 0, sizeof(TSK_INIT_PARAM_S));
    cpupTestTask.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_CPUP;
    cpupTestTask.pcName       = "TestCpupTsk";                           /* Test task name */
    cpupTestTask.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    cpupTestTask.usTaskPrio   = 5;
    cpupTestTask.uwResved   = LOS_TASK_STATUS_DETACHED;

    ret = LOS_TaskCreate(&g_cpuTestTaskId, &cpupTestTask);
    if(ret != LOS_OK) {
        printf("cpupTestTask create failed.\n");
        return LOS_NOK;
    }
    usleep(100);

    /* Number of running tasks or interrupts in the system */
    UINT16 maxNum = MAXTASKNUM; 

    /* Obtain the CPU usage of all tasks or interrupts in the latest 1 second */
    cpupUse = LOS_AllCpuUsage(maxNum, &cpupInfo, CPUP_LAST_ONE_SECONDS, 0);
    printf("the system cpu usage in last 1s: %d\n", cpupUse);

    /* Obtain the total CPU usage of the system (except idle tasks) in the latest 10 seconds. */
    cpupUse = LOS_HistorySysCpuUsage(CPUP_LAST_TEN_SECONDS);
    printf("the history system cpu usage in last 10s: %d\n", cpupUse);

    /* Obtain the CPU usage of the specified task in the latest 1 second. The specified task in this test case is the CPUP test task created in the previous step. */
    cpupUse = LOS_HistoryTaskCpuUsage(g_cpuTestTaskId, CPUP_LAST_ONE_SECONDS);
    printf("cpu usage of the cpupTestTask in last 1s:\n TaskID: %d\n usage: %d\n", g_cpuTestTaskId, cpupUse);

    return LOS_OK;
}
```

## Verification<a name="en-us_topic_0175230409_section556751395722"></a>

The verification result is as follows:

```
the system cpu usage in last 1s: 15
the history system cpu usage in last 10s: 3
cpu usage of the cpupTestTask in last 1s:
TaskID: 10
usage: 0
```

## Complete Code<a name="en-us_topic_0175230409_section2123109795722"></a>

[sample\_cpup.c](resource/sample_cpup.c)

