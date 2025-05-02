include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS = \
    $(wildcard *.c)

PING_INC := \
    -I $(LITEOSTOPDIR)/include

LOCAL_INCLUDE := $(PING_INC) $(LWIP_INCLUDE)

LOCAL_EXT_FLAG := -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
