include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/jsoncpp/jsoncpp.mk

MODULE_NAME := jsoncpp_demo

LOCAL_SRCS := $(wildcard *.cpp)

LOCAL_INCLUDE := $(COMPONENTS_JSONCPP_INCLUDE)

LOCAL_CPPDEFS := -std=c++11

LOCAL_EXT_FLAG := -w -fstack-protector-strong

LOCAL_CPPFLAGS := $(LOCAL_CPPDEFS) $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
