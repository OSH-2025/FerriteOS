include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/tinyxml2/tinyxml2.mk

MODULE_NAME := tinyxml2

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/utility/tinyxml2/$(TINYXML2_VERSION) \
    -I $(LITEOSTOPDIR)/lib/libc/include \
    -I $(LITEOSTOPDIR)/include

LOCAL_SRCS := $(wildcard $(TINYXML2_VERSION)/tinyxml2.cpp)

LOCAL_CDEFS := -DNDEBUG -D__CLANG_ATOMICS

LOCAL_EXT_FLAG := -Wno-error -fstack-protector-strong

LOCAL_CPPFLAGS := $(LOCAL_CDEFS) $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
