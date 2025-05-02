DEMOS_INCLUDE :=

ifeq ($(LOSCFG_DEMOS_FS), y)
include $(LITEOSTOPDIR)/demos/fs/fs.mk
DEMOS_INCLUDE += $(DEMOS_FS_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_GUI), y)
include $(LITEOSTOPDIR)/demos/gui/gui.mk
DEMOS_INCLUDE += $(DEMOS_GUI_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_SENSORHUB), y)
include $(LITEOSTOPDIR)/demos/sensorhub/sensorhub.mk
DEMOS_INCLUDE += $(DEMOS_SENSORHUB_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_KERNEL), y)
include $(LITEOSTOPDIR)/demos/kernel/kernel.mk
DEMOS_INCLUDE += $(DEMOS_KERNEL_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_AGENT_TINY_MQTT), y)
include $(LITEOSTOPDIR)/demos/agenttiny_mqtt/agenttiny_mqtt.mk
DEMOS_INCLUDE += $(DEMOS_AGENTTINY_MQTT_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_AGENT_TINY_LWM2M), y)
include $(LITEOSTOPDIR)/demos/agenttiny_lwm2m/agenttiny_lwm2m.mk
DEMOS_INCLUDE += $(DEMOS_AGENTTINY_LWM2M_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_DTLS_SERVER), y)
include $(LITEOSTOPDIR)/demos/dtls_server/dtls_server.mk
DEMOS_INCLUDE += $(DEMOS_DTLS_SERVER_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_NBIOT_WITHOUT_ATINY), y)
include $(LITEOSTOPDIR)/demos/nbiot_without_atiny/nbiot.mk
DEMOS_INCLUDE += $(DEMOS_NBIOT_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_LMS), y)
include $(LITEOSTOPDIR)/demos/lms/lms.mk
DEMOS_INCLUDE += $(DEMOS_LMS_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_TRACE), y)
include $(LITEOSTOPDIR)/demos/trace/trace.mk
DEMOS_INCLUDE += $(DEMOS_TRACE_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_IPV6_CLIENT), y)
include $(LITEOSTOPDIR)/demos/ipv6_client/ipv6_client.mk
DEMOS_INCLUDE += $(DEMOS_IPV6_CLIENT_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_AI), y)
include $(LITEOSTOPDIR)/demos/ai/ai.mk
DEMOS_INCLUDE += $(DEMOS_AI_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_LIBRWS), y)
include $(LITEOSTOPDIR)/demos/librws/librws.mk
DEMOS_INCLUDE += $(DEMOS_LIBRWS_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_HTTP_CLIENT), y)
include $(LITEOSTOPDIR)/demos/http_client/http_client.mk
DEMOS_INCLUDE += $(DEMOS_HTTP_CLIENT_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_AGRICULTURE), y)
include $(LITEOSTOPDIR)/demos/agriculture/agriculture.mk
DEMOS_INCLUDE += $(DEMOS_AGRICULTURE_INCLUDE)
endif

include $(LITEOSTOPDIR)/demos/utility/utility.mk
DEMOS_INCLUDE += $(DEMOS_UTILITY_INCLUDE)

include $(LITEOSTOPDIR)/demos/media/media.mk
DEMOS_INCLUDE += $(DEMOS_MEDIA_INCLUDE)

include $(LITEOSTOPDIR)/demos/language/language.mk
DEMOS_INCLUDE += $(DEMOS_LANGUAGE_INCLUDE)

include $(LITEOSTOPDIR)/demos/drivers/drivers.mk
DEMOS_INCLUDE += $(DEMOS_DRIVERS_INCLUDE)

include $(LITEOSTOPDIR)/demos/security/security.mk
DEMOS_INCLUDE += $(DEMOS_SECURITY_INCLUDE)

ifeq ($(LOSCFG_DEMOS_PERF), y)
include $(LITEOSTOPDIR)/demos/perf/perf.mk
DEMOS_INCLUDE += $(DEMOS_PERF_INCLUDE)
endif
