include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(LOSCFG_ARCH_CPU)

LOCAL_SRCS := $(wildcard src/*.c) $(wildcard src/*.S)

ifneq ($(LOSCFG_XTENSA_WINDOWSPILL), y)
LOCAL_SRCS := $(filter-out src/los_window.S, $(LOCAL_SRCS))
endif

LOCAL_INCLUDE := -I $(LITEOSTOPDIR)/arch/xtensa/lx/include

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

ifeq ($(LOSCFG_GDB), y)
LOCAL_FLAGS += $(AS_OBJS_LIBC_FLAGS)
endif

include $(MODULE)
