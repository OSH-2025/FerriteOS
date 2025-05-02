LIBEVENT_VERSION = libevent-2.1.12

COMPONENTS_LIBEVENT_INCLUDE += \
    -I $(LITEOSTOPDIR)/components/utility/libevent/$(LIBEVENT_VERSION) \
    -I $(LITEOSTOPDIR)/components/utility/libevent/$(LIBEVENT_VERSION)/include \
    -I $(LITEOSTOPDIR)/components/utility/libevent/$(LIBEVENT_VERSION)/WIN32-Code/nmake

