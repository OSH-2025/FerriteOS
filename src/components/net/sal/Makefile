include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/components.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS := $(wildcard *.c)

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/net/sal \
    -I $(LITEOSTOPDIR)/include

LOCAL_EXT_FLAG := -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE) $(COMPONENTS_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
