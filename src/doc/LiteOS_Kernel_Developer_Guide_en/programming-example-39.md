# Programming Example<a name="EN-US_TOPIC_0311018433"></a>

## Example Description<a name="en-us_topic_0175921106_section1814955816725"></a>

In the programming example, the following activities will happen:

1.  The Example\_TaskEntry task is executed to initialize a spinlock. Two tasks Example\_SpinTask1 and Example\_SpinTask2 are created, which run on two cores respectively.
2.  Both Example\_SpinTask1 and Example\_SpinTask2 request the spinlock, and a certain delay is performed after the spinlock is held, to simulate an actual operation.
3.  After 300 ticks, Example\_TaskEntry is scheduled and executed to delete Example\_SpinTask1 and Example\_SpinTask2.

>![](public_sys-resources/icon-notice.gif) **NOTICE:** 
>
>The running sequence of multiple cores is not fixed. Therefore, the task execution sequence may be different.

## Example Code<a name="en-us_topic_0175921106_section35197262142848"></a>

Prerequisite: The configuration item  **LOSCFG\_KERNEL\_SMP**  has been enabled in the  **menuconfig**  menu and the number of cores has been set.

The code is as follows:

```
#include "los_spinlock.h"
#include "los_task.h"

/*Spinlock handler ID*/
SPIN_LOCK_S g_testSpinlock;
/*Task ID*/
UINT32 g_testTaskId01;
UINT32 g_testTaskId02;

VOID Example_SpinTask1(VOID)
{
    UINT32 i;
    UINTPTR intSave;

    /* Request a spinlock. */
    dprintf("task1 try to get spinlock\n");
    LOS_SpinLockSave(&g_testSpinlock, &intSave);
    dprintf("task1 got spinlock\n");
    for(i = 0; i < 5000; i++) {
        asm volatile("nop");
    }

    /*Release the spinlock.*/
    dprintf("task1 release spinlock\n");
    LOS_SpinUnlockRestore(&g_testSpinlock, intSave);

    return;
}

VOID Example_SpinTask2(VOID)
{
    UINT32 i;
    UINTPTR intSave;

    /* Request a spinlock. */
    dprintf("task2 try to get spinlock\n");
    LOS_SpinLockSave(&g_testSpinlock, &intSave);
    dprintf("task2 got spinlock\n");
    for(i = 0; i < 5000; i++) {
        asm volatile("nop");
    }

    /*Release the spinlock.*/
    dprintf("task2 release spinlock\n");
    LOS_SpinUnlockRestore(&g_testSpinlock, intSave);

    return;
}

UINT32 Example_TaskEntry(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S stTask1;
    TSK_INIT_PARAM_S stTask2;

    /* Initialize the spinlock. */
    LOS_SpinInit(&g_testSpinlock);

    /*Create task1.*/
    memset(&stTask1, 0, sizeof(TSK_INIT_PARAM_S));
    stTask1.pfnTaskEntry  = (TSK_ENTRY_FUNC)Example_SpinTask1;
    stTask1.pcName        = "SpinTsk1";
    stTask1.uwStackSize   = LOSCFG_TASK_MIN_STACK_SIZE;
    stTask1.usTaskPrio    = 5;
#ifdef LOSCFG_KERNEL_SMP
    /*Bind tasks to CPU0 for running.*/
    stTask1.usCpuAffiMask = CPUID_TO_AFFI_MASK(0);
#endif
    ret = LOS_TaskCreate(&g_testTaskId01, &stTask1);
    if(ret != LOS_OK) {
        dprintf("task1 create failed .\n");
        return LOS_NOK;
    }

    /*Create task2.*/
    memset(&stTask2, 0, sizeof(TSK_INIT_PARAM_S));
    stTask2.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_SpinTask2;
    stTask2.pcName       = "SpinTsk2";
    stTask2.uwStackSize  = LOSCFG_TASK_MIN_STACK_SIZE;
    stTask2.usTaskPrio   = 5;
#ifdef LOSCFG_KERNEL_SMP
    /*Bind tasks to CPU1 for running.*/
    stTask1.usCpuAffiMask = CPUID_TO_AFFI_MASK(1);
#endif
    ret = LOS_TaskCreate(&g_testTaskId02, &stTask2);
    if(ret != LOS_OK) {
        dprintf("task2 create failed .\n");
        return LOS_NOK;
    }

    /*Enable the task to enter sleep mode for 300 ticks.*/
    LOS_TaskDelay(300);

    /*Delete task1.*/
    ret = LOS_TaskDelete(g_testTaskId01);
    if(ret != LOS_OK) {
        dprintf("task1 delete failed .\n");
        return LOS_NOK;
    }
    /*Delete task2.*/
    ret = LOS_TaskDelete(g_testTaskId02);
    if(ret != LOS_OK) {
        dprintf("task2 delete failed .\n");
        return LOS_NOK;
    }

    return LOS_OK;
}
```

## Verification<a name="en-us_topic_0175921106_section5949313217435"></a>

The verification result is as follows:

```
task2 try to get spinlock
task2 got spinlock
task1 try to get spinlock
task2 release spinlock
task1 got spinlock
task1 release spinlock
```

## Complete Code<a name="en-us_topic_0175921106_section2929126917435"></a>

[sample\_spinlock.c](resource/sample_spinlock.c)

