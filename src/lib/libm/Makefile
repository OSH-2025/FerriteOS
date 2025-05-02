include $(LITEOSTOPDIR)/config.mk
LIBC_RELATIVE_PATH := ../../lib/libc

MODULE_NAME := m

LOCAL_SRCS = $(wildcard $(LIBC_RELATIVE_PATH)/src/math/*.c)

LOCAL_INCLUDE := \
        -I $(LITEOS_LIBC_PATH)/src/include \
        -I $(LITEOS_LIBC_PATH)/src/internal

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

include $(MODULE_LIBC)

