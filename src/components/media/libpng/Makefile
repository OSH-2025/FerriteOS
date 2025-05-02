include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/media/libpng/libpng.mk

MODULE_NAME := png

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/media/libpng/$(LIBPNG_VERSION) \
    -I $(LITEOSTOPDIR)/lib/zlib/include

PNG_SRCS = $(wildcard $(LIBPNG_VERSION)/*.c) $(wildcard $(LIBPNG_VERSION)/arm/*.c)
LOCAL_SRCS = $(PNG_SRCS)

LOCAL_EXT_FLAG := \
    -Wno-error -Wno-float-equal \
    -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
