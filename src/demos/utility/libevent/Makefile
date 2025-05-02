include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/libevent/libevent.mk

MODULE_NAME := libevent_demo

LOCAL_SRCS = $(wildcard *.c)

LOCAL_INCLUDE := $(LOCAL_INCLUDE) $(COMPONENTS_LIBEVENT_INCLUDE) 

LOCAL_EXT_FLAG := -Wno-error -Wno-pointer-sign -fstack-protector-strong

LOCAL_CDEFS := -D EVENT__HAVE_SA_FAMILY_T

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE) $(LOCAL_CDEFS)

include $(MODULE)
