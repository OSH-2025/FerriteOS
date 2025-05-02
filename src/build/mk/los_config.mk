# Copyright (c) Huawei Technologies Co., Ltd. 2013-2020. All rights reserved.

## platform relative ##
LITEOS_ARM_ARCH :=
LITEOS_CPU_TYPE :=
## c as cxx ld options ##
LITEOS_ASOPTS :=
LITEOS_COPTS_BASE :=
LITEOS_COPTS_EXTRA :=
LITEOS_COPTS_EXTRA_INTERWORK :=
LITEOS_COPTS_DEBUG :=
LITEOS_COPTS_NODEBUG :=
LITEOS_INTERWORK :=
LITEOS_CXXOPTS :=
LITEOS_CXXOPTS_BASE :=
LITEOS_LD_OPTS :=
LITEOS_GCOV_OPTS :=
## dynload ld options ##
LITEOS_DYNLOADOPTS :=
## macro define ##
LITEOS_CMACRO :=
LITEOS_CMACRO_TEST :=
LITEOS_CXXMACRO :=
## head file path and ld path ##
LITEOS_PLATFORM_INCLUDE :=
LITEOS_CXXINCLUDE :=
LITEOS_LD_PATH :=
LITEOS_LD_SCRIPT :=
LITEOS_MK_PATH :=
## c as cxx ld flags ##
LITEOS_ASFLAGS :=
LITEOS_U_INCLUDES :=
LITEOS_K_INCLUDES :=
LITEOS_CFLAGS :=
LITEOS_CFLAGS_INTERWORK :=
LITEOS_LDFLAGS :=
LITEOS_CXXFLAGS :=
LIBC_FILTER_OPTS :=
## depended lib ##
LITEOS_BASELIB :=
LITEOS_LIBDEP :=
## directory ##
LIB_BIGODIR :=
LIB_SUBDIRS :=
##compiler##
LITEOS_COMPILER_CXXLIB_PATH :=
LITEOS_COMPILER_GCCLIB_PATH :=
LITEOS_COMPILER_GCC_INCLUDE :=

## opensource path ##
LITEOS_ZLIB_PATH := $(LITEOSTOPDIR)/lib/zlib/include
LITEOS_LIBC_PATH := $(LITEOSTOPDIR)/lib/libc

-include $(LITEOSTOPDIR)/.config

OS ?= $(shell uname -s)
# Introduced compiler binaries which might different from gcc compilers.
# Those compilers need to implement like $(CC) $(GPP) like gcc does.
# Meanwhile Introduced compiler includes and lib paths
# Export : $(LITEOS_COMPILER_GCC_INCLUDE)
#          $(LITEOS_CXXINCLUDE)
#          $(LITEOS_COMPILER_GCCLIB_PATH)
#          $(LITEOS_COMPILER_CXXLIB_PATH)
ifeq ($(LOSCFG_COMPILER_XTENSA_32), y)
include $(LITEOSTOPDIR)/build/mk/compiler_xtensa.mk
else
# Supported GCC Compilers
# Also support "make CROSS_COMPILE=" to use third party compilers.
include $(LITEOSTOPDIR)/build/mk/compiler_gcc.mk
endif

HIDE    := @
MKDIR    = mkdir
RM       = -rm -rf
ECHO     = echo
ARFLAGS := cr
PYTHON  := python
PYTHON3  := python3

OS       ?= $(shell uname -s)
OBJ_MKDIR = if [ ! -d $(dir $@) ]; then mkdir -p $(dir $@); fi

# Generate file that contains link targets.
# Also support old style sub module make directly using "LITEOS_BASELIB +="
# which is not recommented.
include $(LITEOSTOPDIR)/build/mk/gen_link_targets.mk

## variable define ##
OUT  = $(LITEOSTOPDIR)/out/$(LITEOS_PLATFORM)
BUILD  = $(OUT)/obj
MK_PATH  = $(LITEOSTOPDIR)/build/mk
LITEOS_SCRIPTPATH  ?= $(LITEOSTOPDIR)/tools/scripts
LITEOS_LIB_BIGODIR  = $(OUT)/lib/obj
LOSCFG_ENTRY_SRC    = $(LITEOSTOPDIR)/kernel/init/los_init.c
LITEOS_MENUCONFIG_H = $(LITEOSTOPDIR)/targets/menuconfig.h
LITEOS_PLATFORM_MENUCONFIG_H = $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/include/menuconfig.h
BOARD_LD_FILE = $(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/board.ld
BOARD_LD_S_FILE = $(BOARD_LD_FILE).S

### include variable
MODULE = $(MK_PATH)/module.mk
MODULE_LIBC = $(MK_PATH)/module_libc.mk

ifeq ($(LOSCFG_COMPILER_HIMIX_32), y)
LITEOS_CMACRO      += -D__COMPILER_HUAWEILITEOS__
else ifeq ($(LOSCFG_COMPILER_HIMIX210_64), y)
LITEOS_CMACRO      += -D__COMPILER_HUAWEILITEOS__
else ifeq ($(LOSCFG_COMPILER_HCC_64), y)
LITEOS_CMACRO      += -D__COMPILER_HUAWEILITEOS__
else ifeq ($(LOSCFG_COMPILER_HIMIX310_32), y)
LITEOS_CMACRO      += -D__COMPILER_HUAWEILITEOS__
else ifeq ($(LOSCFG_COMPILER_CSKYV2), y)
LITEOS_CMACRO      += -D__COMPILER_HUAWEILITEOS__
LITEOS_CMACRO      += -D__COMPILER_CSKY__
endif

LITEOS_CMACRO      += -D__LITEOS__ -DSECUREC_IN_KERNEL=0 -D_ALL_SOURCE
ifneq ($(LOSCFG_ARCH_XTENSA_LX6), y)
LITEOS_BASELIB     += -lgcc
endif
ifneq ($(LOSCFG_COMPILER_ARM_NONE_EABI)$(LOSCFG_COMPILER_RISCV_UNKNOWN)$(LOSCFG_COMPILER_RISCV_NULEI)$(LOSCFG_COMPILER_XTENSA_LX6)$(LOSCFG_COMPILER_XTENSA_L106)$(LOSCFG_COMPILER_XTENSA_32)$(LOSCFG_COMPILER_CSKYV2)$(LOSCFG_COMPILER_RISCV64_UNKNOWN)$(LOSCFG_COMPILER_GCC_64), y)
LITEOS_BASELIB     += -lgcc_eh
endif
AS_OBJS_LIBC_FLAGS  = -D__ASSEMBLY__
WARNING_AS_ERROR   := -Wall -Werror

ifeq ($(LOSCFG_KERNEL_DYNLOAD), y)
-include $(LITEOSTOPDIR)/build/mk/dynload_ld.mk
endif

include $(LITEOSTOPDIR)/arch/cpu.mk
include $(LITEOSTOPDIR)/targets/bsp.mk

include $(LITEOSTOPDIR)/lib/api.mk
include $(LITEOSTOPDIR)/drivers/api.mk

include $(LITEOSTOPDIR)/compat/api.mk
include $(LITEOSTOPDIR)/shell/api.mk
include $(LITEOSTOPDIR)/components/components.mk
include $(LITEOSTOPDIR)/demos/demos.mk

LIB_SUBDIRS += kernel lib drivers osdepends

# Export APIs from these module.
ifneq ($(wildcard $(LITEOSTOPDIR)/compat),)
LIB_SUBDIRS += compat
endif
ifneq ($(wildcard $(LITEOSTOPDIR)/shell),)
LIB_SUBDIRS += shell
endif
ifneq ($(wildcard $(LITEOSTOPDIR)/components),)
LIB_SUBDIRS += components
endif
ifneq ($(wildcard $(LITEOSTOPDIR)/demos),)
LIB_SUBDIRS += demos
endif

LITEOS_KERNEL_INCLUDE := -I $(LITEOSTOPDIR)/kernel/include

# auto added the header file LITEOS_PLATFORM_MENUCONFIG_H in all files
LITEOS_PLATFORM_INCLUDE += -include $(LITEOS_PLATFORM_MENUCONFIG_H)
LITEOS_CXXINCLUDE       += -include $(LITEOS_PLATFORM_MENUCONFIG_H)

############################# Tools && Debug Option Begin ##############################

ifeq ($(LOSCFG_3RDPARTY), y)
    -include $(LITEOSTOPDIR)/3rdParty/3rdParty.mk
endif

ifeq ($(LOSCFG_PLATFORM_OSAPPINIT), y)

else
    -include $(LITEOSTOPDIR)/test/test.mk
endif

ifeq ($(LOSCFG_NULL_ADDRESS_PROTECT), y)
    LITEOS_CMACRO += -DLOSCFG_NULL_ADDRESS_PROTECT
endif

############################# Tools && Debug Option End #################################

## Optimize Options
ifeq ($(LOSCFG_COMPILER_OPTIMIZE_NONE), y)
    LITEOS_COPTS_OPTIMIZE = -O0
else ifeq ($(LOSCFG_COMPILER_OPTIMIZE_SPEED), y)
    LITEOS_COPTS_OPTIMIZE = -O2
else ifeq ($(LOSCFG_COMPILER_OPTIMIZE_SIZE), y)
    LITEOS_COPTS_OPTIMIZE = -Os
endif

ifeq ($(LOSCFG_COMPILE_DEBUG), y)
    LITEOS_COPTS_OPTION  = -g -gdwarf-2
endif

LITEOS_COPTS_DEBUG  += $(LITEOS_COPTS_OPTION) $(LITEOS_COPTS_OPTIMIZE)
LITEOS_INTERWORK    += $(LITEOS_COPTS_OPTION) $(LITEOS_COPTS_OPTIMIZE)
LITEOS_CXXOPTS_BASE += $(LITEOS_COPTS_OPTION) $(LITEOS_COPTS_OPTIMIZE)
LITEOS_ASOPTS       += $(LITEOS_COPTS_OPTION)

## Security Options
ifeq ($(LOSCFG_CC_STACKPROTECTOR), y)
    LITEOS_SSP = -fstack-protector --param ssp-buffer-size=4
else ifeq ($(LOSCFG_CC_STACKPROTECTOR_STRONG), y)
    LITEOS_SSP = -fstack-protector-strong
else ifeq ($(LOSCFG_CC_STACKPROTECTOR_ALL), y)
    LITEOS_SSP = -fstack-protector-all
endif

# THIS feature whether support or not depands on if its C++ libs
# are compiled with liteos which should be controlled with KCONFIGS.
LITEOS_CMACRO     += -DLOSCFG_KERNEL_CPP_EXCEPTIONS_SUPPORT
LITEOS_CXXMACRO   += -DLOSCFG_KERNEL_CPP_EXCEPTIONS_SUPPORT
LIBC_FILTER_OPTS  += -Wfloat-equal
LITEOS_COMMON_OPTS = -fno-pic -fno-builtin -funsigned-char \
                     -ffunction-sections -fdata-sections \
                     $(WARNING_AS_ERROR) $(LITEOS_SSP) $(LIBC_FILTER_OPTS)

LITEOS_CXXMACRO   += -D_U=1 -D_L=2 -D_N=4 -D_S=8 -D_P=16 -D_C=32 -D_X=64 -D_B=128
ifeq ($(LOSCFG_LIB_CPP_EXTEND), y)
LITEOS_CXXMACRO   += -D_GLIBCXX_HAS_GTHREADS
endif

ifeq ($(LOSCFG_ARCH_XTENSA_XEA3), y)
# xea2 don't support shadow due to libc in compiler
LITEOS_COMMON_OPTS += -Wshadow
endif

ifeq ($(LOSCFG_ARCH_XTENSA),)
## xtensa do not have -freg-struct-return.
LITEOS_COMMON_OPTS += -freg-struct-return
endif

LITEOS_COMMON_OPTS += -Wformat=2
ifneq ($(LOSCFG_COMPILER_CLANG), y)
LITEOS_COMMON_OPTS += -Wtrampolines
endif

ifeq ($(LOSCFG_LIB_LIBC), y)
    LITEOS_COMMON_OPTS += -nostdinc -nostdlib
endif

LITEOS_COPTS_BASE += $(LITEOS_COMMON_OPTS)

# Anonymous structs and unions are supported on c11; while gcc supports those features as extension
# which is turn on for default. As for Clang, if choose c99, -std=gnu99 should be used.
ifeq ($(LOSCFG_COMPILER_CLANG)$(LOSCFG_COMPILER_XTENSA_L106), y)
LITEOS_COPTS_BASE += -std=gnu99
else
LITEOS_COPTS_BASE += -std=c99
endif

# -Wpointer-arith will treat the size of a void or of a function as 1.
# -Wstrict-prototypes will warn if a function is defined without specifying the argument types.
# -pipe will use pipes, save compilation time
LITEOS_COPTS_BASE += -Wpointer-arith -Wstrict-prototypes -fno-exceptions -pipe

ifeq ($(LOSCFG_COMPILER_GCC), y)
LITEOS_COPTS_BASE += -fno-aggressive-loop-optimizations
endif

# clang support -fno-omit-frame-pointer
ifeq ($(LOSCFG_BACKTRACE), y)
ifeq ($(LOSCFG_ARCH_ARM_CORTEX_M)$(LOSCFG_ARCH_XTENSA),)
LITEOS_COPTS_BASE += -fno-omit-frame-pointer
LITEOS_CXXOPTS_BASE += -fno-omit-frame-pointer
endif
endif

ifneq ($(LOSCFG_COMPILER_XTENSA_32), y)
LITEOS_COPTS_BASE += -Winvalid-pch
endif

ifneq ($(LOSCFG_ARCH_RISCV), y)
LITEOS_COPTS_BASE += -fno-short-enums
endif

LITEOS_CXXOPTS_BASE += -std=c++11 -nostdinc++ -fexceptions -fpermissive -fno-use-cxa-atexit -frtti \
                       $(LITEOS_COMMON_OPTS)

ifneq ($(LOSCFG_COMPILER_XTENSA_32), y)
LITEOS_CXXOPTS_BASE += -Winvalid-pch
endif

ifeq ($(LOSCFG_LLTREPORT), y)
# -fprofile-arcs may introduce false alarm on 'maybe-uninitialized'
LITEOS_GCOV_OPTS := -fprofile-arcs -ftest-coverage -Wno-maybe-uninitialized
LITEOS_BASELIB += -lgcov
endif

ifeq ($(LD), $(CC))
    LITEOS_LD_OPTS += -nostartfiles -static -Wl,-gc-sections
else
    LITEOS_LD_OPTS += -nostartfiles -static --gc-sections
endif

ifeq ($(findstring y, $(LOSCFG_ARCH_CORTEX_M0)$(LOSCFG_ARCH_CORTEX_M0_PLUS)), y)
LITEOS_COPTS_BASE += -fshort-enums
endif

LITEOS_LD_OPTS += $(LITEOS_DYNLOADOPTS)
LITEOS_LD_PATH += -L$(LITEOS_SCRIPTPATH)/ld -L$(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM) -L$(OUT)/lib -L$(LITEOS_LIB_BIGODIR) -L$(LITEOSTOPDIR)/tools/build
LITEOS_LD_PATH += -L$(LITEOS_COMPILER_GCCLIB_PATH) -L$(LITEOS_COMPILER_CXXLIB_PATH)

ifeq ($(LOSCFG_VENDOR), y)
LITEOS_LD_PATH +=  -L$(OUT)/lib/rdk \
                   -L$(OUT)/lib/sdk \
                   -L$(OUT)/lib/main_server
endif

ifeq ($(LOSCFG_DEMOS_AI), y)
LITEOS_LD_PATH +=  -L$(LITEOSTOPDIR)/components/ai/nnacl_lib
endif

ifeq ($(LOSCFG_KERNEL_LMS), y)
ifeq ($(LOSCFG_ARCH_ARM_CORTEX_M),y)
LITEOS_LD_PATH +=  -L$(LITEOSTOPDIR)/kernel/extended/lms/cortex_m/$(LOSCFG_ARCH_CPU)
else
LITEOS_LD_PATH +=  -L$(LITEOSTOPDIR)/kernel/extended/lms/cortex_a_r
endif
endif

ifeq ($(LOSCFG_USING_BOARD_LD), y)
ifeq ($(LOSCFG_BOOTLOADER), y)
LITEOS_LD_SCRIPT := -T$(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/liteos_bootloader.ld
else ifeq ($(LOSCFG_COMPONENTS_OTA), y)
LITEOS_LD_SCRIPT := -T$(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/liteos_ota.ld
else
LITEOS_LD_SCRIPT := -T$(LITEOSTOPDIR)/targets/$(LITEOS_PLATFORM)/liteos.ld
endif
endif

# temporary
LITEOS_PLATFORM_INCLUDE += \
        -I $(LITEOSTOPDIR)/kernel/base/include
LITEOS_CXXINCLUDE += \
        $(LITEOS_COMPILER_GCC_INCLUDE) \
        $(LITEOS_NET_INCLUDE) \
        -I $(LITEOSTOPDIR)/kernel/base/include \
        -I $(LITEOSTOPDIR)/compat/posix/include \
        -I $(LITEOS_LIBC_PATH)/include \
        -I $(LITEOSTOPDIR)/fs/include \
        -I $(LITEOSTOPDIR)/kernel/include \
        $(LITEOS_LIBC_INCLUDE) \
        $(LITEOS_DRIVERS_INCLUDE)

LOSCFG_TOOLS_DEBUG_INCLUDE := $(LITEOS_SHELL_INCLUDE) $(LITEOS_UART_INCLUDE)

LITEOS_COPTS_NODEBUG    := $(LITEOS_NODEBUG) $(LITEOS_COPTS_BASE) $(LITEOS_COPTS_EXTRA)
LITEOS_COPTS_INTERWORK  := $(LITEOS_INTERWORK) $(LITEOS_COPTS_BASE) $(LITEOS_COPTS_EXTRA_INTERWORK)
LITEOS_BASE_INCLUDE     := $(LITEOS_KERNEL_INCLUDE) $(LITEOS_PLATFORM_INCLUDE) \
                           $(LITEOS_LIB_INCLUDE) $(LITEOS_FS_INCLUDE) \
                           $(LITEOS_EXTKERNEL_INCLUDE) \
                           $(LITEOS_COMPAT_INCLUDE) $(LITEOS_DRIVERS_INCLUDE) \
                           $(LOSCFG_TOOLS_DEBUG_INCLUDE) $(LITEOS_NET_INCLUDE) \
                           $(COMPONENTS_INCLUDE) $(DEMOS_INCLUDE)
LITEOS_CFLAGS_INTERWORK := $(LITEOS_COPTS_INTERWORK) $(LITEOS_CMACRO) \
                           $(LITEOS_CMACRO_TEST)     $(LITEOS_IMAGE_MACRO) \
                           $(LITEOS_BASE_INCLUDE)
LITEOS_CFLAGS_NODEBUG   := $(LITEOS_COPTS_NODEBUG) $(LITEOS_CMACRO) \
                           $(LITEOS_CMACRO_TEST)   $(LITEOS_IMAGE_MACRO) \
                           $(LITEOS_BASE_INCLUDE)
