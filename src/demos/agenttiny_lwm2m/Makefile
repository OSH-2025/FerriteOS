include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/components.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS := \
    $(wildcard *.c)

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/demos/agenttiny_lwm2m \
    -I $(LITEOSTOPDIR)/include

LOCAL_INCLUDE += $(COMPONENTS_INCLUDE)

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
