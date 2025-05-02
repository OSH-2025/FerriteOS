include $(LITEOSTOPDIR)/config.mk
LIBC_RELATIVE_PATH := ../../lib/libc
include libc_src.mk

MODULE_NAME := c

# libc module
LOCAL_SRCS_LIBC := $(LIBC_ERRNO_SRC) $(LIBC_STRING_SRC) $(LIBC_STDLIB_SRC) \
				   $(LIBC_INTERNAL_SRC) $(LIBC_TIME_SRC) $(LIBC_MISC_SRC) \
				   $(LIBC_LOCALE_SRC) $(LIBC_CTYPE_SRC) $(LIBC_TEMP_SRC) \
				   $(LIBC_FCNTL_SRC) $(LIBC_SIGNAL_SRC) $(LIBC_NETWORK_SRC) \
				   $(LIBC_UNISTD_SRC) $(LIBC_COMPLEX_SRC) $(LIBC_TERMIOS_SRC)

# huawei_libc module
LOCAL_DIRS_HUAWEI_LIBC := errno string stdlib signal tzdst lock
LOCAL_DIRS_HUAWEI_LIBC := $(addprefix ../huawei_libc/, $(LOCAL_DIRS_HUAWEI_LIBC))

LOCAL_DIRS_y := $(LOCAL_DIRS_HUAWEI_LIBC)
LOCAL_SRCS_y += $(LOCAL_SRCS_LIBC)
LOCAL_SRCS_y += $(foreach dir, $(LOCAL_DIRS_y), $(wildcard $(dir)/*.c))

# standard io basic function
LOCAL_SRCS_STDIO_BASE := sprintf.c asprintf.c snprintf.c vsprintf.c vasprintf.c vsnprintf.c vfprintf.c \
                        sscanf.c vsscanf.c vfscanf.c \
                        __uflow.c __toread.c
LOCAL_SRCS_STDIO_BASE := $(addprefix $(LIBC_RELATIVE_PATH)/src/stdio/, $(LOCAL_SRCS_STDIO_BASE))
LOCAL_SRCS_y += $(LOCAL_SRCS_STDIO_BASE)

# standard io extended function
ifeq ($(LOSCFG_LIB_STDIO), y)
LOCAL_SRCS_STDIO_EX := $(LIBC_STDIO_SRC)
LOCAL_SRCS_STDIO_EX := $(filter-out $(LOCAL_SRCS_STDIO_BASE), $(LOCAL_SRCS_STDIO_EX))
LOCAL_SRCS_y += $(LOCAL_SRCS_STDIO_EX)
endif

LOCAL_SRCS_y += ../huawei_libc/time/time.c ../huawei_libc/time/time64.c
ifneq ($(LOSCFG_COMPAT_POSIX), y)
LOCAL_SRCS_y += $(LIBC_RELATIVE_PATH)/src/prng/rand.c
endif

LOCAL_SRCS    = $(LOCAL_SRCS_y)

LOCAL_SRCS += $(wildcard $(LIBC_RELATIVE_PATH)/src/string/$(ARCH_LOCAL)/*.s)
ifneq ($(LOSCFG_ARCH_ARM_CORTEX_M), y)
LOCAL_SRCS += $(wildcard $(LIBC_RELATIVE_PATH)/src/string/$(ARCH_LOCAL)/*.S)
endif

FILTER_FILES :=
ifneq ($(findstring $(LOSCFG_ARCH_ARM_V7A)$(LOSCFG_ARCH_ARM_V7R), y y),)
FILTER_FILES += $(wildcard $(LIBC_RELATIVE_PATH)/src/string/memcpy.c) \
                $(wildcard $(LIBC_RELATIVE_PATH)/src/string/strcmp.c) \
                $(wildcard $(LIBC_RELATIVE_PATH)/src/string/memcmp.c)
ifeq ($(LOSCFG_ARCH_FPU_VFP_NEON), y)
FILTER_FILES += $(wildcard $(LIBC_RELATIVE_PATH)/src/string/memset.c)
endif
else ifneq ($(findstring $(LOSCFG_ARCH_RISCV_RV32IMC), y),)
FILTER_FILES += $(wildcard $(LIBC_RELATIVE_PATH)/src/string/memset.c) \
                $(wildcard $(LIBC_RELATIVE_PATH)/src/string/strcmp.c) \
                $(wildcard $(LIBC_RELATIVE_PATH)/src/string/memcpy.c)
else ifneq ($(findstring $(LOSCFG_ARCH_ARM_V8A)$(LOSCFG_ARCH_ARM_V8R), y y),)
FILTER_FILES += $(wildcard $(LIBC_RELATIVE_PATH)/src/string/strcmp.c) \
                $(wildcard $(LIBC_RELATIVE_PATH)/src/string/memcmp.c) \
                $(wildcard $(LIBC_RELATIVE_PATH)/src/string/memset.c) \
                $(wildcard $(LIBC_RELATIVE_PATH)/src/string/memcpy.c)
endif
LOCAL_SRCS := $(filter-out $(FILTER_FILES), $(LOCAL_SRCS))

LOCAL_INCLUDE := \
    -I $(LITEOS_LIBC_PATH)/src/include \
    -I $(LITEOS_LIBC_PATH)/src/internal \
    -I $(LITEOS_LIBC_PATH)/src/crypt \
    -I $(LITEOS_LIBC_PATH)/src/ctype \
    -I $(LITEOS_LIBC_PATH)/src/dirent \
    -I $(LITEOS_LIBC_PATH)/src/errno \
    -I $(LITEOS_LIBC_PATH)/src/ipc \
    -I $(LITEOS_LIBC_PATH)/src/locale \
    -I $(LITEOS_LIBC_PATH)/src/math \
    -I $(LITEOS_LIBC_PATH)/src/multibyte \
    -I $(LITEOS_LIBC_PATH)/src/network \
    -I $(LITEOS_LIBC_PATH)/src/passwd \
    -I $(LITEOS_LIBC_PATH)/src/prng \
    -I $(LITEOS_LIBC_PATH)/src/process \
    -I $(LITEOS_LIBC_PATH)/src/regex \
    -I $(LITEOS_LIBC_PATH)/src/search \
    -I $(LITEOS_LIBC_PATH)/src/time \
    -I $(LITEOSTOPDIR)/lib/huawei_libc/random/dev/random \
    -I $(LITEOSTOPDIR)/lib/huawei_libc/pthread \
    -I $(LITEOSTOPDIR)/lib/huawei_libc/tzdst \

ifeq ($(LOSCFG_LLTSER), y)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/tools/gcov_ser
endif

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE_LIBC)
