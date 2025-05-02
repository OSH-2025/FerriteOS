include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/json-c/json-c.mk

MODULE_NAME := json-c_demo

JSON_DEMO_SRCS = $(wildcard *.c)

JSON_DEMO_INCLUDE := \
    -I $(LITEOSTOPDIR)/lib/libc/include \
    -I $(LITEOSTOPDIR)/components/fs/include \
    -I $(LITEOSTOPDIR)/demos/utility/json-c

LOCAL_INCLUDE := $(JSON_DEMO_INCLUDE) $(COMPONENTS_JSON_INCLUDE) 

LOCAL_SRCS := $(subst $(CURDIR)/,, $(JSON_DEMO_SRCS))

LOCAL_EXT_FLAG := -Wno-error -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
