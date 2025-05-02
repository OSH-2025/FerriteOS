include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/components.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS := nb_demo.c

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/include

LOCAL_INCLUDE += $(COMPONENTS_INCLUDE)

LOCAL_FLAGS := $(LOCAL_INCLUDE)

include $(MODULE)
