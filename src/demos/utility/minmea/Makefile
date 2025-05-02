include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/minmea/minmea.mk

MODULE_NAME := minmea_demo

LOCAL_SRCS := $(wildcard *.c)

LOCAL_INCLUDE := $(COMPONENTS_MINMEA_INCLUDE)

LOCAL_FLAGS := $(LOCAL_INCLUDE) -fstack-protector-strong

include $(MODULE)
