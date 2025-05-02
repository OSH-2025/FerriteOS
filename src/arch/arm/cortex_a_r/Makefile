include $(LITEOSTOPDIR)/config.mk

MODULE_NAME := $(LOSCFG_ARCH_CPU)

LOCAL_SRCS_y := $(wildcard src/canary.c) $(wildcard src/cpu.c) \
                $(wildcard src/fault.c) $(wildcard src/mmu.c)  \
                $(wildcard src/task.c) $(wildcard src/*.S)
                
LOCAL_INCLUDE := \
	-I $(LITEOSTOPDIR)/kernel/extended/include \

ifeq ($(LITEOS_ARM_ARCH), -march=armv7-a)
LOCAL_SRCS_y += $(wildcard src/armv7/*.S)
else ifeq ($(LITEOS_ARM_ARCH), -march=armv7-r)
LOCAL_SRCS_y += $(wildcard src/armv7/cache.S)
endif

ifneq ($(LOSCFG_APC_ENABLE), y)
LOCAL_SRCS_y := $(filter-out src/mmu.c, $(LOCAL_SRCS_y))
endif

ifeq ($(LOSCFG_KERNEL_PERF), y)
LOCAL_SRCS_y += $(wildcard src/pmu/armv7_pmu.c)
LOCAL_INCLUDE += -I $(LITEOSTOPDIR)/kernel/extended/src/pmu \
                 -I $(LITEOSTOPDIR)/kernel/extended/perf
endif

LOCAL_SRCS = $(LOCAL_SRCS_y)
LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS)

ifeq ($(LOSCFG_GDB), y)
LOCAL_FLAGS += $(AS_OBJS_LIBC_FLAGS)
endif
include $(MODULE)
