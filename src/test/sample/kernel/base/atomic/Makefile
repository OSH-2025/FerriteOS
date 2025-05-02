include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := atomictest

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/kernel/include \
    -I $(LITEOSTOPDIR)/test/include \
    -I $(LITEOSTOPDIR)/test/sample/kernel/base/atomic

SRC_MODULES := .
SRC_MODULES += testcase
LOCAL_MODULES := $(SRC_MODULES)

LOCAL_SRCS := $(foreach dir,$(LOCAL_MODULES),$(wildcard $(dir)/*.c))
LOCAL_CHS := $(foreach dir,$(LOCAL_MODULES),$(wildcard $(dir)/*.h))

LOCAL_FLAGS :=  $(LOCAL_INCLUDE)  -Wno-error

include $(MODULE)