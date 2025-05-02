include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS = $(wildcard $(PDFGEN_VERSION)/pdfgen.c)

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/utility/pdfgen/$(PDFGEN_VERSION)

LOCAL_FLAGS := $(LOCAL_INCLUDE)

include $(MODULE)
