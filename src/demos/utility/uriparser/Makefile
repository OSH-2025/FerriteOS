include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/uriparser/uriparser.mk

MODULE_NAME := uriparser_demo

LOCAL_SRCS := $(wildcard *.c)

LOCAL_INCLUDE := $(COMPONENTS_URIPARSER_INCLUDE)

LOCAL_EXT_FLAG := -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG) 

include $(MODULE)
