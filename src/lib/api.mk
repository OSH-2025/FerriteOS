# Copyright (c) Huawei Technologies Co., Ltd. 2020-2020. All rights reserved.

LITEOS_LIBC_INCLUDE := -I $(LITEOSTOPDIR)/lib/libsec/include

# Exporting musl libc APIs
#
# Attention:
# './lib/libc/src/include' and 'lib/libc/src/internal' should not exported,
# while os would compile failed if those are not included which should be fix.
ifeq ($(LOSCFG_LIB_LIBC), y)
    ifeq ($(LOSCFG_ARCH_ARM_AARCH32), y)
        ARCH_LOCAL := arm
    else ifeq ($(LOSCFG_ARCH_ARM_AARCH64), y)
        ARCH_LOCAL := aarch64
    else ifeq ($(LOSCFG_ARCH_RISCV_RV32IMC), y)
        ARCH_LOCAL := riscv32
    else ifeq ($(LOSCFG_ARCH_CSKY), y)
        ARCH_LOCAL := csky
    else ifeq ($(LOSCFG_ARCH_XTENSA), y)
        ARCH_LOCAL := xtensa
    endif

    LITEOS_LIBC_INCLUDE += \
        -I $(LITEOS_LIBC_PATH)/arch/$(ARCH_LOCAL) \
        -I $(LITEOS_LIBC_PATH)/arch/generic \
        -I $(LITEOSTOPDIR)/lib/huawei_libc/include \
        -I $(LITEOS_LIBC_PATH)/include

endif

ifeq ($(LOSCFG_LIB_ZLIB), y)
    LITEOS_ZLIB_INCLUDE += -I $(LITEOS_ZLIB_PATH)
endif

ifeq ($(LOSCFG_KERNEL_CPPSUPPORT), y)
    LITEOS_BASELIB += -lsupc++ -lstdc++
endif

LITEOS_LIB_INCLUDE := $(LITEOS_LIBC_INCLUDE) $(LITEOS_LIBM_INCLUDE) \
                      $(LITEOS_ZLIB_INCLUDE) $(LITEOS_COMPILER_GCC_INCLUDE)