include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/security/tinycrypt/tinycrypt.mk

MODULE_NAME := tinycrypt

LOCAL_INCLUDE := \
    -I $(TINYCRYPT_VERSION)/lib/include

LOCAL_SRCS := \
    $(wildcard $(TINYCRYPT_VERSION)/lib/source/*.c)

LOCAL_EXT_FLAG := -Wno-error

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
