include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/iniparser/iniparser.mk

MODULE_NAME := iniparser_demo

LOCAL_SRCS := $(wildcard *.c)

LOCAL_INCLUDE += \
    -I $(LITEOSTOPDIR)/components/fs/include \
    -I $(LITEOSTOPDIR)/lib/libc/include \
    -I $(LITEOSTOPDIR)/lib/libsec/include \
    -I $(LITEOSTOPDIR)/demos/iniparser

LOCAL_INCLUDE += $(COMPONENTS_INIPARSER_INCLUDE)

LOCAL_EXT_FLAG := -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
