include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(LITEOS_PLATFORM)

# LOCAL_SRCS += $(ASSRCS)

USER_SRC = \
    $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Src/main.c \
    $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Src/gpio.c \
    $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Src/handlers.c \
    $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Src/usart.c \
    $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Src/tim.c \
    $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Src/user_task.c \
    $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Src/system_gd32vf103.c
    C_SOURCES += $(USER_SRC)

# C includes
GD_DRIVER_INC = \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/GD32VF103_Firmware/GD32VF103_Firmware_Library/Firmware/GD32VF103_standard_peripheral/Include \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/GD32VF103_Firmware/GD32VF103_Firmware_Library/Firmware/RISCV/drivers \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/GD32VF103_Firmware/GD32VF103_Firmware_Library/Firmware/RISCV/stubs
    BOARD_INCLUDES += $(GD_DRIVER_INC)

INCLUDE_INC = \
    -I $(LITEOSTOPDIR)/include
    BOARD_INCLUDES += $(INCLUDE_INC)

BSP_INC = \
    -I $(LITEOSTOPDIR)/targets/bsp/common \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/include \
    -I $(LITEOSTOPDIR)/kernel/extended/include
    BOARD_INCLUDES += $(BSP_INC)

USER_INC = \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Inc
    BOARD_INCLUDES += $(USER_INC)

ASM_SOURCES = \
    ${wildcard $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/start.S} \
    ${wildcard $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/entry.S}

C_DEFS =  \
    -DUSE_STDPERIPH_DRIVER \
    -DGD32VF103V_EVAL

BOARD_SRCS += $(ASM_SOURCES)
BOARD_SRCS += $(C_SOURCES)
BOARD_DEF += $(C_DEFS)

LOCAL_SRCS := $(subst $(CURDIR)/,, $(BOARD_SRCS))

LOCAL_INCLUDE += $(BOARD_INCLUDES)

LOCAL_FLAGS := $(BOARD_DEF) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
