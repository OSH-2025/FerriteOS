# Programming Example<a name="EN-US_TOPIC_0311018413"></a>

## Example Description <a name="en-us_topic_0175230427_section57243849151319"></a>

In the programming example, the Example\_TaskEntry task is executed to create the Example\_Event task. The Example\_Event task is blocked from reading events. The Example\_TaskEntry task writes events to the task. You can understand the task switching during event operations based on the sequence in which logs are recorded.

1.  The Example\_TaskEntry task is executed to create the Example\_Event task. The Example\_Event task takes a higher priority than the Example\_TaskEntry task.
2.  The Example\_Event task is blocked from reading the event 0x00000001. After the Example\_Event task is blocked, a task switching occurs to execute the task with a lower priority, namely, the Example\_TaskEntry task.
3.  The Example\_TaskEntry task writes the event 0x00000001 toward the Example\_Event task. Task switching occurs, and the Example\_Event task is executed.
4.  The Example\_Event task is executed until the task is complete.
5.  The Example\_TaskEntry task is executed until the task is complete.

## Example Code<a name="en-us_topic_0175230427_section31107981151351"></a>

Prerequisite: The event module has been configured in the  **menuconfig**  menu.

The code is as follows:

```c
#include "los_event.h"
#include "los_task.h"
#include "securec.h"

/*Task ID*/
UINT32 g_testTaskId;

/*Event control structure*/ 
EVENT_CB_S g_exampleEvent;

/*Event that the Example_Event task is waiting for*/ 
#define EVENT_WAIT 0x00000001

 /*Task entrypoint function*/ 
VOID Example_Event(VOID)
{
    UINT32 ret;
    UINT32 event;

    /*Wait for a completion in timeout mode, and the timeout interval is 100 ticks. If the event is not read within 100 ticks, the read operation times out and the task is woken up.*/ 
    printf("Example_Event wait event 0x%x \n", EVENT_WAIT);

    event = LOS_EventRead(&g_exampleEvent, EVENT_WAIT, LOS_WAITMODE_AND, 100);
    if (event == EVENT_WAIT) {
        printf("Example_Event,read event :0x%x\n", event);
    } else {
        printf("Example_Event,read event timeout\n");
    }
}

UINT32 Example_TaskEntry(VOID)
{
    UINT32 ret;
    TSK_INIT_PARAM_S task1;

    /*Initialize the event.*/ 
    ret = LOS_EventInit(&g_exampleEvent);
    if (ret != LOS_OK) {
        printf("init event failed .\n");
        return -1;
    }

    /*Create the task.*/ 
    (VOID)memset_s(&task1, sizeof(TSK_INIT_PARAM_S), 0, sizeof(TSK_INIT_PARAM_S));
    task1.pfnTaskEntry = (TSK_ENTRY_FUNC)Example_Event;
    task1.pcName       = "EventTsk1";
    task1.uwStackSize  = OS_TSK_DEFAULT_STACK_SIZE;
    task1.usTaskPrio   = 5;
    ret = LOS_TaskCreate(&g_testTaskId, &task1);
    if (ret != LOS_OK) {
        printf("task create failed .\n");
        return LOS_NOK;
    }

    /* Write the g_testTaskId wait event. */
    printf("Example_TaskEntry write event .\n");

    ret = LOS_EventWrite(&g_exampleEvent, EVENT_WAIT);
    if (ret != LOS_OK) {
        printf("event write failed .\n");
        return LOS_NOK;
    }

    /*Clear the flag.*/ 
    printf("EventMask:%d\n", g_exampleEvent.uwEventID);
    LOS_EventClear(&g_exampleEvent, ~g_exampleEvent.uwEventID);
    printf("EventMask:%d\n", g_exampleEvent.uwEventID);

    /*Delete the task.*/ 
    ret = LOS_TaskDelete(g_testTaskId);
    if (ret != LOS_OK) {
        printf("task delete failed .\n");
        return LOS_NOK;
    }

    return LOS_OK;
}
```

## Verification<a name="en-us_topic_0175230427_section9844937144850"></a>

The verification result is as follows:

```
Example_Event wait event 0x1 
Example_TaskEntry write event .
Example_Event,read event :0x1
EventMask:1
EventMask:0
```

## Complete Code<a name="en-us_topic_0175230427_section33904501144850"></a>

[sample\_event.c](resource/sample_event.c)

