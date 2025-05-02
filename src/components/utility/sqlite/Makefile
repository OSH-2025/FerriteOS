include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/sqlite/sqlite.mk

MODULE_NAME := $(notdir $(CURDIR))
LOCAL_SRCS_y :=

LOCAL_SRCS_y += $(wildcard $(SQLITE_VERSION)/sqlite3.c)

LOCAL_INCLUDE := \
    -include $(LITEOSTOPDIR)/components/utility/sqlite/$(SQLITE_VERSION)/sqlite3.h \
    -include $(LITEOSTOPDIR)/components/utility/sqlite/$(SQLITE_VERSION)/sqlite3ext.h

LOCAL_SRCS = $(LOCAL_SRCS_y)

SQLITE_DEFS = \
    -D SQLITE_DEBUG=1 \
    -D _REENTRANT=1 \
    -D SQLITE_THREADSAFE=0

LOCAL_CDEFS = \
    -D HAVE_GMTIME_R=1 \
    -D HAVE_INTTYPES_H=1 \
    -D HAVE_LOCALTIME_R=1 \
    -D HAVE_STDINT_H=1 \
    -D HAVE_STDLIB_H=1 \
    -D HAVE_STRINGS_H=1 \
    -D HAVE_STRING_H=1 \
    -D HAVE_UNISTD_H=1 \
    -D HAVE_USLEEP=1 \
    -D HAVE_UTIME=1 \
    -D LT_OBJDIR=\".libs/\" \
    -D PACKAGE_BUGREPORT=\"\" \
    -D PACKAGE_NAME=\"sqlite\" \
    -D PACKAGE_STRING=\"sqlite\ 3.35.3\" \
    -D PACKAGE_TARNAME=\"sqlite\" \
    -D PACKAGE_VERSION=\"3.35.3\" \
    -D STDC_HEADERS=1 \
    -D SQLITE_OMIT_LOAD_EXTENSION \
    -D SQLITE_4_BYTE_ALIGNED_MALLOC=1 \
    -D SQLITE_OMIT_AUTHORIZATION=1 \
    -D SQLITE_OMIT_WAL=1

LOCAL_EXT_FLAG := \
    -Wno-error \
    -Wno-unused-value \
    -Wno-unused-function \
    -Wno-float-equal \
    -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_CDEFS) $(SQLITE_DEFS) $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
