include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/net/net.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS :=
LOCAL_INCLUDE :=
NET_DIR := $(LITEOSTOPDIR)/components/net/net_at

LOCAL_SRCS += $(wildcard at_device/*.c)

LOCAL_SRCS += $(wildcard at_frame/*.c)

ifeq ($(LOSCFG_COMPONENTS_NET_AT_ESP8266), y)
LOCAL_SRCS += $(wildcard at_device/wifi_esp8266/*.c)
LOCAL_INCLUDE += -I $(NET_AT_DIR)/at_device/wifi_esp8266
endif

ifeq ($(LOSCFG_COMPONENTS_NET_AT_BG36), y)
LOCAL_SRCS += $(wildcard at_device/emtc_bg36/*.c)
LOCAL_INCLUDE += -I $(NET_AT_DIR)/at_device/emtc_bg36
endif

ifeq ($(LOSCFG_COMPONENTS_NET_AT_SIM900A), y)
LOCAL_SRCS += $(wildcard at_device/gprs_sim900a/*.c)
LOCAL_INCLUDE += -I $(NET_AT_DIR)/at_device/gprs_sim900a
endif

ifeq ($(LOSCFG_COMPONENTS_NET_AT_BC95), y)
LOCAL_SRCS += $(wildcard at_device/nb_bc95/*.c)
LOCAL_INCLUDE += -I $(NET_AT_DIR)/at_device/nb_bc95
endif

LOCAL_INCLUDE += \
    -I $(NET_AT_DIR)/at_device \
    -I $(NET_AT_DIR)/at_frame

LOCAL_INCLUDE += $(COMPONENTS_NET_INCLUDE)

LOCAL_EXT_FLAG := -Wno-unused-function
LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS) 

include $(MODULE)
