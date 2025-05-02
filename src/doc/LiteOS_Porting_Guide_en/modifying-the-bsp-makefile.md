# Modifying the BSP Makefile<a name="EN-US_TOPIC_0319338642"></a>

Similar to LOSCFG\_PLATFORM\_STM32F429IGTX, add the HAL build corresponding to STM32F4 to  **targets\\bsp\\Makefile**.

```
else ifeq ($(LOSCFG_PLATFORM_STM32F407ZGTX), y)
STM32F407ZGTX_HAL_SRC = \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c_ex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_iwdg.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_eth.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rng.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sd.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_sdmmc.c \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sdram.c
    LOCAL_SRCS = $(STM32F407ZGTX_HAL_SRC)
```

