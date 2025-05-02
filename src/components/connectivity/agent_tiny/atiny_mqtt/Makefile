include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/components.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS := $(wildcard *.c)

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/components/connectivity/agent_tiny/atiny_mqtt \
    -I $(LITEOSTOPDIR)/components/ota/flag_operate \
    -I $(LITEOSTOPDIR)/components/security/mbedtls/mbedtls-3.0.0/library \
    -I $(LITEOSTOPDIR)/include

LOCAL_INCLUDE += \
    $(COMPONENTS_INCLUDE)

LOCAL_EXT_FLAG := -Wno-unused-variable

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
