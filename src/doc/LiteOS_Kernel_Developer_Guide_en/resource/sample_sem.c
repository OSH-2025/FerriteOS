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