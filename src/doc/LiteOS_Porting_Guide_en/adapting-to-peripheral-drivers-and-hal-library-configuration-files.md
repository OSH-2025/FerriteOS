# Adapting to Peripheral Drivers and HAL Library Configuration Files<a name="EN-US_TOPIC_0314628526"></a>

1.  Replace the chip peripheral driver file with the corresponding chip file.
    -   Modify the source file  **system\_xxx.c**  of the chip peripheral driver.

        The  **STM32F407\_OpenEdv\\Src\\system\_stm32f4xx.c**  file is modified by LiteOS. Therefore, this file cannot be used on the new development board. During porting, you can directly replace the file with the corresponding file in the bare metal project. For the STM32F407 development board, the corresponding file in the bare metal project is  **Core\\Src\\system\_stm32f4xx.c**.

    -   Modify the header file of the chip peripheral driver.

        Delete the header file  **STM32F407\_OpenEdv\\Inc\\stm32f429xx.h**  of the original STM32F429 chip peripheral driver and replace it with the corresponding file of the new development board. You can directly use the  **Drivers\\CMSIS\\Device\\ST\\STM32F4xx\\Include\\stm32f407xx.h**  file in the bare metal project.

        Note that the header file  **stm32f429xx.h**  of the original chip peripheral may be referenced in some files. You need to change it to  **stm32f407xx.h**  in the file. Currently, in the  **STM32F407\_OpenEdv**  directory of the new development board, only  **stm32f429xx.h**  is referenced in  **include\\asm\\hal\_platform\_ints.h**, change  **\#include "stm32f429xx.h"**  to  **\#include "stm32f407xx.h"**.

2.  Port the HAL library configuration file.

    Replace  **STM32F407\_OpenEdv\\Inc\\stm32f4xx\_hal\_conf.h**  with the  **Core\\Inc\\stm32f4xx\_hal\_conf.h**  file in the bare metal project.

3.  Delete the random number code.

    Currently, random numbers are not required. To reduce unnecessary porting work, comment out the code related to random numbers. Search for the keyword  **rng**, find the following lines in the  **STM32F407\_OpenEdv**  directory, and delete them:

    -   **Src\\sys\_init.c**:

        ```c
        #include "hal_rng.h"
        
        int atiny_random(void *output, size_t len)
        {
            return hal_rng_generate_buffer(output, len);
        }
        ```

    -   **Src\\main.c**:

        ```c
        #include "hal_rng.h"
        hal_rng_config();         
        ```

4.  In the first line of the  **STM32F407\_OpenEdv\\Src\\main.c**  hardware initialization function, add the HAL\_Init\(\) function for initializing the HAL library.

    ```c
    VOID HardwareInit(VOID)
    {
        HAL_Init();
        SystemClock_Config();
        MX_USART1_UART_Init();
        // hal_rng_config();
        dwt_delay_init(SystemCoreClock);
    }
    ```


