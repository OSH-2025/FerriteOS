# CPU compile options
LOSCFG_ARCH_CPU    := riscv

ifeq ($(LOSCFG_ARCH_RISCV_RV32IMC), y)
LITEOS_ARCH_RISCV       := rvm32
LITEOS_CPU_OPTS         += -msmall-data-limit=0
ifeq ($(LOSCFG_ARCH_RISCV_RV32F), y)
ifeq ($(LOSCFG_ARCH_FPU_ENABLE), y)
LITEOS_CPU_OPTS         += -march=rv32imfc
else
LITEOS_CPU_OPTS         += -march=rv32imc
endif
else
LITEOS_CPU_OPTS         += -march=rv32imc
endif

ifeq ($(LOSCFG_RISCV_COMPILER_OPTIONS_EMIT_LLI), y)
    LITEOS_CPU_OPTS += -femit-lli
endif
endif

ARCH_C_OPT += -falign-functions=2 -fno-optimize-strlen -freorder-blocks-algorithm=simple -fno-schedule-insns

ARCH_CXX_OPT += -mabi=ilp32  \
                -falign-functions=2 -fno-optimize-strlen -freorder-blocks-algorithm=simple -fno-schedule-insns

ARCH_ASM_OPT += -x assembler-with-cpp

ifeq ($(LOSCFG_PLATFORM_GD32VF103V_EVAL)$(LOSCFG_PLATFORM_HIFIVE1_REV1_B01), y)
ARCH_C_OPT += -mabi=ilp32 -mcmodel=medlow
endif

LITEOS_BASELIB          += -l$(LOSCFG_ARCH_CPU)

LITEOS_CORE_COPTS        = $(LITEOS_CPU_OPTS)
LITEOS_INTERWORK        += $(LITEOS_CORE_COPTS)
LITEOS_NODEBUG          += $(LITEOS_CORE_COPTS)
LITEOS_ASOPTS           += $(LITEOS_CPU_OPTS) $(ARCH_ASM_OPT) $(ARCH_C_OPT)
LITEOS_COPTS_BASE       += $(ARCH_C_OPT)
LITEOS_CXXOPTS_BASE     += $(ARCH_CXX_OPT)

ifeq ($(LOSCFG_COMPILER_RISCV_UNKNOWN), y)
LOSCFG_RISCV_COMPILER_OPTIONS_USER_DEFINED := $(subst $\",,$(LOSCFG_RISCV_COMPILER_OPTIONS_USER_DEFINED))
LITEOS_COPTS_BASE += $(LOSCFG_RISCV_COMPILER_OPTIONS_USER_DEFINED)
LITEOS_COPTS_BASE += -mabi=ilp32 -mtune=size -fno-short-enums

ifeq ($(LOSCFG_RISCV_COMPILER_OPTIONS_PUSH_POP), y)
    LITEOS_COPTS_BASE += -mpush-pop  # failed to work when backtrace enabled
endif
ifeq ($(LOSCFG_RISCV_COMPILER_OPTIONS_SAVE_RESTORE), y)
    LITEOS_COPTS_BASE += -msave-restore  # failed to work when backtrace enabled
endif
ifeq ($(LOSCFG_RISCV_COMPILER_OPTIONS_NO_INLINE), y)
    # these options are used to easily patch for ROM codes
    LITEOS_COPTS_BASE += -fno-inline-small-functions -fno-inline-functions-called-once
endif
ifeq ($(LOSCFG_RISCV_COMPILER_OPTIONS_LBU_SB), y)
    LITEOS_COPTS_BASE += -Wa,-enable-c-lbu-sb
endif
endif

ifeq ($(LOSCFG_PLATFORM_GD32VF103V_EVAL), y)
LITEOS_COPTS_BASE += -Wl,-Bstatic  -ffunction-sections -fdata-sections -std=gnu11 -MMD -MP
endif

ifeq ($(LOSCFG_COMPILER_RISCV_NULEI)$(LOSCFG_COMPILER_RISCV64_UNKNOWN), y)
LITEOS_GCCLIB := /rv32imac/ilp32
endif

ifeq ($(LOSCFG_PLATFORM_HIFIVE1_REV1_B01)$(LOSCFG_COMPILER_RISCV64_UNKNOWN), yy)
LITEOS_LD_OPTS += -m elf32lriscv
endif

# strip quotation mark in configuration
LIB_SUBDIRS             += arch/riscv/$(LITEOS_ARCH_RISCV)

ARCH_INCLUDE            := -I $(LITEOSTOPDIR)/arch/riscv/include \
                           -I $(LITEOSTOPDIR)/arch/riscv/$(LITEOS_ARCH_RISCV)/include \
                           -I $(LITEOSTOPDIR)/arch/riscv/$(LITEOS_ARCH_RISCV)/include/arch

LITEOS_PLATFORM_INCLUDE += $(ARCH_INCLUDE)
LITEOS_CXXINCLUDE       += $(ARCH_INCLUDE)

# extra definition for other module
LITEOS_CPU_TYPE          = $(LOSCFG_ARCH_CPU)
