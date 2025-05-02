include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(LITEOS_PLATFORM)

-include $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/config.mk

USER_SRC = \
    os_adapt/os_adapt.c \
    Drivers/CMSIS/Device/FM/FM33xx/Source/system_fm33lc0xx.c \
    Src/mf_config.c \
    Src/main.c \
    Src/tim.c \
    Src/usart.c \
    Src/user_init.c \
    Src/sys_init.c \
    Src/user_task.c
    C_SOURCES += $(USER_SRC)

HAL_DRIVER_INC = \
    -I $(LITEOSTOPDIR)/targets/FM33LC0XX_DEMO/Drivers/FM33LC0xx_LL_Driver/Inc
    BOARD_INCLUDES += $(HAL_DRIVER_INC)

INCLUDE_INC = \
    -I $(LITEOSTOPDIR)/include
    BOARD_INCLUDES += $(INCLUDE_INC)

BSP_INC = \
    -I $(LITEOSTOPDIR)/targets/bsp/common \
    -I $(LITEOSTOPDIR)/targets/bsp/include \
    -I $(LITEOSTOPDIR)/targets/bsp/hw/include \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/include \
    -I $(LITEOSTOPDIR)/kernel/extended/include
    BOARD_INCLUDES += $(BSP_INC)

USER_INC = \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Inc
    BOARD_INCLUDES += $(USER_INC)

C_DEFS += \
    -DFM33LC0XX \
    -DUSE_FULL_ASSERT \
    -D_DLIB_FILE_DESCRIPTOR \
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
