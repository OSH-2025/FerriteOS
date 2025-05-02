include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/net/http_parser/http_parser.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS := $(HTTP_PARSER_VERSION)/http_parser.c

LOCAL_INCLUDE := -I $(LITEOSTOPDIR)/components/net/http_parser/$(HTTP_PARSER_VERSION)

LOCAL_EXT_FLAG := -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG)

include $(MODULE)
