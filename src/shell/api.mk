# Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.

ifeq ($(LOSCFG_SHELL), y)
    LITEOS_SHELL_INCLUDE  += -I $(LITEOSTOPDIR)/shell/include
endif
