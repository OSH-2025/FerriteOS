include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/easyflash/easyflash.mk
include $(LITEOSTOPDIR)/components/utility/sfud/sfud.mk

MODULE_NAME := easyflash

LOCAL_SRCS := \
    $(wildcard $(EASYFLASH_VERSION)/easyflash/src/*.c)

LOCAL_INCLUDE := $(COMPONENTS_EASYFLASH_INCLUDE) $(COMPONENTS_SFUD_INCLUDE)

LOCAL_EXT_FLAG := -Wno-comment -Wno-int-conversion -Wno-switch

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG)

include $(MODULE)

