include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/minmea/minmea.mk

MODULE_NAME := minmea

LOCAL_SRCS := $(MINMEA_VERSION)/minmea.c

LOCAL_INCLUDE := $(COMPONENTS_MINMEA_INCLUDE)

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE) -fstack-protector-strong

include $(MODULE)
