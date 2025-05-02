include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/security/openssl/openssl.mk

MODULE_NAME := openssl_demo

LOCAL_SRCS := $(wildcard *.c)

LOCAL_INCLUDE := $(COMPONENTS_OPENSSL_INCLUDE)

LOCAL_INCLUDE += $(OPENSSL_DEMO_INCLUDE)

LOCAL_EXT_FLAG := -Wno-error -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)