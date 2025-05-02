COMPONENTS_LWM2M_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/connectivity/lwm2m/core \
    -I $(LITEOSTOPDIR)/components/connectivity/lwm2m/core/er-coap-13

include $(LITEOSTOPDIR)/components/connectivity/lwm2m/add_definitions.mk
COMPONENTS_LWM2M_INCLUDE += $(ADD_DEFS)

