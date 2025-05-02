include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := memtest

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/test/include \
    -I $(LITEOSTOPDIR)/test/sample/kernel/base/mem/memory \
    -I $(LITEOSTOPDIR)/test/sample/kernel/base/mem/membox \
    -I $(LITEOSTOPDIR)/kernel/extended/include

ifeq ($(LOSCFG_KERNEL_MEM_BESTFIT), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/kernel/base/mem/bestfit
endif

ifeq ($(LOSCFG_KERNEL_MEM_BESTFIT_LITTLE), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/kernel/base/mem/bestfit_little
endif

SRC_MODULES := memory membox
SRC_MODULES += memory/testcase membox/testcase

LOCAL_MODULES := $(SRC_MODULES)

LOCAL_SRCS := $(foreach dir,$(LOCAL_MODULES),$(wildcard $(dir)/*.c))
LOCAL_CHS := $(foreach dir,$(LOCAL_MODULES),$(wildcard $(dir)/*.h))

LOCAL_FLAGS :=  $(LOCAL_INCLUDE)  -Wno-error

include $(MODULE)