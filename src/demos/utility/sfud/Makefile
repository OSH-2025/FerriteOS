include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/sfud/sfud.mk

MODULE_NAME := sfud_demo

LOCAL_SRCS = $(wildcard sfud_demo.c)

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/demos/utility/sfud \
    -I $(LITEOSTOPDIR)/include

LOCAL_EXT_FLAG := \
    -fstack-protector-strong

LOCAL_INCLUDE += $(COMPONENTS_SFUD_INCLUDE) $(LOCAL_EXT_FLAG)

LOCAL_FLAGS := $(LOCAL_INCLUDE)
include $(MODULE)
