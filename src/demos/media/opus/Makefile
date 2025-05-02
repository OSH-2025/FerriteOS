include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/media/opus/opus.mk

MODULE_NAME := opus_demo

OPUS_INC += $(COMPONENTS_OPUS_INCLUDE)

LOCAL_INCLUDE := $(OPUS_INC)

LOCAL_SRCS := $(wildcard *.c)

LOCAL_EXT_FLAG := -Wno-error -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
