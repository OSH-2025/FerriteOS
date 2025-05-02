include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/fs/fs.mk

MODULE_NAME     := $(notdir $(CURDIR))
LOCAL_SRCS_y    :=
LOCAL_DIRS_y    :=

# VFs
ifeq ($(LOSCFG_COMPONENTS_FS_VFS), y)
LOCAL_DIRS_y += vfs
LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/lib/libc/include
LOCAL_INCLUDE += $(LWIP_INCLUDE)
endif

# FatFs
ifeq ($(LOSCFG_COMPONENTS_FS_FATFS), y)
LOCAL_DIRS_y += fatfs
LOCAL_DIRS_y += fatfs/ff13b/source
LOCAL_DIRS_y += fatfs/ff13b/source/default
endif

# DevFs
ifeq ($(LOSCFG_COMPONENTS_FS_DEVFS), y)
LOCAL_DIRS_y += devfs
endif

# KiFs
ifeq ($(LOSCFG_COMPONENTS_FS_KIFS), y)
LOCAL_DIRS_y += kifs
endif

# RamFs
ifeq ($(LOSCFG_COMPONENTS_FS_RAMFS), y)
LOCAL_DIRS_y += ramfs
endif

# SpifFs
ifeq ($(LOSCFG_COMPONENTS_FS_SPIFFS), y)
LOCAL_DIRS_y += spiffs
LOCAL_DIRS_y += spiffs/spiffs_git/src
endif

# LittleFs
ifeq ($(LOSCFG_COMPONENTS_FS_LITTLEFS), y)
LOCAL_DIRS_y += littlefs
LOCAL_DIRS_y += littlefs/$(LITTLEFS_VERSION)
endif

LOCAL_SRCS_y += $(foreach dir, $(LOCAL_DIRS_y), $(wildcard $(dir)/*.c))
LOCAL_SRCS = $(LOCAL_SRCS_y)

LOCAL_INCLUDE += $(COMPONENTS_FS_INCLUDE)

LOCAL_EXT_FLAG := -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)

