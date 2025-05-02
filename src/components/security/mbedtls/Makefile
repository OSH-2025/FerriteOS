include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/security/mbedtls/mbedtls.mk
include $(LITEOSTOPDIR)/components/net/net.mk

MODULE_NAME := mbedtls
LOCAL_DIRS_y :=
LOCAL_CDEFS :=

LOCAL_DIRS_y += mbedtls_port
LOCAL_DIRS_y += $(MBEDTLS_VERSION)/library

LOCAL_INCLUDE += \
    -I $(LITEOSTOPDIR)/include \
    -I mbedtls_port \
    -I $(MBEDTLS_VERSION)/include \
    -I $(MBEDTLS_VERSION)/include/mbedtls

LOCAL_INCLUDE += $(COMPONENTS_NET_INCLUDE)

LOCAL_CDEFS += -DMBEDTLS_CONFIG_FILE=\"los_mbedtls_config.h\"

LOCAL_SRCS += $(foreach dir, $(LOCAL_DIRS_y), $(wildcard $(dir)/*.c))

LOCAL_FLAGS := $(LOCAL_CDEFS) $(LOCAL_INCLUDE)

include $(MODULE)