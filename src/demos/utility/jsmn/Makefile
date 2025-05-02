include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/jsmn/jsmn.mk

MODULE_NAME := jsmn_demo

LOCAL_SRCS = $(wildcard *.c)

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/lib/libc/include \
    -I $(LITEOSTOPDIR)/include

LOCAL_INCLUDE := $(LOCAL_INCLUDE) $(COMPONENTS_JSMN_INCLUDE) 

LOCAL_EXT_FLAG := -Wno-error -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
