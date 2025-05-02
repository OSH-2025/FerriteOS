include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(LOSCFG_ARCH_CPU)

LOCAL_SRCS_y := $(wildcard src/*.c) $(wildcard src/*.S)

LOCAL_INCLUDE := \
        -I $(LITEOSTOPDIR)/kernel/extended/include \
        -I $(LITEOSTOPDIR)/kernel/extended/dynload/include

ifneq ($(LOSCFG_FPB_ENABLE), y)
LOCAL_SRCS_y := $(filter-out src/fpb.c, $(LOCAL_SRCS_y))
endif

ifneq ($(LOSCFG_KERNEL_DYNLOAD), y)
LOCAL_SRCS_y := $(filter-out src/dynload.c, $(LOCAL_SRCS_y))
endif

ifneq ($(LOSCFG_APC_ENABLE), y)
LOCAL_SRCS_y := $(filter-out src/pmp.c, $(LOCAL_SRCS_y))
endif

LOCAL_SRCS = $(LOCAL_SRCS_y)
LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
