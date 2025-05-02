include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/language/lua/lua.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_INCLUDE := -I $(LITEOSTOPDIR)/components/language/lua/$(LUA_VERSION)

LOCAL_SRCS := \
    $(wildcard $(LUA_VERSION)/onelua.c)

LOCAL_CDEFS := \
    -DEXIT_FAILURE=1 \
    -DEXIT_SUCCESS=0

LOCAL_EXT_FLAG := -Wno-unused-variable -Wno-unused -Wno-float-equal

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG) $(LOCAL_CDEFS)

include $(MODULE)
