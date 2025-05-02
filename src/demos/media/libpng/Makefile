include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/media/libpng/libpng.mk

MODULE_NAME := png_demo

PNG_DEMO_INCLUDE := $(COMPONENTS_LIBPNG_INCLUDE)

LOCAL_SRCS := $(wildcard *.c)

LOCAL_INCLUDE := $(PNG_DEMO_INCLUDE)

LOCAL_EXT_FLAG := \
    -Wno-implicit-function-declaration \
    -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
