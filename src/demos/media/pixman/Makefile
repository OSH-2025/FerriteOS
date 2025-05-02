include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/media/pixman/pixman.mk

MODULE_NAME := pixman_demo

PIXMAN_DEMO_INCLUDE := $(COMPONENTS_PIXMAN_INCLUDE)

LOCAL_SRCS := $(wildcard *.c)

LOCAL_INCLUDE := $(PIXMAN_DEMO_INCLUDE)

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS) -Wno-implicit-function-declaration -fstack-protector-strong

include $(MODULE)
