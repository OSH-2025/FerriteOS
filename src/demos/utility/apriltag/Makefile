include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/apriltag/apriltag.mk

MODULE_NAME := apriltag_demo

LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/lib/libc/include
LOCAL_INCLUDE += $(COMPONENTS_APRILTAG_INCLUDE)

LOCAL_SRCS := $(wildcard *.c)

LOCAL_EXT_FLAG := \
    -Wno-error \
    -Wno-strict-prototypes \
    -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG)

include $(MODULE)
