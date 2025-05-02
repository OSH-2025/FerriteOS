include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/c-algorithms/c-algorithms.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_INCLUDE += $(COMPONENTS_C_ALGORITHMS_INCLUDE)

LOCAL_SRCS = $(wildcard $(ALGO_VERSION)/src/*.c)

LOCAL_EXT_FLAG := -Wno-error -fstack-protector-strong
LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG)
include $(MODULE)
