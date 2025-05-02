UPNP_VERSION = libupnp-1.6.22

COMPONENTS_UPNP_INCLUDE += \
    -I $(LITEOSTOPDIR)/components/media/upnp/$(UPNP_VERSION) \
    -I $(LITEOSTOPDIR)/components/media/upnp/$(UPNP_VERSION)/upnp/src/inc \
    -I $(LITEOSTOPDIR)/components/media/upnp/$(UPNP_VERSION)/upnp/inc \
    -I $(LITEOSTOPDIR)/components/media/upnp/$(UPNP_VERSION)/ixml/inc \
    -I $(LITEOSTOPDIR)/components/media/upnp/$(UPNP_VERSION)/ixml/src/inc \
    -I $(LITEOSTOPDIR)/components/media/upnp/$(UPNP_VERSION)/threadutil/inc \
    -I $(LITEOSTOPDIR)/components/media/upnp/$(UPNP_VERSION)/build/inc \
