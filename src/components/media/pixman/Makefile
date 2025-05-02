include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/media/pixman/pixman.mk

MODULE_NAME := pixman

PIXMAN_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/media/pixman/$(PIXMAN_VERSION) \
    -I $(LITEOSTOPDIR)/components/media/pixman/$(PIXMAN_VERSION)/test \
    -I $(LITEOSTOPDIR)/components/media/pixman/$(PIXMAN_VERSION)/pixman

PIXMAN_INCLUDE += $(COMPONENTS_LIBPNG_INCLUDE)
LOCAL_INCLUDE := $(PIXMAN_INCLUDE)

PIXMAN_SRCS := $(wildcard $(PIXMAN_VERSION)/pixman/*.c)
PIXMAN_SRCS += $(wildcard $(PIXMAN_VERSION)/test/utils.c)
PIXMAN_SRCS_OUT := \
    $(PIXMAN_VERSION)/pixman/pixman-region.c \
    $(PIXMAN_VERSION)/pixman/pixman-sse2.c \
    $(PIXMAN_VERSION)/pixman/pixman-ssse3.c \
    $(PIXMAN_VERSION)/pixman/pixman-mmx.c \
    $(PIXMAN_VERSION)/pixman/pixman-vmx.c \
    $(PIXMAN_VERSION)/pixman/pixman-x86.c

PIXMAN_SRCS := $(filter-out $(PIXMAN_SRCS_OUT), $(PIXMAN_SRCS))

LOCAL_SRCS = $(PIXMAN_SRCS)

DEFS := \
    -DHAVE_CONFIG_H -D__arm__

LOCAL_EXT_FLAG := \
    -Wno-incompatible-pointer-types -Wno-strict-prototypes\
    -Wno-error=attributes -Wno-error=incompatible-pointer-types \
    -Wno-unknown-pragmas -Wno-attributes -Wno-unused-const-variable \
    -Wno-error=unused-local-typedefs -Wno-error=unused-const-variable \
    -Wno-error=unknown-pragmas -Wno-unused-local-typedefs -Wno-error=strict-prototypes \
    -Wno-float-equal -fstack-protector-strong

LOCAL_FLAGS := $(DEFS) $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
