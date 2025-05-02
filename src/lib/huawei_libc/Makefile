include $(LITEOSTOPDIR)/config.mk
LIBC_RELATIVE_PATH := ../../lib/libc
include ../libc/libc_src.mk

MODULE_NAME := posix

LOCAL_DIRS_y :=

# huawei_libc path
LOCAL_DIRS_HUAWEI_LIBC := pthread mq sched misc

# musl_libc path
LOCAL_SRCS_LIBC := $(LIBC_EXIT_SRC) $(LIBC_PRNG_SRC) $(LIBC_MULTIBYTE_SRC) \
		           $(LIBC_ENV_SRC) $(LIBC_LINUX_SRC) $(LIBC_PROCESS_SRC) \
				   $(LIBC_LEGACY_SRC) $(LIBC_THREAD_SRC)

# posix configured module path
LOCAL_SRCS_$(LOSCFG_COMPAT_POSIX) += time/timer.c
LOCAL_SRCS_$(LOSCFG_COMPAT_POSIX) += $(LOCAL_SRCS_LIBC)
LOCAL_DIRS_$(LOSCFG_COMPAT_POSIX) += $(LOCAL_DIRS_HUAWEI_LIBC)

LOCAL_SRCS_y += $(foreach dir, $(LOCAL_DIRS_y), $(wildcard $(dir)/*.c))

LOCAL_SRCS    = $(LOCAL_SRCS_y)

LOCAL_INCLUDE := -I $(LITEOSTOPDIR)/lib/huawei_libc/pthread \
                 -I $(LITEOSTOPDIR)/lib/huawei_libc/errno \
                 -I $(LITEOSTOPDIR)/lib/huawei_libc/tzdst \
                 -I $(LITEOS_LIBC_PATH)/src/include \
                 -I $(LITEOS_LIBC_PATH)/src/internal \
                 -I $(LITEOS_LIBC_PATH)/src/time

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE_LIBC)
