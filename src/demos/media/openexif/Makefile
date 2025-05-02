include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/media/openexif/openexif.mk

MODULE_NAME := openexif_demo

LOCAL_SRCS := $(wildcard *.cpp)

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/include \
    $(COMPONENTS_INCLUDE)

LOCAL_EXT_FLAG := -w -Wno-error -fstack-protector-strong

LOCAL_CPPFLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
