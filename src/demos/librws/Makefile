include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/net/librws/librws.mk

MODULE_NAME := librws_demo

LOCAL_SRCS := $(wildcard *.c)

LOCAL_INCLUDE := $(COMPONENTS_LIBRWS_INCLUDE)

LOCAL_EXT_FLAG := -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG)

include $(MODULE)
