include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/sqlite/sqlite.mk

MODULE_NAME := sqlite_demo

LOCAL_SRCS += sqlite_demo.c

LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/demos/utility/sqlite \
    -I $(LITEOSTOPDIR)/include

LOCAL_INCLUDE += $(COMPONENTS_SQLITE_INCLUDE) $(COMPONENTS_FS_INCLUDE) \
    $(LITEOS_CFLAGS_INTERWORK)

LOCAL_EXT_FLAG := -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
