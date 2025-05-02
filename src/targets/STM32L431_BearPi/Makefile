include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(LITEOS_PLATFORM)

-include $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/config.mk

ALL_ASSRCS := $(wildcard board/$(LITEOS_PLATFORM)/*.S)
ASSRCS := $(subst board/$(LITEOS_PLATFORM)/board.ld.S,,$(ALL_ASSRCS))

LOCAL_SRCS += $(ASSRCS)

LOCAL_SRCS += \
    ${wildcard $(LITEOSTOPDIR)/targets/STM32L431_BearPi/Hardware/Src/*.c} \
    ${wildcard $(LITEOSTOPDIR)/targets/STM32L431_BearPi/Hardware/LCD/*.c} \
    $(LITEOSTOPDIR)/targets/STM32L431_BearPi/os_adapt/os_adapt.c \
    $(LITEOSTOPDIR)/targets/STM32L431_BearPi/Src/gpio.c \
    $(LITEOSTOPDIR)/targets/STM32L431_BearPi/Src/Huawei_IoT_QR_Code.c \
    $(LITEOSTOPDIR)/targets/STM32L431_BearPi/Src/i2c.c \
    $(LITEOSTOPDIR)/targets/STM32L431_BearPi/Src/loader_main.c \
    $(LITEOSTOPDIR)/targets/STM32L431_BearPi/Src/main.c \
    $(LITEOSTOPDIR)/targets/STM32L431_BearPi/Src/spi.c \
    $(LITEOSTOPDIR)/targets/STM32L431_BearPi/Src/stm32l4xx_it.c \
    $(LITEOSTOPDIR)/targets/STM32L431_BearPi/Src/sys_init.c \
    $(LITEOSTOPDIR)/targets/STM32L431_BearPi/Src/system_stm32l4xx.c \
    $(LITEOSTOPDIR)/targets/STM32L431_BearPi/Src/usart.c \
    $(LITEOSTOPDIR)/targets/STM32L431_BearPi/Src/tim.c \
    $(LITEOSTOPDIR)/targets/STM32L431_BearPi/Src/user_task.c \
    $(LITEOSTOPDIR)/targets/STM32L431_BearPi/Src/flash_adaptor.c

ifeq ($(LOSCFG_COMPONENTS_FS), y)
    ifeq ($(LOSCFG_FATFS_USE_SPI_FLASH), y)
        LOCAL_SRCS += \
            Src/fatfs_hal.c
    endif
endif

# C includes
LOCAL_INCLUDE += \
    -I $(LITEOSTOPDIR)/targets/bsp/include \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/STM32L4xx_HAL_Driver/Inc \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/STM32L4xx_HAL_Driver/Inc/Legacy \
    -I $(LITEOSTOPDIR)/targets/STM32L431_BearPi/Hardware/Inc \
    -I $(LITEOSTOPDIR)/targets/STM32L431_BearPi/Hardware/LCD \
    -I $(LITEOSTOPDIR)/targets/STM32L431_BearPi/Inc \
    -I $(LITEOSTOPDIR)/targets/bsp/common \
    -I $(LITEOSTOPDIR)/targets/bsp/include \
    -I $(LITEOSTOPDIR)/targets/bsp/hw/include \
    -I $(LITEOSTOPDIR)/compat/posix/src \
    -I $(LITEOSTOPDIR)/targets/STM32L431_BearPi/include \
    -I $(LITEOSTOPDIR)/kernel/extended/include \
    -I $(LITEOSTOPDIR)/targets/STM32L431_BearPi/Inc \
    -I $(LITEOSTOPDIR)/targets/STM32L431_BearPi/include \
    -I $(LITEOSTOPDIR)/include

# C defines
C_DEFS = \
    -D USE_HAL_DRIVER \
    -D NDEBUG \
    -D __LITEOS__ \
    -D _ALL_SOURCE

LOCAL_SRCS += \
    ${wildcard $(LITEOSTOPDIR)/targets/STM32L431_BearPi/los_startup_gcc.S}

LOCAL_FLAGS := $(C_DEFS) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS) $(LITEOS_CFLAGS_INTERWORK)

include $(MODULE)
