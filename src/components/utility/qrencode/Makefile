include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/qrencode/qrencode.mk

MODULE_NAME := qrencode

QRENCODE_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/utility/qrencode/$(QRENCODE_VERSION)/qrencode

LOCAL_INCLUDE := $(QRENCODE_INCLUDE)

QRENCODE_SRCS := $(wildcard $(QRENCODE_VERSION)/qrencode/*.c)

LOCAL_SRCS = $(QRENCODE_SRCS)

LOCAL_EXT_FLAG := \
    -Wno-error=unknown-pragmas \
    -Wno-unknown-pragmas

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
