include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS = \
    $(wildcard *.c)

IFCONFIG_INC := \
    -I $(LITEOSTOPDIR)/include

LOCAL_INCLUDE := $(IFCONFIG_INC) $(LWIP_INCLUDE)

LOCAL_EXT_FLAG := -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
