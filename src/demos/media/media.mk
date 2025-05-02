DEMOS_MEDIA_INCLUDE :=

ifeq ($(LOSCFG_DEMOS_LIBPNG), y)
include $(LITEOSTOPDIR)/demos/media/libpng/libpng.mk
DEMOS_MEDIA_INCLUDE += $(DEMOS_LIBPNG_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_OPUS), y)
include $(LITEOSTOPDIR)/demos/media/opus/opus.mk
DEMOS_MEDIA_INCLUDE += $(DEMOS_OPUS_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_OPENEXIF), y)
include $(LITEOSTOPDIR)/demos/media/openexif/openexif.mk
DEMOS_MEDIA_INCLUDE += $(DEMOS_OPENEXIF_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_PIXMAN), y)
include $(LITEOSTOPDIR)/demos/media/pixman/pixman.mk
DEMOS_MEDIA_INCLUDE += $(DEMOS_PIXMAN_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_UPNP), y)
include $(LITEOSTOPDIR)/demos/media/upnp/upnp.mk
DEMOS_INCLUDE += $(DEMOS_UPNP_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_TJPGDEC), y)
include $(LITEOSTOPDIR)/demos/media/tjpgdec/tjpgdec.mk
DEMOS_INCLUDE += $(DEMOS_TJPGDEC_INCLUDE)
endif
