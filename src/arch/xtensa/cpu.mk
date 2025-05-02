# strip quotation mark in configuration

LOSCFG_ARCH_CPU_STRIP   := $(subst $\",,$(LOSCFG_ARCH_CPU))
LOSCFG_ARCH_CPU          = $(LOSCFG_ARCH_CPU_STRIP)

LOSCFG_ARCH_FPU_STRIP   := $(subst $\",,$(LOSCFG_ARCH_FPU))
LOSCFG_ARCH_FPU          = $(LOSCFG_ARCH_FPU_STRIP)

LITEOS_BASELIB          += -l$(LOSCFG_ARCH_CPU)

ifeq ($(LOSCFG_ARCH_XTENSA_LX6)$(LOSCFG_ARCH_XTENSA_L106), y)
XTENSA_ARCH_DIR := lx
endif
ifeq ($(LOSCFG_ARCH_XTENSA_XEA3), y)
XTENSA_ARCH_DIR := xea3
endif
ifeq ($(LOSCFG_ARCH_XTENSA_XEA2), y)
XTENSA_ARCH_DIR := xea2
endif
LIB_SUBDIRS             += arch/xtensa/$(XTENSA_ARCH_DIR)

ifeq ($(LOSCFG_HUAWEI_VISIONQ6_V3), y)
XTENSA_CORE              = Huawei_VisionQ6_v3
else ifeq ($(LOSCFG_XTENSA_HIFI3Z), y)
XTENSA_CORE              = H_3Z_ides
XTENSA_CORE_OPTS        += -DXCHAL_NUM_AREGS=64 -DLOSCFG_DSP_COPROCESS
endif

XTENSA_SYSTEM            = $(XTENSA_TOOL_DIR)/config
ifeq ($(LOSCFG_ARCH_XTENSA_LX6)$(LOSCFG_ARCH_XTENSA_L106), y)
XTENSA_CORE_OPTS        += -mtext-section-literals
else
XTENSA_CORE_OPTS        += --xtensa-core=$(XTENSA_CORE) --xtensa-system=$(XTENSA_SYSTEM)
endif
XTENSA_CORE_OPTS        += -mlongcalls
LITEOS_CORE_COPTS       += $(XTENSA_CORE_OPTS)
LITEOS_INTERWORK        += $(XTENSA_CORE_OPTS)
LITEOS_NODEBUG          += $(XTENSA_CORE_OPTS)
LITEOS_ASOPTS           += $(XTENSA_CORE_OPTS)

ARCH_INCLUDE            := -I $(LITEOSTOPDIR)/arch/xtensa/$(XTENSA_ARCH_DIR)/include \
                           -I $(LITEOSTOPDIR)/arch/xtensa/$(XTENSA_ARCH_DIR)/src/include \
                           -I $(LITEOSTOPDIR)/arch/xtensa/include

LITEOS_PLATFORM_INCLUDE += $(ARCH_INCLUDE)
LITEOS_CXXINCLUDE       += $(ARCH_INCLUDE)

LITEOS_LD_PATH += -L$(XTENSA_ELF_DIR)/arch/lib \
                  -L$(XTENSA_ELF_DIR)/lib \
                  -L$(XTENSA_ELF_DIR)/lib/xcc

# extra definition for other module
LITEOS_CPU_TYPE          = $(LOSCFG_ARCH_CPU)

# linux style macros
AS_OBJS_LIBC_FLAGS      += $(LINUX_ARCH_y)
