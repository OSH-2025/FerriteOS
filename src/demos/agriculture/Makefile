include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := agriculture

LOCAL_SRCS := $(wildcard *.c) \
	$(wildcard e53_ia1/*.c)

LOCAL_INCLUDE := -I $(LITEOSTOPDIR)/demos/agriculture/e53_ia1 \
    -I $(LITEOSTOPDIR)/targets/STM32L431_BearPi/Hardware/LCD

LOCAL_EXT_FLAG := -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG) 

include $(MODULE)
