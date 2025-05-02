include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/curl/curl.mk

MODULE_NAME := curl_demo

LOCAL_SRCS += curl_demo.c

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/demos/utility/curl \
    -I $(LITEOSTOPDIR)/include

LOCAL_INCLUDE += $(COMPONENTS_CURL_INCLUDE) $(COMPONENTS_FS_INCLUDE) $(LWIP_INCLUDE)

LOCAL_EXT_FLAG := \
    -Wno-error -Wno-Wunused-variable \
    -Wno-attribute-warning -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
