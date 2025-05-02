# Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.

ifeq ($(LOSCFG_DRIVERS_BASE), y)
    LITEOS_DBASE_INCLUDE   += -I $(LITEOSTOPDIR)/drivers/base/include
endif

ifeq ($(LOSCFG_DRIVERS_SIMPLE_UART), y)
    LITEOS_UART_INCLUDE += -I $(LITEOSTOPDIR)/drivers/uart/include
endif

LITEOS_DRIVERS_INCLUDE     := -I $(LITEOSTOPDIR)/drivers/interrupt/include \
                              -I $(LITEOSTOPDIR)/drivers/timer/include

LITEOS_DRIVERS_INCLUDE     += $(LITEOS_UART_INCLUDE) $(LITEOS_DBASE_INCLUDE)
