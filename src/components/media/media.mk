COMPONENTS_MEDIA_INCLUDE :=

ifeq ($(LOSCFG_COMPONENTS_LIBPNG), y)
include $(LITEOSTOPDIR)/components/media/libpng/libpng.mk
COMPONENTS_MEDIA_INCLUDE += $(COMPONENTS_LIBPNG_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_OPUS), y)
include $(LITEOSTOPDIR)/components/media/opus/opus.mk
COMPONENTS_MEDIA_INCLUDE += $(COMPONENTS_OPUS_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_OPENEXIF), y)
include $(LITEOSTOPDIR)/components/media/openexif/openexif.mk
COMPONENTS_MEDIA_INCLUDE += $(COMPONENTS_OPENEXIF_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_PIXMAN), y)
include $(LITEOSTOPDIR)/components/media/pixman/pixman.mk
COMPONENTS_MEDIA_INCLUDE += $(COMPONENTS_PIXMAN_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_UPNP), y)
include $(LITEOSTOPDIR)/components/media/upnp/upnp.mk
COMPONENTS_INCLUDE += $(COMPONENTS_UPNP_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_TJPGDEC), y)
include $(LITEOSTOPDIR)/components/media/tjpgdec/tjpgdec.mk
COMPONENTS_INCLUDE += $(COMPONENTS_TJPGDEC_INCLUDE)
endif
