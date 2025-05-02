include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/media/tjpgdec/tjpgdec.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS := $(wildcard src/*.c)

LOCAL_INCLUDE := $(COMPONENTS_TJPGDEC_INCLUDE)

LOCAL_FLAGS := $(LOCAL_INCLUDE) -fstack-protector-strong

include $(MODULE)
