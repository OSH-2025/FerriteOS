# Programming Example<a name="EN-US_TOPIC_0311018428"></a>

## Example Description<a name="en-us_topic_0175230480_section19202902143129"></a>

In the programming example, the following functions are demonstrated:

1.  Create, delete, start, stop or restart a software timer.
2.  Use a one-shot software timer and a periodical software timer.

## Example Code<a name="en-us_topic_0175230480_section3113546143129"></a>

Prerequisite: The software timer module has been configured in the  **menuconfig**  menu.

The code is as follows:

```c
UINT32 g_timerCount1 = 0;
UINT32 g_timerCount2 = 0;

VOID Timer1_CallBack(UINT32 arg)
{
    UINT64 lastTick;

    g_timerCount1++;
    lastTick=(UINT32)LOS_TickCountGet();
    dprintf("g_timerCount1=%d\n", g_timerCount1);
    dprintf("tick_last1=%d\n", lastTick);
}

VOID Timer2_CallBack(UINT32 arg)
{
    UINT64 lastTick;

    lastTick=(UINT32)LOS_TickCountGet();
    g_timerCount2++;
    dprintf("g_timerCount2=%d\n", g_timerCount2);
    dprintf("tick_last2=%d\n", lastTick);
}

VOID Timer_example(VOID)
{
    UINT16 id1;     // Timer1 id
    UINT16 id2;     // Timer2 id
    UINT32 tick;

    LOS_SwtmrCreate(1000, LOS_SWTMR_MODE_ONCE, Timer1_CallBack, &id1, 1);
    LOS_SwtmrCreate(100, LOS_SWTMR_MODE_PERIOD, Timer2_CallBack, &id2, 1);
    dprintf("create Timer1 success\n");

    LOS_SwtmrStart(id1);
    dprintf("start Timer1 sucess\n");
    LOS_TaskDelay(200);
    LOS_SwtmrTimeGet(id1, &tick);
    dprintf("tick =%d\n", tick);
    LOS_SwtmrStop(id1);
    dprintf("stop Timer1 sucess\n");

    LOS_SwtmrStart(id1);
    LOS_TaskDelay(1000);
    LOS_SwtmrDelete(id1);
    dprintf("delete Timer1 sucess\n");

    LOS_SwtmrStart(id2);
    dprintf("start Timer2\n");
    LOS_TaskDelay(1000);
    LOS_SwtmrStop(id2);
    LOS_SwtmrDelete(id2); 
}
```

## Verification<a name="en-us_topic_0175230480_section3992572012944"></a>

The verification result is as follows:

```
Create Timer1 success
start Timer1 sucess
tick =800
Stop Timer1 sucess
g_timerCount1=1201
tick_last1=1201
delete Timer1 success
Start Timer2
g_timerCount2=1
tick_last2=1301
g_timerCount2=2
tick_last2=1401
g_timerCount2=3
tick_last2=1501
g_timerCount2=4
tick_last2=1601
g_timerCount2=5
tick_last2=1701
g_timerCount2=6
tick_last2=1801
g_timerCount2=7
tick_last2=1901
g_timerCount2=8
tick_last2=2001
g_timerCount2=9
tick_last2=2101
g_timerCount2=10
tick_last2=2201
```

## Complete Code<a name="en-us_topic_0175230480_section47519890121124"></a>

[sample\_Timer.c](resource/sample_Timer.c)

