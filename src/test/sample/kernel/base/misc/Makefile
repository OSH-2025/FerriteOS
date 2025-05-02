include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := misctest

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/test/include \
    -I $(LITEOSTOPDIR)/compat/posix/src \
    -I $(LITEOSTOPDIR)/test/sample/kernel/base/misc

SRC_MODULES := .
SRC_MODULES += testcase

LOCAL_MODULES := $(SRC_MODULES)

LOCAL_SRCS := $(foreach dir,$(LOCAL_MODULES),$(wildcard $(dir)/*.c))
LOCAL_CHS := $(foreach dir,$(LOCAL_MODULES),$(wildcard $(dir)/*.h))

LOCAL_FLAGS :=  $(LOCAL_INCLUDE)  -Wno-error

include $(MODULE)