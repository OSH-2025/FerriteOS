include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/easyflash/easyflash.mk

MODULE_NAME := ef_demo

LOCAL_SRCS = $(wildcard ef_demo.c)

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/demos/utility/easyflash \
    -I $(LITEOSTOPDIR)/include

LOCAL_EXT_FLAG := \
    -fstack-protector-strong  -Wno-comment

LOCAL_INCLUDE += $(COMPONENTS_EASYFLASH_INCLUDE)

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG)

include $(MODULE)

