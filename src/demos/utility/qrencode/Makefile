include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/qrencode/qrencode.mk

MODULE_NAME := qrencode_demo

QRENCODE_DEMO_INCLUDE := $(COMPONENTS_QRENCODE_INCLUDE)

LOCAL_SRCS := $(wildcard *.c)

LOCAL_INCLUDE := $(QRENCODE_DEMO_INCLUDE)

LOCAL_FLAGS :=  $(LOCAL_INCLUDE) -Wno-implicit-function-declaration

include $(MODULE)
