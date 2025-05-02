include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/components.mk

MODULE_NAME := $(notdir $(CURDIR))

LOSCFG_MQTT_DIR := $(LITEOSTOPDIR)/components/connectivity/mqtt

LOCAL_SRCS := \
    $(wildcard MQTTClient-C/src/*.c) \
    $(wildcard MQTTClient-C/src/liteOS/*.c) \
    $(wildcard MQTTPacket/src/*.c)

LOCAL_INCLUDE := \
    -I $(LOSCFG_MQTT_DIR)/MQTTClient-C/src \
    -I $(LOSCFG_MQTT_DIR)/MQTTClient-C/src/liteOS \
    -I $(LOSCFG_MQTT_DIR)/MQTTPacket/src \
    -I $(LITEOSTOPDIR)/include

LOCAL_INCLUDE += $(COMPONENTS_INCLUDE)

LOCAL_FLAGS :=  $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE)
