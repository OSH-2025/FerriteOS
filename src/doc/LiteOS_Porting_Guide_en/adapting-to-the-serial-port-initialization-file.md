# Adapting to the Serial Port Initialization File<a name="EN-US_TOPIC_0314628528"></a>

1.  Use the serial port initialization files  **Core\\Src\\usart.c**  and  **Core\\Inc\\usart.h**  of the bare metal project to replace **targets\\STM32F407\_OpenEdv\\Src\\usart.c**  and  **targets\\STM32F407\_OpenEdv\\Inc\\usart.h**  in the LiteOS source code.
2.  Add the reference to the HAL driver header file of the STM32F4 series chips in **targets\\STM32F407\_OpenEdv\\Inc\\usart.h**.

    ```c
    #include "stm32f4xx_hal.h"
    ```

3.  Add the following function definition to the end of the **targets\\STM32F407\_OpenEdv\\Src\\usart.c** file:

    ```c
    __attribute__((used)) int _write(int fd, char *ptr, int len)
    {
        (void)HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, 0xFFFF);
        return len;
    }
    ```


