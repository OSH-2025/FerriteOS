include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/freetype/freetype.mk

MODULE_NAME := freetype_demo

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/lib/libc/include \
    -I $(LITEOSTOPDIR)/include

LOCAL_INCLUDE += $(COMPONENTS_FREETYPE_INCLUDE) $(COMPONENTS_FS_INCLUDE)

LOCAL_SRCS := $(wildcard *.c)

LOCAL_EXT_FLAG := \
    -Wno-error -Wno-strict-prototypes \
    -Wno-unused-function -fstack-protector-strong

LOCAL_CFLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE) 

include $(MODULE)
