COMPONENTS_OTA_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/ota/upgrade \
    -I $(LITEOSTOPDIR)/components/ota/include \
    -I $(LITEOSTOPDIR)/components/ota/package \
    -I $(LITEOSTOPDIR)/components/ota/package/opt \
    -I $(LITEOSTOPDIR)/components/ota/sota \
    -I $(LITEOSTOPDIR)/components/ota/flag_operate \
    -I $(LITEOSTOPDIR)/components/ota/utility

ifneq ($(OS), Linux)
    LITEOS_BASELIB += -lota
endif
