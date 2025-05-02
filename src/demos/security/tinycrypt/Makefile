include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/security/tinycrypt/tinycrypt.mk

MODULE_NAME := tinycrypt_demo

LOCAL_SRCS := $(wildcard *.c)

LOCAL_INCLUDE := $(COMPONENTS_TINYCRYPT_INCLUDE)

LOCAL_INCLUDE += $(EMOS_TINYCRYPT_INCLUDE)

LOCAL_EXT_FLAG := -Wno-error

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
