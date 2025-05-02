include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/media/upnp/upnp.mk

MODULE_NAME := upnp_demo

LOCAL_SRCS :=
LOCAL_SRCS_y :=

LOCAL_SRCS_y += upnp_demo.c
LOCAL_SRCS = $(LOCAL_SRCS_y)

LOCAL_INCLUDE := $(COMPONENTS_UPNP_INCLUDE) 

LOCAL_EXT_FLAG := -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)