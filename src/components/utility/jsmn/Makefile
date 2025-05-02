include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/jsmn/jsmn.mk

MODULE_NAME := jsmn

LOCAL_SRCS := $(wildcard example/*.c)

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/lib/libc/include \
    -I $(LITEOSTOPDIR)/include

LOCAL_INCLUDE += $(COMPONENTS_JSMN_INCLUDE)

LOCAL_FLAGS := $(LOCAL_INCLUDE)

include $(MODULE)
