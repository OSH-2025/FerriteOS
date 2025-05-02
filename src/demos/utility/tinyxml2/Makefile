include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/tinyxml2/tinyxml2.mk

MODULE_NAME := tinyxml2_demo

LOCAL_SRCS := $(wildcard *.cpp)

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/lib/libc/include \
    -I $(LITEOSTOPDIR)/include

LOCAL_INCLUDE += $(COMPONENTS_TINYXML2_INCLUDE) $(LITEOS_CFLAGS_INTERWORK)

LOCAL_EXT_FLAG := -w -Wno-error -fstack-protector-strong

LOCAL_CPPDEFS := -std=c++11 -D_GLIBCXX_HAS_GTHREADS

LOCAL_CPPFLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG) $(LOCAL_CPPDEFS)

include $(MODULE)

