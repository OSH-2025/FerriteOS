# Configuring the System Clock<a name="EN-US_TOPIC_0314628527"></a>

1.  Set the system clock rate.

    You can set the clock rate in the  **STM32F407\_OpenEdv\\include\\hisoc\\clock.h**  file. Generally, the clock rate is set to  **SystemCoreClock**. The implementation code is as follows:

    ```c
    #define get_bus_clk()  SystemCoreClock
    ```

2.  Modify the system clock configuration function SystemClock\_Config\(\).

    Functions are defined in the  **STM32F407\_OpenEdv\\Src\\sys\_init.c**  file. You can use the functions in  **Core\\Src\\main.c**  of the bare metal project.


