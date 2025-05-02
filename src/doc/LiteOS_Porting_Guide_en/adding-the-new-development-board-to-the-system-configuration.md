# Adding the New Development Board to the System Configuration<a name="EN-US_TOPIC_0314628532"></a>

1.  Modify **targets\\targets.mk**.

    You can refer to the build configuration of other development boards to add the configuration of the ALIENTEK development board.

    ```
    ######################### STM32F407ZGTX Options###############################
    else ifeq ($(LOSCFG_PLATFORM_STM32F407ZGTX), y)
        TIMER_TYPE := arm/timer/arm_cortex_m
        LITEOS_CMACRO_TEST += -DSTM32F407xx
        HAL_DRIVER_TYPE := STM32F4xx_HAL_Driver
    ```

2.  Add  **STM32F407\_OpenEdv.config**.

    Copy the  **Cloud\_STM32F429IGTx\_FIRE.config**  file in **tools\\build\\config**, rename the file  **STM32F407\_OpenEdv.config**, change **Cloud\_STM32F429IGTx\_FIRE** in the file to  **STM32F407\_OpenEdv**, and change **LOSCFG\_PLATFORM\_STM32F429IGTX** to **LOSCFG\_PLATFORM\_STM32F407ZGTX**.

3.  Modify  **targets\\Kconfig**  to add the menuconfig option.

    a.  Add the following content by referring to **config LOSCFG\_PLATFORM\_STM32F429IGTX**:

        config LOSCFG_PLATFORM_STM32F407ZGTX
            bool "STM32F407_OpenEdv"
            select LOSCFG_USING_BOARD_LD
            select LOSCFG_ARCH_CORTEX_M4

    b.  Add STM32F407\_OpenEdv to the help information under the **choice** item.

    c.  Add the following content under  **config LOSCFG\_PLATFORM**:

        default "STM32F407_OpenEdv"         if LOSCFG_PLATFORM_STM32F407ZGTX

    >![](public_sys-resources/icon-note.gif) **NOTE:**
    >
    >**STM32F407\_OpenEdv** is the name of the development board directory added in  [Adding the Directory of the New Development Board](adding-the-directory-of-the-new-development-board.md).


4.  Modify the .config file.

    Copy the **tools\\build\\config\\STM32F407\_OpenEdv.config**  file to the root directory of LiteOS and rename it  **.config**  to replace the original .config file in the root directory.
