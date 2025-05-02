include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/components.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS := \
    $(wildcard *.c)

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/connectivity/agent_tiny/atiny_lwm2m \
    -I $(LITEOSTOPDIR)/components/ota/include \
    -I $(LITEOSTOPDIR)/components/ota/flag_operate \
    -I $(LITEOSTOPDIR)/components/ota/package \
    -I $(LITEOSTOPDIR)/include

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS) $(COMPONENTS_INCLUDE)

include $(MODULE)

