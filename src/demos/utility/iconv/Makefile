include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/iconv/iconv.mk

MODULE_NAME := iconv_demo

ICON_DEMO_SRCS = $(wildcard *.c)

LOCAL_INCLUDE := $(COMPONENTS_ICONV_INCLUDE)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/demos/iconv

LOCAL_SRCS := $(ICON_DEMO_SRCS)

LOCAL_EXT_FLAG := -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
