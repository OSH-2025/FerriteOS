COMPONENTS_FS_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/fs/include

ifeq ($(LOSCFG_COMPONENTS_FS_FATFS), y)
COMPONENTS_FS_INCLUDE += \
    -I $(LITEOSTOPDIR)/components/fs/fatfs/ff13b/source \
    -I $(LITEOSTOPDIR)/components/fs/fatfs/ff13b/source/default
endif

ifeq ($(LOSCFG_COMPONENTS_FS_SPIFFS), y)
COMPONENTS_FS_INCLUDE += \
    -I $(LITEOSTOPDIR)/components/fs/spiffs/spiffs_git/src \
    -I $(LITEOSTOPDIR)/components/fs/spiffs/spiffs_git/src/default
endif

ifeq ($(LOSCFG_COMPONENTS_FS_LITTLEFS), y)
LITTLEFS_VERSION := littlefs-2.3.0
COMPONENTS_FS_INCLUDE += -DLFS_THREADSAFE
COMPONENTS_FS_INCLUDE += \
    -I $(LITEOSTOPDIR)/components/fs/littlefs/$(LITTLEFS_VERSION)
endif
