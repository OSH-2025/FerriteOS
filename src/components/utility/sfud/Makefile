include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS := $(wildcard $(SFUD_VERSION)/sfud/src/*.c)

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/utility/sfud/$(SFUD_VERSION)/sfud/inc

LOCAL_EXT_FLAG := \
    -Wno-unused-function -Wno-unused-variable -Wno-incompatible-pointer-types \
    -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG)

include $(MODULE)

