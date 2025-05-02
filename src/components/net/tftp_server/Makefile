include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/net/tftp_server/tftp_server.mk

MODULE_NAME := tftp_server

LOCAL_INCLUDE := $(COMPONENTS_TFTP_SERVER_INCLUDE)

LOCAL_INCLUDE += $(LWIP_INCLUDE) $(COMPONENTS_FS_INCLUDE)

LOCAL_SRCS := $(wildcard *.c)

LOCAL_EXT_FLAG := -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
