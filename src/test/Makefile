include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(shell pwd))

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/kernel/base/include \
    -I $(LITEOSTOPDIR)/kernel/include \
    -I $(LITEOSTOPDIR)/test/include \

ifeq ($(LOSCFG_KERNEL_MEM_BESTFIT), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/kernel/base/mem/bestfit
endif

ifeq ($(LOSCFG_KERNEL_MEM_BESTFIT_LITTLE), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/kernel/base/mem/bestfit_little
endif

LOCAL_SRCS := $(wildcard src/ostest.c) \
              $(wildcard src/test_common.c) \
              $(wildcard src/icunit.c) \
              $(wildcard src/board/$(LITEOS_PLATFORM)/*.c)
LOCAL_FLAGS :=  $(LOCAL_INCLUDE) -Wno-error

include $(MODULE)