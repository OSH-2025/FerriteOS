include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/bidireference/bidireference.mk

MODULE_NAME := bidireference_demo

BIDIREFERENCE_SRC = $(wildcard *.c)

LOCAL_INCLUDE := $(COMPONENTS_BIDIREFERENCE_INCLUDE)
LOCAL_INCLUDE += \
    -I $(LITEOSTOPDIR)/demos/utility/bidireference \
    -I $(LITEOSTOPDIR)/components/fs/include

LOCAL_SRCS := $(BIDIREFERENCE_SRC)

LOCAL_EXT_FLAG := -Wno-error -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
