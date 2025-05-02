include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_SRCS := \
    $(wildcard $(LIBEVENT_VERSION)/*.c)

LOCAL_SRCS_OUT := \
    $(wildcard $(LIBEVENT_VERSION)/buffer_iocp.c) \
    $(wildcard $(LIBEVENT_VERSION)/event_iocp.c) \
    $(wildcard $(LIBEVENT_VERSION)/evthread_win32.c) \
    $(wildcard $(LIBEVENT_VERSION)/bufferevent_async.c)
	
LOCAL_SRCS := $(filter-out $(LOCAL_SRCS_OUT), $(LOCAL_SRCS))

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/lib/libc/include \
    -I $(LITEOSTOPDIR)//components/security/openssl/$(OPENSSL_VERSION)/include \
    -I $(LITEOSTOPDIR)/components/net/lwip/$(LWIP_VERSION)/src/include/lwip \
    -I $(LITEOSTOPDIR)/components/utility/libevent/$(LIBEVENT_VERSION) \
    -I $(LITEOSTOPDIR)/components/utility/libevent/$(LIBEVENT_VERSION)/compat \
    -I $(LITEOSTOPDIR)/components/utility/libevent/$(LIBEVENT_VERSION)/include \
    -I $(LITEOSTOPDIR)/components/utility/libevent/$(LIBEVENT_VERSION)/WIN32-Code/nmake

LOCAL_CDEFS := \
    -D EVENT__HAVE_UNISTD_H \
    -D EVENT__HAVE_SA_FAMILY_T \
    -D EVENT__HAVE_STRTOK_R \
    -D EVENT__HAVE_NETDB_H \
    -D EVENT__HAVE_SYS_SOCKET_H \
    -D EVENT__HAVE_STRSEP \
    -D EVENT__HAVE_GETTIMEOFDAY \
    -D EVENT__HAVE_SYS_TIME_H \
    -D EVENT__HAVE_SA_FAMILY_T \
    -D EVENT__HAVE_POLL \
    -D EVENT__HAVE_SELECT \
    -D EVENT__HAVE_SYS_RANDOM_H

LOCAL_EXT_FLAG := -Wno-unused-variable -Wno-unused-label

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_CDEFS) $(LOCAL_EXT_FLAG)

include $(MODULE)
