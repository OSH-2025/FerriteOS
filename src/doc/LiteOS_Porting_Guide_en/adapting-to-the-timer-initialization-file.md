# Adapting to the Timer Initialization File<a name="EN-US_TOPIC_0319337940"></a>

1.  Replace  **targets\\STM32F407\_OpenEdv\\Src\\tim.c**  and  **targets\\STM32F407\_OpenEdv\\Inc\\tim.h**  in the LiteOS source code with the timer initialization files  **Core\\Src\\tim.c**  and  **Core\\Inc\\tim.h**  of the bare metal project.
2.  <a name="li7771155672316"></a>Compared with LiteOS, the bare metal project does not have the declaration of the related function \(GetTimerCycles, TimerHwiCreate, and TimInit\) and the definition of the enumeration type Timer\_t. Therefore, the following code is added to the  **targets\\STM32F407\_OpenEdv\\Inc\\tim.h**  file:

    ```
        #include "los_typedef.h"
        typedef enum {
            TIMER1 = 1,
            TIMER2,
            TIMER3,
            TIMER4,
            TIMER5,
            TIMER6,
            TIMER7,
            TIMER8
        } Timer_t;
    
        UINT64 GetTimerCycles(Timer_t num);
        VOID TimerHwiCreate(VOID);
        VOID TimInit(VOID);
    ```

3.  Add the implementation of the corresponding function in  [2](#li7771155672316)  and add the following code to the  **targets\\STM32F407\_OpenEdv\\Src\\tim.c**  file:

    ```
        #include "los_hwi.h"
    
        #define TIMER3_RELOAD 50000
        UINT64 Timer3Getcycle(VOID)
        {
            static UINT64 bacCycle;
            static UINT64 cycleTimes;
            UINT64 swCycles = htim3.Instance->CNT;
    
            if (swCycles <= bacCycle) {
                cycleTimes++;
            }
            bacCycle = swCycles;
            return swCycles + cycleTimes * TIMER3_RELOAD;
        }
    
        VOID TimInit(VOID)
        {
            MX_TIM3_Init();
        }
    
        VOID TimerHwiCreate(VOID)
        {
            UINT32 ret;
    
            ret = LOS_HwiCreate(TIM_IRQ, 0, 0, TIM3_IRQHandler, 0); // 16: cortex-m irq num shift
            if (ret != 0) {
                printf("ret of TIM3 LOS_HwiCreate = %#x\n", ret);
                return;
            }
            HAL_TIM_Base_Start_IT(&htim3);
        }
    
        UINT64 GetTimerCycles(Timer_t num)
        {
            UINT64 cycles = 0;
    
            switch (num) {
                case 3:
                    cycles = Timer3Getcycle();
                    break;
                default:
                    printf("Wrong number of TIMER.\n");
            }
            return cycles;
        }
    ```


