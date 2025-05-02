# Programming Example<a name="EN-US_TOPIC_0311018323"></a>

## Example Description<a name="en-us_topic_0175230609_section43216539154246"></a>

There are some examples giving some basic insight into priority-based task scheduling and use cases of APIs, including create, delay, lock, unlock, suspend, resume, and query \(task ID and information by task ID\) a task.

## Example Code<a name="en-us_topic_0175230609_section4467667154434"></a>

Prerequisite: The task module has been configured in the  **menuconfig**  menu.

```c
UINT32 g_taskHiId;
UINT32 g_taskLoId;
#define TSK_PRIOR_HI 4
#define TSK_PRIOR_LO 5

UINT32 Example_TaskHi(VOID)
{
    UINT32 ret;

    printf("Enter TaskHi Handler.\r\n");

    /*Delay TaskHi for 2 ticks. TaskHi is then suspended and the task (g_taskLoId) with the highest priority among the rest will be executed first.*/
    ret = LOS_TaskDelay(2);
    if (ret != LOS_OK) {
        printf("Delay Task Failed.\r\n");
        return LOS_NOK;
    }

    /*After 2 ticks elapse, TaskHi is resumed.*/
    printf("TaskHi LOS_TaskDelay Done.\r\n");

    /*Suspend the task.*/
    ret = LOS_TaskSuspend(g_taskHiId);
    if (ret != LOS_OK) {
        printf("Suspend TaskHi Failed.\r\n");
        return LOS_NOK;
    }
    printf("TaskHi LOS_TaskResume Success.\r\n");

    return ret;
}

/*Task entrypoint function for the TaskLo*/
UINT32 Example_TaskLo(VOID)
{
    UINT32 ret;

    printf("Enter TaskLo Handler.\r\n");

    /*Delay TaskLo for 2 ticks. TaskLo is then suspended and the task (background task) with the highest priority among the rest will be executed first.*/
    ret = LOS_TaskDelay(2);
    if (ret != LOS_OK) {
        printf("Delay TaskLo Failed.\r\n");
        return LOS_NOK;
    }

    printf("TaskHi LOS_TaskSuspend Success.\r\n");

    /*Resume the suspended g_taskHiId task.*/
    ret = LOS_TaskResume(g_taskHiId);
    if (ret != LOS_OK) {
        printf("Resume TaskHi Failed.\r\n");
        return LOS_NOK;
    }

    printf("TaskHi LOS_TaskDelete Success.\r\n");

    return ret;
}

/*Task test entrypoint function. Two tasks with different priorities will be created.*/
UINT32 Example_TskCaseEntry(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S initParam;

    /*Lock task scheduling to prevent higher-priority tasks from being scheduled prior to this task.*/
    LOS_TaskLock();

    printf("LOS_TaskLock() Success!\r\n");

    initParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_TaskHi;
    initParam.usTaskPrio = TSK_PRIOR_HI;
    initParam.pcName = "TaskHi";
    initParam.uwStackSize = LOSCFG_TASK_MIN_STACK_SIZE;
    initParam.uwResved   = LOS_TASK_STATUS_DETACHED;
    /*Create a high-priority task. The task will not be executed immediately after being created, because task scheduling is locked.*/
    ret = LOS_TaskCreate(&g_taskHiId, &initParam);
    if (ret != LOS_OK) {
        LOS_TaskUnlock();

        printf("Example_TaskHi create Failed!\r\n");
        return LOS_NOK;
    }

    printf("Example_TaskHi create Success!\r\n");

    initParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_TaskLo;
    initParam.usTaskPrio = TSK_PRIOR_LO;
    initParam.pcName = "TaskLo";
    initParam.uwStackSize = LOSCFG_TASK_MIN_STACK_SIZE;
    initParam.uwResved   = LOS_TASK_STATUS_DETACHED;

    /*Create a low-priority task. The task will not be executed immediately after being created, because task scheduling is locked.*/
    ret = LOS_TaskCreate(&g_taskLoId, &initParam);
    if (ret != LOS_OK) {
        LOS_TaskUnlock();

        printf("Example_TaskLo create Failed!\r\n");
        return LOS_NOK;
    }

    printf("Example_TaskLo create Success!\r\n");

    /*Unlock task scheduling. The task with the highest priority in the Ready list will be executed.*/
    LOS_TaskUnlock();

    return LOS_OK;
}
```

## Verification<a name="en-us_topic_0175230609_section28642348111041"></a>

The verification result is as follows:

```
LOS_TaskLock() Success!
Example_TaskHi create Success!
Example_TaskLo create Success!
Enter TaskHi Handler.
Enter TaskLo Handler.
TaskHi LOS_TaskDelay Done.
TaskHi LOS_TaskSuspend Success.
TaskHi LOS_TaskResume Success.
TaskHi LOS_TaskDelete Success.
```

## Complete Code<a name="en-us_topic_0175230609_section17630141111041"></a>

[sample\_task.c](resource/sample_task.c)

