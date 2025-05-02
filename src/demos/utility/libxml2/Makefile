include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/libxml2/libxml2.mk

MODULE_NAME := xml2_demo

XML2_DEMO_INCLUDE := $(COMPONENTS_LIBXML2_INCLUDE)

LOCAL_SRCS := $(wildcard *.c)

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/include

LOCAL_INCLUDE += $(XML2_DEMO_INCLUDE) $(COMPONENTS_FS_INCLUDE)

LOCAL_EXT_FLAG := -Wno-error -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)

