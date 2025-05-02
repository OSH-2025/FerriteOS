include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/security/rhash/rhash.mk

MODULE_NAME := rhash_demo

LOCAL_SRCS := $(wildcard *.c)

LOCAL_INCLUDE := $(COMPONENTS_RHASH_INCLUDE)

LOCAL_EXT_FLAG := -Wno-error -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
