# Programming Example<a name="EN-US_TOPIC_0311018439"></a>

## Example Description<a name="en-us_topic_0175230231_section662833410449"></a>

The programming example will cover the following functions:

1.  Time conversion: from milliseconds to ticks, or conversely
2.  Time measurement and deferral: measures the number of cycles per second, the number of ticks for which the system is running, and the number of ticks for which the deferral lasts

## Example Code<a name="en-us_topic_0175230231_section4101244910522"></a>

Prerequisites

-   The default value  **100**  of  **LOSCFG\_BASE\_CORE\_TICK\_PER\_SECOND**  is used. This configuration item specifies the number of ticks in a second.
-   The system main clock frequency  **OS\_SYS\_CLOCK**  \(in Hz\) is set.

Time conversion:

```c
VOID Example_TransformTime(VOID)
{
    UINT32 ms;
    UINT32 tick;

     uwTick = LOS_MS2Tick(10000);//Convert 10000 ms into ticks 
    dprintf("tick = %d \n",tick);
     uwMs= LOS_Tick2MS(100);//Convert 100 ticks into ms 
    dprintf("ms = %d \n",ms);
}
```

Time measurement and delay:

```c
VOID Example_GetTime(VOID)
{
    UINT32 cyclePerTick;
    UINT64 tickCount;

    cyclePerTick  = LOS_CyclePerTickGet();
    if(0 != cyclePerTick) {
        dprintf("LOS_CyclePerTickGet = %d \n", cyclePerTick);
    }

    tickCount = LOS_TickCountGet();
    if(0 != tickCount) {
        dprintf("LOS_TickCountGet = %d \n", (UINT32)tickCount);
    }

    LOS_TaskDelay(200);
    tickCount = LOS_TickCountGet();
    if(0 != tickCount) {
        dprintf("LOS_TickCountGet after delay = %d \n", (UINT32)tickCount);
    }
}
```

## Verification<a name="en-us_topic_0175230231_section183091759378"></a>

The verification result is as follows:

Time conversion:

```
tick = 1000
ms = 1000
```

Time measurement and delay:

```
LOS_CyclePerTickGet = 495000 
LOS_TickCountGet = 1 
LOS_TickCountGet after delay = 201
```

## Complete Code<a name="en-us_topic_0175230231_section4241230112430"></a>

[sample\_time.c](resource/sample_time.c)

