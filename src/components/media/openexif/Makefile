include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/media/openexif/openexif.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/include/fs \
    -I $(LITEOSTOPDIR)/components/media/openexif/$(OPENEXIF_VERSION)/src \
    -I $(LITEOSTOPDIR)/components/media/openexif/$(OPENEXIF_VERSION)/src/OpenExifJpeg

LOCAL_SRCS = \
    $(wildcard $(OPENEXIF_VERSION)/src/*.cpp) \
    $(wildcard $(OPENEXIF_VERSION)/src/OpenExifJpeg/*.cpp)

LOCAL_CDEFS = -DOPENEXIF_NO_IJG

LOCAL_EXT_FLAG := \
    -Wno-float-equal -Wno-maybe-uninitialized \
    -Wno-strict-aliasing -Wno-unused-variable \
    -fstack-protector-strong

LOCAL_CPPFLAGS := $(LOCAL_CDEFS) $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
