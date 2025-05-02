# Programming Example<a name="EN-US_TOPIC_0311018397"></a>

## Example Description<a name="en-us_topic_0175230440_section1288182415417"></a>

The following functions are implemented:

1.  Creating interrupts
2.  Setting the interrupt affinity
3.  Enabling interrupts
4.  Triggering interrupts
5.  Disables an interrupt.
6.  Deleting interrupts

## Example Code<a name="en-us_topic_0175230440_section47263443154123"></a>

Prerequisite: The maximum number of interrupts that can be used and the number of interrupt priorities that can be set have been configured in the  **menuconfig**  menu.

The code is as follows:

```c
#include "los_hwi.h"
#include "los_typedef.h"
#include "los_task.h"

STATIC VOID HwiUsrIrq(VOID)
{
    printf("\n in the func HwiUsrIrq \n"); 
}

/* cpu0 trigger, cpu0 response */
UINT32 It_Hwi_001(VOID)
{
    UINT32 ret;
    UINT32 irqNum = 26; /* ppi */
    UINT32 irqPri = 0x3;

    ret = LOS_HwiCreate(irqNum, irqPri, 0, (HWI_PROC_FUNC)HwiUsrIrq, 0);
    if (ret != LOS_OK) {
        return LOS_NOK;
    }

#ifdef LOSCFG_KERNEL_SMP
    ret = LOS_HwiSetAffinity(irqNum, CPUID_TO_AFFI_MASK(ArchCurrCpuid()));
    if (ret != LOS_OK) {
        return LOS_NOK;
    }
#endif
    ret = LOS_HwiEnable(irqNum);
    if (ret != LOS_OK) {
        return LOS_NOK;
    }

    ret = LOS_HwiTrigger(irqNum);
    if (ret != LOS_OK) {
        return LOS_NOK;
    }

    LOS_TaskDelay(1);

    ret = LOS_HwiDisable(irqNum);
    if (ret != LOS_OK) {
        return LOS_NOK;
    }

    ret = LOS_HwiDelete(irqNum, NULL);
    if (ret != LOS_OK) {
        return LOS_NOK;
    }

    return LOS_OK;
}
```

## Complete Code<a name="en-us_topic_0175230440_section58415911122418"></a>

[sample\_hwi.c](resource/sample_hwi.c)

