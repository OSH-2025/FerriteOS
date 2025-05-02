# Programming Example \(SMP\)<a name="EN-US_TOPIC_0311018324"></a>

## Example Description<a name="en-us_topic_0175921101_section43216539154246"></a>

There are some examples giving some basic insight into priority-based task scheduling and use cases of APIs, including create, delay, lock, unlock, suspend, resume, and query \(task ID and information by task ID\) a task.

1.  Two tasks will be created: TaskHi and TaskLo.
2.  TaskHi has a high priority and is bound to the CPU of the current test task.
3.  TaskLo has a low priority and its CPU affinity is not configured.

>![](public_sys-resources/icon-notice.gif) **NOTICE:** 
>
>Because affinity is not configured for TaskLo, the operation that calls the LOS\_TaskLock API will fail, and the "Example\_TaskLo create Success!" and "Enter TaskLo Handler." statements are not printed in a strict sequence.

## Example Code<a name="en-us_topic_0175921101_section567143124115"></a>

Prerequisite: The task module has been configured and SMP mode has been enabled in the  **menuconfig**  menu.

```c
UINT32 g_taskLoId;
UINT32 g_taskHiId;
#define TSK_PRIOR_HI 4
#define TSK_PRIOR_LO 5

UINT32 Example_TaskHi(VOID)
{
    UINT32 ret;

    printf("[cpu%d] Enter TaskHi Handler.\r\n", ArchCurrCpuid());

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

    printf("[cpu%d] Enter TaskLo Handler.\r\n", ArchCurrCpuid());

    /*Delay TaskLo for 2 ticks. TaskLo is then suspended and the task (background task) with the highest priority among the rest will be executed first.*/
    ret = LOS_TaskDelay(2);
    if (ret != LOS_OK) {
        printf("Delay TaskLo Failed.\r\n");
        return LOS_NOK;
    }

    printf("TaskHi LOS_TaskDelete Success.\r\n");
    return ret;
}

/*Task test entrypoint function. Two tasks with different priorities will be created.*/
UINT32 Example_TskCaseEntry(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S initParam = {0};

    /*Lock task scheduling.*/
    LOS_TaskLock();

    printf("LOS_TaskLock() Success on cpu%d!\r\n", ArchCurrCpuid());

    initParam.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_TaskHi;
    initParam.usTaskPrio = TSK_PRIOR_HI;
    initParam.pcName = "TaskHi";
    initParam.uwStackSize = LOSCFG_TASK_MIN_STACK_SIZE;
    initParam.uwResved   = LOS_TASK_STATUS_DETACHED;
#ifdef LOSCFG_KERNEL_SMP
    /*Bind high-priority tasks to CPU1 for execution.*/
    initParam.usCpuAffiMask = CPUID_TO_AFFI_MASK(ArchCurrCpuid());
#endif
    /*Create a high-priority task. The task will not be executed immediately after being created, because the scheduler of CPU1 is locked.*/
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
#ifdef LOSCFG_KERNEL_SMP
    /*Do not set CPU affinity for low-priority tasks.*/
    initParam.usCpuAffiMask = 0;
#endif
    /*Create a low-priority task. The task will be executed immediately on other CPUs after being created, because the task is not bound to the current CPU though task scheduling is locked.*/
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

## Verification<a name="en-us_topic_0175921101_section28642348111041"></a>

The verification result is as follows:

```
LOS_TaskLock() success on cpu1!
Example_TaskHi create Success!
Example_TaskLo create Success!
[cpu2] Enter TaskLo Handler.
[cpu1] Enter TaskHi Handler.
TaskHi LOS_TaskDelete Success.
TaskHi LOS_TaskDelay Done.
```

## Complete Code<a name="en-us_topic_0175921101_section9191152164712"></a>

[sample\_task\_smp.c](resource/sample_task_smp.c)

