# Adapting to the GPIO Initialization File<a name="EN-US_TOPIC_0319337941"></a>

1.  Copy the  **gpio.c**  file in  **Core\\Src**  and the  **gpio.h**  file in  **Core\\Inc**  to  **targets\\STM32F407\_OpenEdv\\Src**  and  **targets\\STM32F407\_OpenEdv\\Inc\\**.
2.  Add  **Src/gpio.c**  to the  _USER\_SRC_  variable in the  **STM32F407\_OpenEdv\\Makefile**  file, that is, build GPIO into the project.

    ```
        USER_SRC = \
            os_adapt/os_adapt.c \
            Src/main.c \
            Src/system_stm32f4xx.c \
            Src/tim.c \
            Src/usart.c \
            Src/sys_init.c \
            Src/user_task.c \
            Src/gpio.c
    ```

3.  Add the following code to the  **targets\\STM32F407\_OpenEdv\\Inc\\gpio.h**  file to avoid errors caused by the lack of related macro definitions:

    ```
        #include "stm32f4xx_hal.h"
    ```


