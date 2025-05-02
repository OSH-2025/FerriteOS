include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/security/rhash/rhash.mk

MODULE_NAME := rhash

SRCS := $(wildcard $(RHASH_VERSION)/*.c) \
   $(wildcard $(RHASH_VERSION)/librhash/*.c)

OUT_SRC := $(RHASH_VERSION)/librhash/test_hashes.c \
    $(RHASH_VERSION)/win_utils.c \
    $(RHASH_VERSION)/rhash_main.c

LOCAL_SRCS := $(filter-out $(OUT_SRC), $(SRCS))

LOCAL_INCLUDE := $(COMPONENTS_RHASH_INCLUDE) \
    -I $(LITEOSTOPDIR)/components/security/rhash/$(RHASH_VERSION)/librhash

LOCAL_EXT_FLAG := -fstack-protector-strong -Wno-float-equal -DRHASH_XVERSION=142

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG)

include $(MODULE)
