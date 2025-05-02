# Modifying the Link Script<a name="EN-US_TOPIC_0314628529"></a>

**STM32F407\_OpenEdv\\liteos.ld**  is the link script of the new development board. You need to change the values of  **stack**,  **flash**, and  **ram**  based on the actual situation of the development board, you can set this parameter by referring to the value in the  **STM32F407ZGTx\_FLASH.ld**  script.

-   In the link script,  **stack**  corresponds to the  _\_estack_  variable.
-   **flash**  corresponds to the  _FLASH_  variable.
-   **ram**  corresponds to the  _RAM_  variable.

In addition, to adapt to the LiteOS, the following code is added to the link script:

1.  Add a vector to initialize LiteOS.

    ```
    /* used by the startup to initialize liteos vector */
    _si_liteos_vector_data = LOADADDR(.vector_ram);
    
    /* Initialized liteos vector sections goes into RAM, load LMA copy after code */
    .vector_ram :
    {
    . = ORIGIN(RAM);
    _s_liteos_vector = .;
    *(.data.vector)    /* liteos vector in ram */
    _e_liteos_vector = .;
    } > RAM AT> FLASH
    ```

2.  Add the definition of the  _\_\_bss\_end_  variable to the .bss segment because this variable is used in LiteOS instead of  _\_\_bss\_end\_\__.

    ```
    __bss_end = _ebss;
    ```

3.  Set the address of the memory pool used by LiteOS, including the start address and end address.

    ```
    . = ALIGN(8);
    __los_heap_addr_start__ = .;
    __los_heap_addr_end__ = ORIGIN(RAM) + LENGTH(RAM) - _Min_Stack_Size - 1;
    ```


