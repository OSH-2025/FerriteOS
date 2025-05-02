include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/jsoncpp/jsoncpp.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS := $(wildcard $(JSONCPP_VERSION)/src/lib_json/*.cpp)

LOCAL_INCLUDE := $(COMPONENTS_JSONCPP_INCLUDE) \
    -I $(LITEOSTOPDIR)/components/utility/jsoncpp/$(JSONCPP_VERSION)/src/lib_json

LOCAL_EXT_FLAG := -w -fstack-protector-strong

LOCAL_CPPFLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
