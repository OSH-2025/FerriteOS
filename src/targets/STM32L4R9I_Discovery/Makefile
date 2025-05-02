include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(LITEOS_PLATFORM)

-include $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/config.mk

ALL_ASSRCS := $(wildcard board/$(LITEOS_PLATFORM)/*.S)
ASSRCS := $(subst board/$(LITEOS_PLATFORM)/board.ld.S,,$(ALL_ASSRCS))

LOCAL_SRCS += $(ASSRCS)

USER_SRC = \
    os_adapt/os_adapt.c \
    Src/main.c \
    Src/system_stm32l4xx.c \
    Src/gpio.c \
    Src/tim.c \
    Src/usart.c \
    Src/sys_init.c \
    Src/user_task.c
    C_SOURCES += $(USER_SRC)

# C includes
HAL_DRIVER_INC = \
   -I $(LITEOSTOPDIR)/targets/bsp/drivers/STM32L4xx_HAL_Driver/Inc \
   -I $(LITEOSTOPDIR)/targets/bsp/drivers/STM32L4xx_HAL_Driver/Inc/Legacy
    BOARD_INCLUDES += $(HAL_DRIVER_INC)

INCLUDE_INC = \
    -I $(LITEOSTOPDIR)/include
    BOARD_INCLUDES += $(INCLUDE_INC)

BSP_INC = \
    -I $(LITEOSTOPDIR)/targets/bsp/common \
    -I $(LITEOSTOPDIR)/targets/bsp/include \
    -I $(LITEOSTOPDIR)/targets/bsp/hw/include \
    -I $(LITEOSTOPDIR)/targets/STM32L4R9I_Discovery/include \
    -I $(LITEOSTOPDIR)/kernel/extended/include \
    -I $(LITEOSTOPDIR)/kernel/include
    BOARD_INCLUDES += $(BSP_INC)

USER_INC = \
    -I $(LITEOSTOPDIR)/targets/STM32L4R9I_Discovery/Inc
    BOARD_INCLUDES += $(USER_INC)

# C defines
C_DEFS += \
    -DUSE_HAL_DRIVER \
    -DSTM32L4R9xx \
    -DNDEBUG \
    -D__LITEOS__ \
    -D_ALL_SOURCE

ASM_SOURCES = \
    ${wildcard los_startup_gcc.S}

BOARD_SRCS += $(ASM_SOURCES)
BOARD_SRCS += $(C_SOURCES)

LOCAL_SRCS := $(subst $(CURDIR)/,, $(BOARD_SRCS))

BOARD_DEF += $(C_DEFS)
LOCAL_INCLUDE += $(BOARD_INCLUDES)

LOCAL_FLAGS := $(LITEOS_CFLAGS_INTERWORK) $(BOARD_DEF) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
