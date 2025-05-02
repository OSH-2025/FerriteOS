include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := bsp

STM32F7xx_HAL_SRC = $(wildcard $(HAL_DRIVER_SRC)/Src/*.c)
ifeq ($(LOSCFG_PLATFORM_STM32F769IDISCOVERY)$(LOSCFG_PLATFORM_STM32F746_NUCLEO)$(LOSCFG_PLATFORM_STM32F767_NUCLEO), y)
STM32F7xx_HAL_OUT = \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_crc.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_usb.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_utils.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_mmc.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_smbus.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_timebase_rtc_alarm_template.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_timebase_rtc_wakeup_template.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_hal_timebase_tim_template.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_dac.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_dma.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_dma2d.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_exti.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_gpio.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_i2c.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_lptim.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_pwr.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_rcc.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_rng.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_rtc.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_spi.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_tim.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_usart.c \
    drivers/STM32F7xx_HAL_Driver/Src/stm32f7xx_ll_utils.c
    STM32F7xx_HAL_SRC := $(filter-out $(STM32F7xx_HAL_OUT), $(STM32F7xx_HAL_SRC))
    LOCAL_SRCS = $(STM32F7xx_HAL_SRC)
else ifeq ($(LOSCFG_PLATFORM_STM32F429IGTX), y)
STM32F429IGTX_HAL_SRC = \
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_can.c \
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
    LOCAL_SRCS = $(STM32F429IGTX_HAL_SRC)
else ifeq ($(LOSCFG_PLATFORM_STM32F407_ATK_EXPLORER), y)
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
    drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rng.c
    LOCAL_SRCS = $(STM32F407ZGTX_HAL_SRC)
else ifeq ($(findstring $(LOSCFG_PLATFORM_STM32L431_BearPi)$(LOSCFG_PLATFORM_STM32L4R9I_DISCOVERY)$(LOSCFG_PLATFORM_STM32L496_NUCLEO)$(LOSCFG_PLATFORM_STM32L476_NB476),y), y)
STM32L4_HAL_SRC = \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim_ex.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr_ex.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_pwr.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_gpio.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rcc_ex.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ex.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_flash_ramfunc.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_cortex.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_uart_ex.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_tim.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_spi.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_qspi.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_i2c_ex.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_iwdg.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_adc.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_dma_ex.c \
    drivers/STM32L4xx_HAL_Driver/Src/stm32l4xx_hal_rng.c
    LOCAL_SRCS = $(STM32L4_HAL_SRC)

else ifeq ($(LOSCFG_PLATFORM_STM32F103_FIRE_ARBITRARY), y)
STM32F103VETX_HAL_SRC = \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2c.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_iwdg.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_eth.c
    LOCAL_SRCS = $(STM32F103VETX_HAL_SRC)
else ifeq ($(LOSCFG_PLATFORM_STM32F072_Nucleo), y)
STM32F072RBTX_HAL_SRC = \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_i2c.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_flash_ex.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_rcc_ex.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_gpio.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_pwr.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim_ex.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_cortex.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_uart.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_tim.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_spi.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_iwdg.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal.c \
    drivers/STM32F0xx_HAL_Driver/Src/stm32f0xx_hal_dma.c
    LOCAL_SRCS = $(STM32F072RBTX_HAL_SRC)
else ifeq ($(LOSCFG_PLATFORM_CB2201), y)
    CSKY_HAL_SRC = $(wildcard $(HAL_DRIVER_SRC)/Src/*.c) 
    LOCAL_SRCS = $(CSKY_HAL_SRC)
else ifeq ($(LOSCFG_PLATFORM_SMARTL_E802), y)
    QEMU_CSKY_HAL_SRC = $(wildcard $(HAL_DRIVER_SRC)/Src/*.c) 
    LOCAL_SRCS = $(QEMU_CSKY_HAL_SRC)
else ifeq ($(LOSCFG_PLATFORM_GD32F303RGT6_BEARPI), y)
    GD32F303RGT6_DRIVER_SRC = $(wildcard drivers/$(GD_DRIVER_TYPE)/Source/*.c)
    LOCAL_SRCS = $(GD32F303RGT6_DRIVER_SRC)
else ifeq ($(LOSCFG_PLATFORM_GD32E103C_START), y)
    GD32E103CBT6_DRIVER_SRC = $(wildcard drivers/$(GD_DRIVER_TYPE)/Source/*.c)
    LOCAL_SRCS = $(GD32E103CBT6_DRIVER_SRC)
else ifeq ($(LOSCFG_PLATFORM_STM32L073_NUCLEO), y)
STM32L073RZT6_DRIVER_SRC = \
    $(HAL_DRIVER_SRC)/Src/stm32l0xx_hal_tim.c \
    $(HAL_DRIVER_SRC)/Src/stm32l0xx_hal_tim_ex.c \
    $(HAL_DRIVER_SRC)/Src/stm32l0xx_hal_uart.c \
    $(HAL_DRIVER_SRC)/Src/stm32l0xx_hal_uart_ex.c \
    $(HAL_DRIVER_SRC)/Src/stm32l0xx_hal.c \
    $(HAL_DRIVER_SRC)/Src/stm32l0xx_hal_i2c.c \
    $(HAL_DRIVER_SRC)/Src/stm32l0xx_hal_i2c_ex.c \
    $(HAL_DRIVER_SRC)/Src/stm32l0xx_hal_rcc.c \
    $(HAL_DRIVER_SRC)/Src/stm32l0xx_hal_rcc_ex.c \
    $(HAL_DRIVER_SRC)/Src/stm32l0xx_hal_flash_ramfunc.c \
    $(HAL_DRIVER_SRC)/Src/stm32l0xx_hal_flash.c \
    $(HAL_DRIVER_SRC)/Src/stm32l0xx_hal_flash_ex.c \
    $(HAL_DRIVER_SRC)/Src/stm32l0xx_hal_gpio.c \
    $(HAL_DRIVER_SRC)/Src/stm32l0xx_hal_dma.c \
    $(HAL_DRIVER_SRC)/Src/stm32l0xx_hal_pwr.c \
    $(HAL_DRIVER_SRC)/Src/stm32l0xx_hal_pwr_ex.c \
    $(HAL_DRIVER_SRC)/Src/stm32l0xx_hal_cortex.c \
    $(HAL_DRIVER_SRC)/Src/stm32l0xx_hal_exti.c
    LOCAL_SRCS = $(STM32L073RZT6_DRIVER_SRC)
else ifeq ($(LOSCFG_PLATFORM_STM32L552_NUCLEO), y)
STM32L552ZET6Q_HAL_SRC = \
    $(HAL_DRIVER_SRC)/Src/stm32l5xx_hal_i2c.c \
    $(HAL_DRIVER_SRC)/Src/stm32l5xx_hal_flash.c \
    $(HAL_DRIVER_SRC)/Src/stm32l5xx_hal_flash_ex.c \
    $(HAL_DRIVER_SRC)/Src/stm32l5xx_hal_rcc.c \
    $(HAL_DRIVER_SRC)/Src/stm32l5xx_hal_rcc_ex.c \
    $(HAL_DRIVER_SRC)/Src/stm32l5xx_hal_gpio.c \
    $(HAL_DRIVER_SRC)/Src/stm32l5xx_hal_pwr.c \
    $(HAL_DRIVER_SRC)/Src/stm32l5xx_hal_pwr_ex.c \
    $(HAL_DRIVER_SRC)/Src/stm32l5xx_hal_tim_ex.c \
    $(HAL_DRIVER_SRC)/Src/stm32l5xx_hal_cortex.c \
    $(HAL_DRIVER_SRC)/Src/stm32l5xx_hal_uart.c \
    $(HAL_DRIVER_SRC)/Src/stm32l5xx_hal_uart_ex.c \
    $(HAL_DRIVER_SRC)/Src/stm32l5xx_hal_tim.c \
    $(HAL_DRIVER_SRC)/Src/stm32l5xx_hal.c \
    $(HAL_DRIVER_SRC)/Src/stm32l5xx_hal_dma.c
    LOCAL_SRCS = $(STM32L552ZET6Q_HAL_SRC)
else ifeq ($(LOSCFG_PLATFORM_GD32VF103V_EVAL), y)
GD32VF103VET6_GD_SRC = \
    $(wildcard drivers/$(GD_DRIVER_TYPE)/Source/*.c) \
    $(wildcard drivers/GD32VF103_Firmware/GD32VF103_Firmware_Library/Firmware/RISCV/drivers/*.c) \
    $(wildcard drivers/GD32VF103_Firmware/GD32VF103_Firmware_Library/Firmware/RISCV/stubs/*.c)
    LOCAL_SRCS = $(GD32VF103VET6_GD_SRC)
else ifeq ($(LOSCFG_PLATFORM_HIFIVE1_REV1_B01), y)
FE310_SIFIVE_SRC = \
    $(wildcard  $(HAL_DRIVER_SRC)/src/drivers/*.c) \
    $(wildcard  $(HAL_DRIVER_SRC)/src/*.c) \
    $(wildcard  $(HAL_DRIVER_SRC)/src/*.S) \
    $(wildcard  $(HAL_DRIVER_SRC)/gloss/*.S)
    LOCAL_SRCS = $(FE310_SIFIVE_SRC)
else ifeq ($(LOSCFG_PLATFORM_FM33LC0XX_DEMO), y)
FM33LC0XX_HAL_SRC = \
    $(wildcard  $(HAL_DRIVER_SRC)/Src/*.c)
    LOCAL_SRCS = $(FM33LC0XX_HAL_SRC)
else ifeq ($(LOSCFG_PLATFORM_APM32F103_GEEHY), y)
    APM32F103ZET6_DRIVER_SRC = $(wildcard drivers/APM32F10x_StdPeriphDriver/Src/*.c)
    LOCAL_SRCS = $(APM32F103ZET6_DRIVER_SRC)
else ifeq ($(LOSCFG_PLATFORM_RASPBERRY_PI2B), y)
    RASPBERRY_PI2B_DRIVER_SRC = $(wildcard $(HAL_DRIVER_SRC)/Src/*.c)
    LOCAL_SRCS = $(RASPBERRY_PI2B_DRIVER_SRC)
else ifeq ($(LOSCFG_PLATFORM_STM32F103_C8T6), y)
STM32F103C8T6_HAL_SRC = \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_i2c.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_flash_ex.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_rcc_ex.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_gpio.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_pwr.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim_ex.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_cortex.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_uart.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_tim.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_spi.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_iwdg.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_dma.c \
    drivers/STM32F1xx_HAL_Driver/Src/stm32f1xx_hal_eth.c
    LOCAL_SRCS = $(STM32F103C8T6_HAL_SRC)
endif

LOCAL_INCLUDE += $(PLATFORM_INCLUDE) \
    -I $(LITEOSTOPDIR)/include \
    -I $(LITEOSTOPDIR)/lib/libc/stdio \
    -I $(LITEOSTOPDIR)/compat/posix/src \
    -I $(LITEOSTOPDIR)/targets/bsp/common \
    -I $(LITEOSTOPDIR)/targets/bsp/include \
    -I $(LITEOSTOPDIR)/targets/bsp/hw/include \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Inc \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/include \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Hardware/Inc \
    -I $(LITEOSTOPDIR)/targets/bsp/$(LITEOS_PLATFORM)/include \
    -I $(LITEOSTOPDIR)/compat/cmsis

ifeq ($(LOSCFG_KERNEL_TICKLESS), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/kernel/extended/include
endif

ifeq ($(LOSCFG_KERNEL_TRACE), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/kernel/extended/include
endif

ifeq ($(LOSCFG_KERNEL_CPUP), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/kernel/extended/include
endif

ifeq ($(LOSCFG_KERNEL_RUNSTOP), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/kernel/extended/include
endif

ifndef LOSCFG_PLATFORM_OSAPPINIT
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/test/include
endif

ifeq ($(findstring $(LOSCFG_SHELL)$(LOSCFG_SHELL_LK)$(LOSCFG_SHELL_DMESG), y y y),)
    LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/shell/include
endif

ifeq ($(LOSCFG_ARCH_ARM_CORTEX_M), y)
LOCAL_SRCS += common/dwt.c
endif

LOCAL_SRCS += $(wildcard common/demo_entry.c)
LOCAL_SRCS += $(wildcard common/fs/*.c)

ifeq ($(LOSCFG_COMPONENTS_NET_AT), y)
LOCAL_SRCS += $(wildcard drivers/at_hal/at_hal.c)
LOCAL_INCLUDE += \
    -I $(LITEOSTOPDIR)/components/net/net_at/at_frame \
    -I $(LITEOSTOPDIR)/components/net/net_at/at_device
endif

ifeq ($(LOSCFG_PLATFORM_HIFIVE1_REV1_B01), y)
    LOCAL_EXT_FLAG := -Wno-strict-prototypes -Wno-maybe-uninitialized
endif

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS) \
    $(LITEOS_CFLAGS_INTERWORK) $(PLATFORM_DEFS) $(LOCAL_EXT_FLAG)

include $(MODULE)
