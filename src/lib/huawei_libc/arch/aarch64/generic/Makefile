include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := csysdeps

LOCAL_INCLUDE :=

LOCAL_SRCS := strcmp.S memcmp.S memset.S memcpy.S

LOCAL_ASFLAGS := -Wa, $(AS_OBJS_LIBC_FLAGS)

LOCAL_FLAGS := $(LOCAL_INCLUDE)

include $(MODULE_LIBC)
