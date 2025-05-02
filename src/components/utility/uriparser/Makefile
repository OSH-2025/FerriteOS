include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/uriparser/uriparser.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS := $(wildcard $(URIPARSER_VERSION)/src/*.c)

LOCAL_INCLUDE := -I $(LITEOSTOPDIR)/components/utility/uriparser/$(URIPARSER_VERSION)/include

LOCAL_EXT_FLAG := -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG)

include $(MODULE)
