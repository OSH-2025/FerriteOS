include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

ifeq ($(LOSCFG_SHELL), y)
LOCAL_SRCS := $(wildcard src/base/*.c) \


ifeq ($(LOSCFG_SHELL_UART), y)
LOCAL_SRCS += src/base/porting/port_uart.c
endif

LOCAL_SRCS += $(wildcard src/magickey/*.c)
endif

ifeq ($(LOSCFG_SHELL_EXTENDED_CMDS), y)
LOCAL_SRCS += $(wildcard src/cmds/*.c)
endif

ifeq ($(LOSCFG_SHELL), y)
LOCAL_INCLUDE := \
    -I $(LITEOSTOPDIR)/include \
    -I $(LITEOSTOPDIR)/shell/full/include \
    -I $(LITEOSTOPDIR)/kernel/base/include \
    -I $(LITEOSTOPDIR)/components/fs/ramfs \
    -I $(LITEOSTOPDIR)/components/fs/fat
endif

ifndef LOSCFG_PLATFORM_OSAPPINIT
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/test/include
endif

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS) $(LITEOS_CFLAGS_INTERWORK)

include $(MODULE)
