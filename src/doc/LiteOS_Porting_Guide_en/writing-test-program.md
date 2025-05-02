# Writing Test Program<a name="EN-US_TOPIC_0314628521"></a>

Write the test code in the  **Core\\Src\\main.c**  file of the bare device project to implement the cyclic output of the serial ports and the LED indicator blinks.

1.  Add a header file:

    ```
    #include <stdio.h>
    ```

2.  Add the following code to the while\(1\) loop of the main\(\) function:

    ```
    printf("hello\n");
    HAL_Delay(1000);
    HAL_GPIO_TogglePin(GPIOF, GPIO_PIN_9);
    ```

3.  Add the following function to  **/\* USER CODE BEGIN 4 \*/**:

    ```
    __attribute__((used)) int _write(int fd, char *ptr, int len)
    {
        (void)HAL_UART_Transmit(&huart1, (uint8_t *)ptr, len, 0xFFFF);
        return len;
    }
    ```


