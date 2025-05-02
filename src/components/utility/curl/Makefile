include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/net/lwip/lwip.mk
include $(LITEOSTOPDIR)/components/utility/curl/curl.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS_y :=
LOCAL_DIRS_y :=

LOCAL_DIRS_y += $(CURL_VERSION)/lib
LOCAL_DIRS_y += $(CURL_VERSION)/lib/vauth
LOCAL_DIRS_y += $(CURL_VERSION)/lib/vtls
LOCAL_DIRS_y += $(CURL_VERSION)/src

LOCAL_INCLUDE := $(LWIP_INCLUDE)
LOCAL_INCLUDE += \
    -I $(LITEOSTOPDIR)/components/utility/curl/$(CURL_VERSION)/lib \
    -I $(LITEOSTOPDIR)/components/utility/curl/$(CURL_VERSION)/include \
    -I $(LITEOSTOPDIR)/components/utility/curl/$(CURL_VERSION)/src

LOCAL_SRCS_y += $(foreach dir, $(LOCAL_DIRS_y), $(wildcard $(dir)/*.c))
LOCAL_SRCS   = $(LOCAL_SRCS_y)

LOCAL_EXT_FLAG := \
    -Wno-error -Wno-implicit-function-declaration \
    -Wno-unused-variable -fstack-protector-strong

LOCAL_CDEFS := \
    -D HAVE_CONFIG_H \
    -D BUILDING_LIBCURL

ifeq ($(CURL_OPENSSL), y)
CURL_DEFS += -DUSE_OPENSSL
LOCAL_INCLUDE += $(COMPONENTS_OPENSSL_INCLUDE)
endif

LOCAL_FLAGS := $(LOCAL_CDEFS) $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
