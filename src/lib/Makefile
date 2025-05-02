include $(LITEOSTOPDIR)/config.mk

MODULE_y += libsec

MODULE_$(LOSCFG_LIB_LIBC) += libc huawei_libc
MODULE_$(LOSCFG_LIB_LIBM) += libm
MODULE_$(LOSCFG_LIB_ZLIB) += zlib

ifneq ($(findstring $(LOSCFG_ARCH_ARM_V7A)$(LOSCFG_ARCH_ARM_V7R), y y),)
MODULE_$(LOSCFG_LIB_LIBC) += huawei_libc/arch/$(ARCH_LOCAL)/armv7
else ifneq ($(findstring $(LOSCFG_ARCH_RISCV_RV32IMC), y),)
MODULE_$(LOSCFG_LIB_LIBC) += huawei_libc/arch/$(ARCH_LOCAL)
else ifneq ($(findstring $(LOSCFG_ARCH_ARM_V8A)$(LOSCFG_ARCH_ARM_V8R), y y),)
MODULE_$(LOSCFG_LIB_LIBC) += huawei_libc/arch/$(ARCH_LOCAL)/generic
endif
MODULE_$(LOSCFG_LIB_CPP_EXTEND) += libcpp

include $(MODULE)