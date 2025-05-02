include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/iniparser/iniparser.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/utility/iniparser/$(INI_VERSION)/src

LOCAL_SRCS = $(wildcard $(INI_VERSION)/src/*.c)

LOCAL_EXT_FLAG := -Wno-error -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
