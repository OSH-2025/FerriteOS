include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/iconv/iconv.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS := $(wildcard $(ICONV_VERSION)/source/lib/*.c) \
    $(wildcard $(ICONV_VERSION)/source/libcharset/lib/*.c)

LOCAL_INCLUDE += \
    -I $(LITEOSTOPDIR)/components/utility/iconv \
    -I $(LITEOSTOPDIR)/components/utility/iconv/$(ICONV_VERSION)/source \
    -I $(LITEOSTOPDIR)/components/utility/iconv/$(ICONV_VERSION)/source/libcharset/include \
    -I $(LITEOSTOPDIR)/components/utility/iconv/$(ICONV_VERSION)/source/libcharset/lib \
    -I $(LITEOSTOPDIR)/components/utility/iconv/$(ICONV_VERSION)/source/lib \
    -I $(LITEOSTOPDIR)/components/utility/iconv/$(ICONV_VERSION)/source/include \

LOCAL_CDEFS := -DLIBDIR -DLIBICONV_PLUG -DHAVE_WORKING_O_NOFOLLOW=0

LOCAL_EXT_FLAG := -Wno-unused -Wno-int-conversion -Wno-strict-prototypes \
    -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_CDEFS) $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
