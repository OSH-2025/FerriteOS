include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/fastlz/fastlz.mk

MODULE_NAME := fastlz_demo

LOCAL_SRCS = $(wildcard fastlz_demo.c)

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/demos/utility/fastlz \
    -I $(LITEOSTOPDIR)/include

LOCAL_INCLUDE += $(COMPONENTS_FASTLZ_INCLUDE)

LOCAL_EXT_FLAG := -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
