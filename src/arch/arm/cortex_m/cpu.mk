# strip quotation mark in configuration

LOSCFG_ARCH_CPU_STRIP   := $(subst $\",,$(LOSCFG_ARCH_CPU))
LOSCFG_ARCH_CPU          = $(LOSCFG_ARCH_CPU_STRIP)

LOSCFG_ARCH_FPU_STRIP   := $(subst $\",,$(LOSCFG_ARCH_FPU))
LOSCFG_ARCH_FPU          = $(LOSCFG_ARCH_FPU_STRIP)

LITEOS_BASELIB          += -l$(LOSCFG_ARCH_CPU)
LIB_SUBDIRS             += arch/arm/cortex_m


# FPU compile options: hard/soft/softfp, use hard as default
ifeq ($(findstring y, $(LOSCFG_ARCH_CORTEX_M0)$(LOSCFG_ARCH_CORTEX_M0_PLUS)), y)
# cortex-m0 and cortex-m0plus don't support fpu
else ifeq ($(LOSCFG_ARCH_CORTEX_M3), y)
LITEOS_FLOAT_OPTS       := -mfloat-abi=softfp
else ifeq ($(LOSCFG_ARCH_CORTEX_M33), y)
LITEOS_FPU_OPTS         := -mfpu=fpv5-d16
LITEOS_FLOAT_OPTS       := -mfloat-abi=softfp
else
ifeq ($(LOSCFG_ARCH_FPU_ENABLE), y)
LITEOS_FLOAT_OPTS       := -mfloat-abi=softfp
LITEOS_FPU_OPTS         := -mfpu=$(LOSCFG_ARCH_FPU)
else
EXTENSION               := +nofp
LITEOS_FLOAT_OPTS       := -mfloat-abi=soft
endif
endif

# CPU compile options
LITEOS_CPU_OPTS         := -mcpu=$(LOSCFG_ARCH_CPU)$(EXTENSION)

# gcc libc folder style is combine with core and fpu, use thumb as default
# for example, cortex-m7 with softfp abi and thumb is: thumb/v7e-m+fp/softfp
# attention: for v6e, can use thumb/v6-m/nofp/libgcc.a
ifdef LOSCFG_ARCH_ARM_V6M
LITEOS_GCCLIB           := thumb/v6-m/nofp
else
ifeq ($(LOSCFG_ARCH_FPU_ENABLE), y)
LITEOS_GCCLIB           := thumb/v7e-m+fp/$(subst -mfloat-abi=,,$(LITEOS_FLOAT_OPTS))
else
LITEOS_GCCLIB           := thumb/v7-m/nofp
endif
endif

LITEOS_CORE_COPTS        = $(LITEOS_CPU_OPTS) $(LITEOS_FLOAT_OPTS) $(LITEOS_FPU_OPTS)
LITEOS_INTERWORK        += $(LITEOS_CORE_COPTS)
LITEOS_NODEBUG          += $(LITEOS_CORE_COPTS)
LITEOS_ASOPTS           += $(LITEOS_CPU_OPTS)
LITEOS_CXXOPTS_BASE     += $(LITEOS_CORE_COPTS)

ARCH_INCLUDE            := -I $(LITEOSTOPDIR)/arch/arm/cortex_m/include \
                           -I $(LITEOSTOPDIR)/arch/arm/cmsis

LITEOS_PLATFORM_INCLUDE += $(ARCH_INCLUDE)
LITEOS_CXXINCLUDE       += $(ARCH_INCLUDE)

# extra definition for other module
LITEOS_CPU_TYPE          = $(LOSCFG_ARCH_CPU)
LITEOS_ARM_ARCH         := -march=$(subst $\",,$(LOSCFG_ARCH_ARM_VER))

# linux style macros
LINUX_ARCH_$(LOSCFG_ARCH_ARM_V6M) = -D__LINUX_ARM_ARCH__=6
LINUX_ARCH_$(LOSCFG_ARCH_ARM_V7M) = -D__LINUX_ARM_ARCH__=7
LINUX_ARCH_$(LOSCFG_ARCH_ARM_V8M) = -D__LINUX_ARM_ARCH__=8

AS_OBJS_LIBC_FLAGS      += $(LINUX_ARCH_y)
