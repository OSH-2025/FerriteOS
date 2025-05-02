# Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.

ifneq ($(LOSCFG_COMPILER_THIRDPARTY_SUPPORT), y)
    # If thirdparty compiler is not enabled, then empty $(CROSS_COMPILE)
    # and use system chosen compiler.
    # Attention: this override only affects the export environment. While
    # 'make CROSS_COMPILE=' style's $(CROSS_COMPILE) cannot be overrided.
    # In this case thirdparty support is mannually enabled which not controlled
    # by this config $(LOSCFG_COMPILER_THIRDPARTY_SUPPORT)
    CROSS_COMPILE :=
endif

ifeq ($(CROSS_COMPILE),)
    ifeq ($(LOSCFG_COMPILER_HIMIX_32), y)
        CROSS_COMPILE := arm-himix410-linux-
        COMPILE_ALIAS := arm-linux-musleabi
    else ifeq ($(LOSCFG_COMPILER_HIMIX210_64), y)
        CROSS_COMPILE := aarch64-himix210-linux-
        COMPILE_ALIAS := aarch64-linux-musl
    else ifeq ($(LOSCFG_COMPILER_HCC_64), y)
        CROSS_COMPILE := aarch64-linux-gnu-
    else ifeq ($(LOSCFG_COMPILER_GCC_64), y)
        CROSS_COMPILE := aarch64-none-elf-
    else ifeq ($(LOSCFG_COMPILER_ARM_NONE_EABI), y)
        CROSS_COMPILE := arm-none-eabi-
    else ifeq ($(LOSCFG_COMPILER_RISCV), y)
        CROSS_COMPILE := riscv32-linux-musl-
    else ifeq ($(LOSCFG_COMPILER_RISCV_UNKNOWN), y)
        CROSS_COMPILE := riscv32-unknown-elf-
    else ifeq ($(LOSCFG_COMPILER_CSKYV2), y)
        CROSS_COMPILE := csky-elfabiv2-
    else ifeq ($(LOSCFG_COMPILER_RISCV_NULEI), y)
        CROSS_COMPILE := riscv-nuclei-elf-
    else ifeq ($(LOSCFG_COMPILER_RISCV64_UNKNOWN), y)
        CROSS_COMPILE := riscv64-unknown-elf-
    else ifeq ($(LOSCFG_COMPILER_XTENSA_LX6), y)
        CROSS_COMPILE := xtensa-esp32-elf-
    else ifeq ($(LOSCFG_COMPILER_XTENSA_L106), y)
        CROSS_COMPILE := xtensa-lx106-elf-
    endif
endif

COMPILE_NAME   = $(CROSS_COMPILE:%-=%)
COMPILE_ALIAS ?= $(COMPILE_NAME)
VERSION_NUM    = $(shell $(CC) -dumpversion)

CC       = $(CROSS_COMPILE)gcc
GPP      = $(CROSS_COMPILE)g++
AS       = $(CROSS_COMPILE)as
AR       = $(CROSS_COMPILE)ar
LD       = $(CROSS_COMPILE)ld
OBJCOPY  = $(CROSS_COMPILE)objcopy
OBJDUMP  = $(CROSS_COMPILE)objdump
SIZE     = $(CROSS_COMPILE)size
NM       = $(CROSS_COMPILE)nm

# Check if input compiler is availible
ifeq ($(OS), Linux)
    ifeq ($(shell which $(CC)),)
        $(error compiler $(COMPILE_NAME) is not in the ENV)
    endif
    LITEOS_COMPILER_PATH := $(shell $(LITEOSTOPDIR)/build/scripts/get_compiler_path.sh $(CC))
else
    ifeq ($(shell where $(CC)),)
        $(error compiler $(COMPILE_NAME) is not in the ENV)
    endif
    LITEOS_COMPILER_EXE_PATH := $(shell where $(CC))
    LITEOS_COMPILER_PATH := $(subst \bin\$(CC).exe,, $(LITEOS_COMPILER_EXE_PATH))
endif

ifeq ($(LOSCFG_COMPILER_ARM_NONE_EABI), y)
    ifeq ($(shell if [ $(shell echo $(VERSION_NUM) | tr -d ".") -lt 921 ]; then echo y; fi), y)
        $(error The compiler version is too early. You are advised to use a version later than gcc-arm-none-eabi-9-2019-q4-major)
    endif
    ifeq ($(shell if [ $(shell echo $(VERSION_NUM) | tr -d ".") -gt 1021 ]; then echo y; fi), y)
        LD = $(CC)
    endif
endif

# Generally 32 bit and 64 bit compilers has different libgcc paths.
# FOR some history resaons.
# 32 bit : $(LITEOS_GCCLIB) can be defined as CPU level lib path,
#          if not define, use the common libs instead.
# 64 bit : Default use lib64 path, if not exist, use lib instead.
GCC_USE_CPU_OPT := y
GCC_GCCLIB_PATH :=
GCC_GXXLIB_PATH :=

GCCLIB_PATH_32 = $(LITEOS_COMPILER_PATH)/lib/gcc/$(COMPILE_NAME)/$(VERSION_NUM)
GCCLIB_PATH_64 = $(LITEOS_COMPILER_PATH)/lib64/gcc/$(COMPILE_NAME)/$(VERSION_NUM)

CXXLIB_PATH_32 = $(LITEOS_COMPILER_PATH)/$(COMPILE_NAME)/lib
CXXLIB_PATH_64 = $(LITEOS_COMPILER_PATH)/$(COMPILE_NAME)/lib64

ifeq ($(findstring 64, $(CROSS_COMPILE)),)
    GCC_GCCLIB_PATH := $(GCCLIB_PATH_32)
    GCC_GXXLIB_PATH := $(CXXLIB_PATH_32)
else
    ifneq ($(wildcard $(GCCLIB_PATH_64)),)
        GCC_GCCLIB_PATH := $(GCCLIB_PATH_64)
    else
        GCC_GCCLIB_PATH := $(GCCLIB_PATH_32)
    endif

    ifneq ($(wildcard $(GCC_GXXLIB_PATH)),)
        GCC_GXXLIB_PATH := $(CXXLIB_PATH_64)
    else
        GCC_GXXLIB_PATH := $(CXXLIB_PATH_32)
    endif
endif

ifeq ($(wildcard $(GCC_GCCLIB_PATH)/$(LITEOS_GCCLIB)),)
    GCC_USE_CPU_OPT := n
endif
ifeq ($(LOSCFG_ARCH_XTENSA_LX6)$(LOSCFG_ARCH_XTENSA_L106), y)
    GCC_USE_CPU_OPT := n
endif

ifeq ($(GCC_USE_CPU_OPT), y)
    LITEOS_COMPILER_GCCLIB_PATH = $(GCC_GCCLIB_PATH)/$(LITEOS_GCCLIB)
    LITEOS_COMPILER_CXXLIB_PATH = $(GCC_GXXLIB_PATH)/$(LITEOS_GCCLIB)
else
    LITEOS_COMPILER_GCCLIB_PATH = $(GCC_GCCLIB_PATH)
    LITEOS_COMPILER_CXXLIB_PATH = $(GCC_GXXLIB_PATH)
endif
LITEOS_COMPILER_GCC_INCLUDE = -I $(GCC_GCCLIB_PATH)/include

LITEOS_COMPILER_CXX_PATH := $(LITEOS_COMPILER_PATH)/$(COMPILE_NAME)/include
LITEOS_CXXINCLUDE += \
    -I $(LITEOSTOPDIR)/lib/libcpp \
    -I $(LITEOS_COMPILER_CXX_PATH)/c++/$(VERSION_NUM) \
    -I $(LITEOS_COMPILER_CXX_PATH)/c++/$(VERSION_NUM)/bits \
    -I $(LITEOS_COMPILER_CXX_PATH)/c++/$(VERSION_NUM)/ext \
    -I $(LITEOS_COMPILER_CXX_PATH)/c++/$(VERSION_NUM)/backward \
    -I $(LITEOS_COMPILER_CXX_PATH)/c++/$(VERSION_NUM)/$(COMPILE_ALIAS)

define path_checker
    $(if $(wildcard $(1)),,$(error path $(1) is not exist))
endef

# Check GCC lib and include path is avaiable for now.
# CXX paths to be added. Since LiteOS support for C++ is based on whether those libs
# are compiled for LiteOS. C++ support needs to configured with Kconfigs.
$(call path_checker,$(LITEOS_COMPILER_GCCLIB_PATH))
$(call path_checker,$(GCC_GCCLIB_PATH)/include)
