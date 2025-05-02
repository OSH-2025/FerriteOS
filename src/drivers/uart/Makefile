include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(notdir $(CURDIR))

MODULE_$(LOSCFG_DRIVERS_UART_ARM_PL011) += src/arm_pl011

ifneq ($(findstring y, $(LOSCFG_DRIVERS_UART_ARM_PL011)), y)
    LOCAL_SRCS_$(LOSCFG_DRIVERS_SIMPLE_UART) += src/generic_uart/uart_debug.c
endif

LOCAL_SRCS = $(LOCAL_SRCS_y)

LOCAL_FLAGS := $(PLATFORM_INCLUDE) $(PLATFORM_DEFS)

include $(MODULE)
