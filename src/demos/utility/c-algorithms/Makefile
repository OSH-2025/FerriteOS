include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/c-algorithms/c-algorithms.mk

MODULE_NAME := c_algorithms_demo

LOCAL_SRCS := $(wildcard *.c)

LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/components/fs/include
LOCAL_INCLUDE += $(COMPONENTS_C_ALGORITHMS_INCLUDE)

LOCAL_EXT_FLAG := -Wno-error -fstack-protector-strong
LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG)

include $(MODULE)
