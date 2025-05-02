include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/inih/inih.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/utility/inih/$(INIH_VERSION)/

LOCAL_SRCS = $(wildcard $(INIH_VERSION)/*.c)

LOCAL_EXT_FLAG := -Wno-error -fstack-protector-strong
LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG)
include $(MODULE)
