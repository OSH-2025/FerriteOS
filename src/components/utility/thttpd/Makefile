include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/thttpd/thttpd.mk

MODULE_NAME := thttpd

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/include \
    -I $(LITEOSTOPDIR)/include/fs \
    -I $(THTTPD_VERSION)

LOCAL_INCLUDE += $(LWIP_INCLUDE)
LOCAL_INCLUDE += $(COMPONENTS_FS_INCLUDE)

THTTPD_SRCS := $(wildcard *.c)
THTTPD_SRCS += $(wildcard $(THTTPD_VERSION)/*.c)

LOCAL_SRCS := $(THTTPD_SRCS)

LOCAL_CDEFS := \
    -DHAVE__PROGNAME=1 -DHAVE_PATHS_H=1 -DHAVE_DIRENT_H=1 -DHAVE_STRERROR=1 -DHAVE_VSNPRINTF=1 \
    -DHAVE_GETADDRINFO=1 -DHAVE_GETNAMEINFO=1 -DHAVE_GAI_STRERROR=1 -DHAVE_ATOLL=1 -DHAVE_UNISTD_H=1 \
    -DHAVE_TM_GMTOFF=1 -DHAVE_INT64T=1 -DHAVE_SOCKLENT=1

LOCAL_EXT_FLAG := \
    -Wno-error -Wno-strict-prototypes -Wno-unused-variable -Wno-misleading-indentation \
    -Wno-return-type -Wno-pointer-sign -Wno-implicit-function-declaration \
    -Wno-unused-function -Wno-int-conversion -Wno-unused-but-set-variable \
    -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_CDEFS) $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
