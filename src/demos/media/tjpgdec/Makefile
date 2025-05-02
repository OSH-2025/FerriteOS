include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/media/tjpgdec/tjpgdec.mk

MODULE_NAME := tjpgdec_demo

LOCAL_SRCS := $(wildcard *.c)

LOCAL_INCLUDE := $(COMPONENTS_TJPGDEC_INCLUDE)

LOCAL_FLAGS := $(LOCAL_INCLUDE) -fstack-protector-strong

include $(MODULE)
