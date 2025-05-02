include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS = $(wildcard $(REGEX_VERSION)/re.c) regex_shellcmd.c

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/utility/regex/$(REGEX_VERSION)

LOCAL_EXT_FLAG := -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
