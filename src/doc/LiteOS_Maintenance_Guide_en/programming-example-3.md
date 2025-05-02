# Programming Example<a name="EN-US_TOPIC_0312244428"></a>

## Example Description<a name="en-us_topic_0278792249_section1259791613416"></a>

The following functions are implemented:

1.  Create a task for the trace test.
2.  Set the event mask.
3.  Start trace.
4.  Stop trace.
5.  Output trace data in a required format.

## Programming Example<a name="en-us_topic_0278792249_section183663491786"></a>

Prerequisite: The trace module has been configured in the  **menuconfig**  menu.

The code is as follows:

```c
#include "los_trace.h"

UINT32 g_traceTestTaskId;
VOID Example_Trace(VOID)
{
    UINT32 ret;

    LOS_TaskDelay(10);

    /* Start trace */
    ret = LOS_TraceStart();
    if (ret != LOS_OK) {
        dprintf("trace start error\n");
        return;
    }

    /* Event that triggers task switching*/
    LOS_TaskDelay(1);

    LOS_TaskDelay(1);

    LOS_TaskDelay(1);

    /* Stop trace */
    LOS_TraceStop();

    LOS_TraceRecordDump(FALSE);
}

UINT32 Example_Trace_test(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S traceTestTask;

    /* Create a task for testing trace. */
    memset(&traceTestTask, 0, sizeof(TSK_INIT_PARAM_S));
    traceTestTask.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_Trace;
    traceTestTask.pcName       = "TestTraceTsk";    /* Test task name */
    traceTestTask.uwStackSize  = 0x800;
    traceTestTask.usTaskPrio   = 5;
    traceTestTask.uwResved   = LOS_TASK_STATUS_DETACHED;
    ret = LOS_TaskCreate(&g_traceTestTaskId, &traceTestTask);
    if(ret != LOS_OK){
        dprintf("TraceTestTask create failed .\n");
        return LOS_NOK;
    }

    /* By default, the trace function is enabled. Therefore, you can disable the trace function, clear the buffer, and then restart the trace function. */
    LOS_TraceStop();
    LOS_TraceReset();

    /* Enable the event recording function of the task module. */
    LOS_TraceEventMaskSet(TRACE_TASK_FLAG);

    return LOS_OK;
}
```

## Verification<a name="en-us_topic_0278792249_section375141581110"></a>

The verification result is as follows:

```
 
*******TraceInfo begin*******
clockFreq = 50000000
CurObjIndex = 9
Index   TaskID   TaskPrio   TaskName 
0       0x10000  0          Swt_Task
1       0x10001  31         IdleCore000
2       0x10002  1          system_wq
3       0x10003  10         app_Task
4       0x10004  5          TestTraceTsk
5       0x10005  0          Swt_Task
6       0x10006  31         IdleCore000
7       0x10007  10         SendToSer
8       0x10008  5          tcpip_thread
9       0x0      0          

CurEvtIndex = 7
Index   Time(cycles)      EventType      CurTask   Identity      params    
0       0x78142f2         0x45           0x10004   0x10006       0x5          0x20         0x1f         
1       0x788b982         0x45           0x10006   0x10004       0x1f         0x4          0x5          
2       0x788ba2e         0x45           0x10004   0x10006       0x5          0x20         0x1f         
3       0x7905aa7         0x45           0x10003   0x10004       0xa          0x4          0x5          
4       0x7905b82         0x45           0x10004   0x10003       0x5          0x20         0xa          
5       0x7908402         0x45           0x10006   0x10008       0x1f         0x4          0x5          
6       0x79084a9         0x45           0x10003   0x10004       0xa          0x4          0x5   
 *******TraceInfo end*******
```

The two segments of data are task information and event information, respectively.

-   Task information includes task ID, task priority, and task name.
-   Event information includes occurrence time, event type, task in which the event occurs, main object of the event operation, and other parameters of the event.

    For details about the event type specified by EventType, see enum LOS\_TRACE\_TYPE in the header file  **los\_trace.h**.

    For details about the object described by the main object Identity and the event parameters indicated by params, see \#TYPE\#\_PARAMS in the header file  **los\_trace.h**.


## Complete Code<a name="en-us_topic_0278792249_section122211829161211"></a>

[sample\_trace.c](resource/sample_trace.c)

