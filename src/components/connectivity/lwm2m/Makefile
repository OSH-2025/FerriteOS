include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/connectivity/connectivity.mk

MODULE_NAME := $(notdir $(CURDIR))

LWM2M_DIR := $(LITEOSTOPDIR)/components/connectivity/lwm2m

LWM2M_ADD_DEFS :=

LOCAL_SRCS := \
    $(wildcard core/*.c) \
    $(wildcard core/er-coap-13/*.c) \
    $(wildcard examples/shared/*.c)

LOCAL_INCLUDE := \
    -I $(LWM2M_DIR)/core \
    -I $(LWM2M_DIR)/examples/shared \
    -I $(LITEOSTOPDIR)/include

LWM2M_ADD_DEFS += -DLWM2M_BIG_ENDIAN

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS) $(LWM2M_ADD_DEFS) $(COMPONENTS_CONNECTIVITY_INCLUDE)

include $(MODULE)
