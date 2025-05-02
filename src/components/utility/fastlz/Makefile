include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS = $(wildcard $(FASTLZ_VERSION)/fastlz.c) fastlz_shellcmd.c

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/utility/fastlz/$(FASTLZ_VERSION) \
    -I $(LITEOSTOPDIR)/include

LOCAL_EXT_FLAG := -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
