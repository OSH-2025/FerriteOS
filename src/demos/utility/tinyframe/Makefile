include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/tinyframe/tinyframe.mk

MODULE_NAME := tinyframe_demo


LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/lib/libc/include
LOCAL_INCLUDE += $(COMPONENTS_TINYFRAME_INCLUDE)

LOCAL_SRCS := $(wildcard *.c)

LOCAL_EXT_FLAG := -Wno-error -fstack-protector-strong
LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG)

include $(MODULE)
