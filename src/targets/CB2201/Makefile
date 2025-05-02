include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(LITEOS_PLATFORM)

LOCAL_SRCS := $(wildcard Src/*.c)

ifeq ($(LOSCFG_PLATFORM_ADAPT), y)
LOCAL_SRCS += $(wildcard os_adapt/*.c)
endif

ifndef LOSCFG_PLATFORM_OSAPPINIT
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/test/include
endif

ALL_ASSRCS := $(wildcard *.S)
ASSRCS := $(ALL_ASSRCS)

# C defines
C_DEFS += \
    -DCK802 \
    -D__LITEOS__

LOCAL_SRCS += $(ASSRCS)

LOCAL_INCLUDE += \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/Inc \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/include \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/include/hisoc \
    -I $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/include/asm \
    -I $(LITEOSTOPDIR)/targets/bsp/drivers/$(HAL_DRIVER_TYPE)/Inc \
    -I $(LITEOSTOPDIR)/include

BOARD_DEF += $(C_DEFS)

LOCAL_FLAGS := $(BOARD_DEF) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
