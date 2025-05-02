include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/net/los_iperf/iperf.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS := $(wildcard src/*.c) \

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/lib/libc/include

LOCAL_INCLUDE += $(LWIP_INCLUDE)

LOCAL_EXT_FLAG := \
    -Wno-implicit-function-declaration \
    -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
