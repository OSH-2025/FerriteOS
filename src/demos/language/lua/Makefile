include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/language/lua/lua.mk

MODULE_NAME := lua_demo

LOCAL_SRCS := $(wildcard *.c)

LOCAL_INCLUDE := $(COMPONENTS_LUA_INCLUDE) \
    -I $(LITEOSTOPDIR)/components/fs/include

LOCAL_FLAGS := $(LOCAL_INCLUDE)

include $(MODULE)
