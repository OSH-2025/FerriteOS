include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/apriltag/apriltag.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/lib/libc/include \
    -I $(LITEOSTOPDIR)/components/utility/apriltag/port \
    -I $(LITEOSTOPDIR)/components/utility/apriltag/$(APRILTAG_VERSION) \
    -I $(LITEOSTOPDIR)/components/utility/apriltag/$(APRILTAG_VERSION)/common

LOCAL_SRCS_OUT := \
    $(wildcard $(APRILTAG_VERSION)/apriltag_pywrap.c)

LOCAL_SRCS += \
    $(wildcard $(APRILTAG_VERSION)/*.c) \
    $(wildcard port/*.c) \
    $(wildcard $(APRILTAG_VERSION)/common/*.c) \

LOCAL_SRCS := $(filter-out $(LOCAL_SRCS_OUT), $(LOCAL_SRCS))

LOCAL_EXT_FLAG := \
    -Wno-error \
    -fstack-protector-strong \
    -Wno-strict-prototypes \
    -Wno-float-equal \
    -Wno-maybe-uninitialized

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG)
include $(MODULE)
