include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/media/upnp/upnp.mk

MODULE_NAME := upnp

LOCAL_SRCS = \
    $(wildcard $(UPNP_VERSION)/upnp/src/api/*.c)            \
    $(wildcard $(UPNP_VERSION)/threadutil/src/*.c)          \
    $(wildcard $(UPNP_VERSION)/ixml/src/*.c)                \
    $(wildcard $(UPNP_VERSION)/upnp/src/ssdp/*.c)           \
    $(wildcard $(UPNP_VERSION)/upnp/src/soap/*.c)           \
    $(wildcard $(UPNP_VERSION)/upnp/src/uuid/*.c)           \
    $(wildcard $(UPNP_VERSION)/upnp/src/gena/*.c)           \
    $(wildcard $(UPNP_VERSION)/upnp/src/genlib/service_table/*.c)    \
    $(wildcard $(UPNP_VERSION)/upnp/src/genlib/net/*.c)              \
    $(wildcard $(UPNP_VERSION)/upnp/src/genlib/net/uri/*.c)          \
    $(wildcard $(UPNP_VERSION)/upnp/src/genlib/client_table/*.c)     \
    $(wildcard $(UPNP_VERSION)/upnp/src/genlib/net/http/*.c)         \
    $(wildcard $(UPNP_VERSION)/upnp/src/genlib/miniserver/*.c)       \
    $(wildcard $(UPNP_VERSION)/upnp/src/genlib/util/*.c)

LOCAL_CDEFS = -DSQLITE_DEBUG=1

LOCAL_EXT_FLAG := -Wno-error -Wno-float-equal

LOCAL_FLAGS := $(LOCAL_CDEFS) $(LOCAL_EXT_FLAG) $(COMPONENTS_UPNP_INCLUDE)

include $(MODULE)
