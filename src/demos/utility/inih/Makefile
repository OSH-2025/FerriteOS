include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/inih/inih.mk

MODULE_NAME := inih_demo

LOCAL_SRCS_y := $(wildcard *.c)

LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/components/fs/include
LOCAL_INCLUDE += $(COMPONENTS_INIH_INCLUDE)
LOCAL_SRCS := $(LOCAL_SRCS_y)

LOCAL_EXT_FLAG := -Wno-error -fstack-protector-strong
LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG)

include $(MODULE)
