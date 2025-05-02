LOSCFG_MQTT_DIR := $(LITEOSTOPDIR)/components/connectivity/mqtt

COMPONENTS_MQTT_INCLUDE:= \
    -I $(LOSCFG_MQTT_DIR)/MQTTClient-C/src \
    -I $(LOSCFG_MQTT_DIR)/MQTTClient-C/src/liteOS \
    -I $(LOSCFG_MQTT_DIR)/MQTTPacket/src
