# Programming Example<a name="EN-US_TOPIC_0311018418"></a>

## Example Description<a name="en-us_topic_0175230248_section1288182415417"></a>

In the programming example, the following functions are implemented:

1.  The Example\_TaskEntry task is executed to create a semaphore. Task scheduling is locked. Two tasks Example\_SemTask1 and Example\_SemTask2 are created, where Example\_SemTask2 takes a higher priority than Example\_SemTask1. The two tasks request the same semaphore. After task scheduling is unlocked, the two tasks are blocked, the test task Example\_TaskEntry releases the semaphore.
2.  Example\_SemTask2 is granted the semaphore, scheduled, and sent to sleep mode for 20 ticks. While Example\_SemTask2 is delayed, Example\_SemTask1 is woken up.
3.  Example\_SemTask1 requests the semaphore and is willing to wait the semaphore for 10 ticks to become free. At the time when Example\_SemTask1 requests the semaphore, the semaphore is held by Example\_SemTask2 and consequently Example\_SemTask1 is suspended. After the 10-tick wait period elapses, the semaphore is still out of the reach of Example\_SemTask1, and Example\_SemTask1 is woken up, attempting to wait permanently for the semaphore. The wait for semaphore switches Example\_SemTask1 to suspended state.
4.  After 20 ticks, Example\_SemTask2 is woken up and releases the semaphore. Example\_SemTask1 is scheduled, granted the semaphore, and finally releases it.
5.  40 ticks after Example\_SemTask1 is finished, Example\_TaskEntry is woken up, deletes the semaphore and then the two tasks.

## Example Code<a name="en-us_topic_0175230248_section47263443154123"></a>

Prerequisite: The semaphore module has been configured in the  **menuconfig**  menu.

The code is as follows:

```c
#include "los_sem.h"
#include "securec.h"

 /*Task ID*/ 
static UINT32 g_testTaskId01;
static UINT32 g_testTaskId02;
 /*Task priority*/ 
#define TASK_PRIO_TEST  5
/*Semaphore structure ID*/ 
static UINT32 g_semId;

VOID Example_SemTask1(VOID)
{
    UINT32 ret;

    printf("Example_SemTask1 try get sem g_semId ,timeout 10 ticks.\n");
    /*The task requests the semaphore in scheduled blocking mode, with the wait period being 10 ticks*/ 
    ret = LOS_SemPend(g_semId, 10);

    /*The task is granted the semaphore.*/ 
    if (ret == LOS_OK) {
         LOS_SemPost(g_semId);
         return;
    }
    /*The task does not get the semaphore within the scheduled time.*/ 
    if (ret == LOS_ERRNO_SEM_TIMEOUT) {
        printf("Example_SemTask1 timeout and try get sem g_semId wait forever.\n");
        /*The task requests the semaphore in permanent blocking mode.*/ 
        ret = LOS_SemPend(g_semId, LOS_WAIT_FOREVER);
        printf("Example_SemTask1 wait_forever and get sem g_semId .\n");
        if (ret == LOS_OK) {
            LOS_SemPost(g_semId);
            return;
        }
    }
}

VOID Example_SemTask2(VOID)
{
    UINT32 ret;
    printf("Example_SemTask2 try get sem g_semId wait forever.\n");
    /*The task requests the semaphore in permanent blocking mode.*/ 
    ret = LOS_SemPend(g_semId, LOS_WAIT_FOREVER);

    if (ret == LOS_OK) {
        printf("Example_SemTask2 get sem g_semId and then delay 20ticks .\n");
    }

  /*Enable the task to enter sleep mode for 20 ticks.*/ 
    LOS_TaskDelay(20);

    printf("Example_SemTask2 post sem g_semId .\n");
    /*The task releases the semaphore.*/ 
    LOS_SemPost(g_semId);
    return;
}

UINT32 ExampleTaskEntry(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S task1;
    TSK_INIT_PARAM_S task2;

   /*Create a semaphore.*/ 
    LOS_SemCreate(0,&g_semId);

  /*Lock task scheduling.*/ 
    LOS_TaskLock();

    /*Create task1.*/ 
    (VOID)memset_s(&task1, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    task1.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_SemTask1;
    task1.pcName       = "TestTsk1";
    task1.uwStackSize  = OS_TSK_DEFAULT_STACK_SIZE;
    task1.usTaskPrio   = TASK_PRIO_TEST;
    ret = LOS_TaskCreate(&g_testTaskId01, &task1);
    if (ret != LOS_OK) {
        printf("task1 create failed .\n");
        return LOS_NOK;
    }

    /*Create task2.*/ 
    (VOID)memset_s(&task2, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    task2.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_SemTask2;
    task2.pcName       = "TestTsk2";
    task2.uwStackSize  = OS_TSK_DEFAULT_STACK_SIZE;
    task2.usTaskPrio   = (TASK_PRIO_TEST - 1);
    ret = LOS_TaskCreate(&g_testTaskId02, &task2);
    if (ret != LOS_OK) {
        printf("task2 create failed .\n");
        return LOS_NOK;
    }

    /*Unlock task scheduling.*/ 
    LOS_TaskUnlock();

    ret = LOS_SemPost(g_semId);

 /*Enable the task to enter sleep mode for 40 ticks.*/ 
    LOS_TaskDelay(40);

 /*Delete the semaphore.*/ 
    LOS_SemDelete(g_semId);

    /*Delete task1.*/ 
    ret = LOS_TaskDelete(g_testTaskId01);
    if (ret != LOS_OK) {
        printf("task1 delete failed .\n");
        return LOS_NOK;
    }
    /*Delete task2.*/ 
    ret = LOS_TaskDelete(g_testTaskId02);
    if (ret != LOS_OK) {
        printf("task2 delete failed .\n");
        return LOS_NOK;
    }

    return LOS_OK;
}
```

## Verification<a name="en-us_topic_0175230248_section60141895113933"></a>

The verification result is as follows:

```
Example_SemTask2 try get sem g_semId wait forever.
Example_SemTask1 try get sem g_semId ,timeout 10 ticks.
Example_SemTask2 get sem g_semId and then delay 20ticks .
Example_SemTask1 timeout and try get sem g_semId wait forever.
Example_SemTask2 post sem g_semId .
Example_SemTask1 wait_forever and get sem g_semId .
```

## Complete Code<a name="en-us_topic_0175230248_section60896759114224"></a>

[sample\_sem.c](resource/sample_sem.c)

