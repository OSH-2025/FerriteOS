include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/language/jerryscript/jerryscript.mk

MODULE_NAME := jerryscript_demo

LOCAL_SRCS := $(wildcard jerryscript_demo.c)

LOCAL_INCLUDE := \
    $(COMPONENTS_JERRYSCRIPT_INCLUDE) \
    -I $(LITEOSTOPDIR)/demos/language/jerryscript

LOCAL_EXT_FLAG := -fstack-protector-strong
LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG)

include $(MODULE)

