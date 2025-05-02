include $(LITEOSTOPDIR)/config.mk
MODULE_NAME := fs_demo

LOCAL_SRCS :=
LOCAL_SRCS_y :=

LOCAL_SRCS_y += fs_common.c fs_demo.c
LOCAL_SRCS_$(LOSCFG_DEMOS_FS_FAT) += fatfs_demo.c
LOCAL_SRCS_$(LOSCFG_DEMOS_FS_SPIF) += spiffs_demo.c
LOCAL_SRCS_$(LOSCFG_DEMOS_FS_RAM) += ramfs_demo.c
LOCAL_SRCS_$(LOSCFG_DEMOS_FS_LITTLEFS) += littlefs_demo.c

LOCAL_INCLUDE += \
    -I $(LITEOSTOPDIR)/demos/fs \
    -I $(LITEOSTOPDIR)/include

LOCAL_SRCS = $(LOCAL_SRCS_y)

LOCAL_EXT_FLAG := -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE) $(LITEOS_CFLAGS_INTERWORK)

include $(MODULE)
