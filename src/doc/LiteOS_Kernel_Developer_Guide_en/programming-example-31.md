# Programming Example<a name="EN-US_TOPIC_0311018423"></a>

## Example Description<a name="en-us_topic_0175230612_section1814955816725"></a>

In the programming example, the following activities will happen:

1.  The Example\_TaskEntry task is executed to create a mutex. Task scheduling is locked. Two tasks Example\_MutexTask1 and Example\_MutexTask2 are created, where Example\_MutexTask2 takes a higher priority than Example\_MutexTask1. Then, task scheduling is unlocked, and the Example\_TaskEntry task stays in sleep mode for 300 ticks.
2.  Example\_MutexTask2 is scheduled, granted a mutex, and then sent to sleep mode for 100 ticks. While Example\_MutexTask2 is suspended, Example\_MutexTask1 is woken up.
3.  Example\_MutexTask1 requests a mutex and is willing to wait the mutex for 10 ticks to become free. At the time when Example\_MutexTask1 requests the mutex, the mutex is held by Example\_MutexTask2 and consequently Example\_MutexTask1 is suspended. After the 10-tick wait period elapses, the mutex is still out of the reach of Example\_MutexTask1, and Example\_MutexTask1 is woken up, attempting to wait permanently for the mutex. The wait for the mutex switches Example\_MutexTask1 to suspended state.
4.  After 100 ticks, Example\_MutexTask2 is woken up and releases the mutex, and then Example\_MutexTask1 is woken up. Example\_MutexTask1 is scheduled, granted the mutex, and finally releases it.
5.  After the 300 ticks in sleep mode, Example\_TaskEntry task is scheduled and executed. The mutex is deleted, and two tasks are deleted.

## Example Code<a name="en-us_topic_0175230612_section35197262142848"></a>

Prerequisite: The mutex module has been configured using the  **make menuconfig**  command.

The code is as follows:

```c
/*Mutex handler ID*/ 
UINT32 g_testMux;
/*Task ID*/
UINT32 g_testTaskId01;
UINT32 g_testTaskId02;

VOID Example_MutexTask1(VOID)
{
    UINT32 ret;

    printf("task1 try to get  mutex, wait 10 ticks.\n");
    /* Request a mutex.*/
    ret = LOS_MuxPend(g_testMux, 10);

    if (ret == LOS_OK) {
        printf("task1 get mutex g_testMux.\n");
         /*Release the mutex.*/ 
        LOS_MuxPost(g_testMux);
        return;
    } else if (ret == LOS_ERRNO_MUX_TIMEOUT ) {
            printf("task1 timeout and try to get mutex, wait forever.\n");
            /* Request a mutex.*/
            ret = LOS_MuxPend(g_testMux, LOS_WAIT_FOREVER);
            if (ret == LOS_OK) {
                printf("task1 wait forever, get mutex g_testMux.\n");
                /*Release the mutex.*/ 
                LOS_MuxPost(g_testMux);
                return;
            }
    }
    return;
}

VOID Example_MutexTask2(VOID)
{
    printf("task2 try to get  mutex, wait forever.\n");
    /* Request a mutex.*/
    (VOID)LOS_MuxPend(g_testMux, LOS_WAIT_FOREVER);

    printf("task2 get mutex g_testMux and suspend 100 ticks.\n");

     /*Enable the task to enter sleep mode for 100 ticks.*/ 
    LOS_TaskDelay(100);

    printf("task2 resumed and post the g_testMux\n");
    /*Release the mutex.*/ 
    LOS_MuxPost(g_testMux);
    return;
}

UINT32 Example_TaskEntry(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S task1;
    TSK_INIT_PARAM_S task2;

    /*Create a mutex.*/ 
    LOS_MuxCreate(&g_testMux);

     /*Lock task scheduling.*/ 
    LOS_TaskLock();

    /*Create task1.*/ 
    memset(&task1, 0, sizeof(TSK_INIT_PARAM_S));
    task1.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_MutexTask1;
    task1.pcName       = "MutexTsk1";
    task1.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    task1.usTaskPrio   = 5;
    ret = LOS_TaskCreate(&g_testTaskId01, &task1);
    if (ret != LOS_OK) {
        printf("task1 create failed.\n");
        return LOS_NOK;
    }

    /*Create task2.*/ 
    memset(&task2, 0, sizeof(TSK_INIT_PARAM_S));
    task2.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_MutexTask2;
    task2.pcName       = "MutexTsk2";
    task2.uwStackSize  = LOSCFG_BASE_CORE_TSK_DEFAULT_STACK_SIZE;
    task2.usTaskPrio   = 4;
    ret = LOS_TaskCreate(&g_testTaskId02, &task2);
    if (ret != LOS_OK) {
        printf("task2 create failed.\n");
        return LOS_NOK;
    }

    /*Unlock task scheduling.*/ 
    LOS_TaskUnlock();
    /*Enable the task to enter sleep mode for 300 ticks.*/ 
    LOS_TaskDelay(300);

     /*Delete the mutex.*/ 
    LOS_MuxDelete(g_testMux);

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

## Verification<a name="en-us_topic_0175230612_section5949313217435"></a>

The verification result is as follows:

```
task2 try to get  mutex, wait forever.
task2 get mutex g_testMux and suspend 100 ticks.
task1 try to get  mutex, wait 10 ticks.
task1 timeout and try to get mutex, wait forever.
task2 resumed and post the g_testMux
task1 wait forever,get mutex g_testMux.
```

## Complete Code<a name="en-us_topic_0175230612_section2929126917435"></a>

[sample\_mutex.c](resource/sample_mutex.c)

