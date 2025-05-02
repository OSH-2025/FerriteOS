include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/media/opus/opus.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_DIRS_y :=
C_DEFS :=

LOCAL_DIRS_y += $(OPUS_VERSION)/celt
LOCAL_DIRS_y += $(OPUS_VERSION)/celt/arm
LOCAL_DIRS_y += $(OPUS_VERSION)/silk
LOCAL_DIRS_y += $(OPUS_VERSION)/silk/float
LOCAL_DIRS_y += $(OPUS_VERSION)/src

OPUS_DIR := $(LITEOSTOPDIR)/components/media/opus/$(OPUS_VERSION)

OPUS_INC += \
    -I $(OPUS_DIR) \
    -I $(OPUS_DIR)/src \
    -I $(OPUS_DIR)/include \
    -I $(OPUS_DIR)/celt \
    -I $(OPUS_DIR)/celt/arm \
    -I $(OPUS_DIR)/silk \
    -I $(OPUS_DIR)/silk/arm \
    -I $(OPUS_DIR)/silk/fixed \
    -I $(OPUS_DIR)/silk/float

OPUS_MAIN_HEAD += \
    -include $(OPUS_DIR)/silk/main.h

OPUS_SRCS := $(foreach dir, $(LOCAL_DIRS_y), $(wildcard $(dir)/*.c))

OPUS_SRCS_OUT := $(wildcard $(OPUS_VERSION)/celt/arm/celt*.c) \
    $(OPUS_VERSION)/celt/arm/pitch_neon_intr.c \
    $(OPUS_VERSION)/celt/opus_custom_demo.c \
    $(OPUS_VERSION)/src/opus_compare.c \
    $(OPUS_VERSION)/src/mlp_train.c \
    $(OPUS_VERSION)/src/opus_demo.c

OPUS_SRCS := $(filter-out $(OPUS_SRCS_OUT), $(OPUS_SRCS))

LOCAL_SRCS := $(OPUS_SRCS)

LOCAL_INCLUDE := $(OPUS_INC)

LOCAL_CDEFS = -DVAR_ARRAYS -DUSE_ALLOCA -DNONTHREADSAFE_PSEUDOSTACK -DOPUS_BUILD -DOPUS_WILL_BE_SLOW

LOCAL_EXT_FLAG := \
    -Wno-error -Wno-cpp \
    -Wno-float-equal \
    -fstack-protector-strong

LOCAL_FLAGS :=$(LOCAL_CDEFS) $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE) $(LITEOS_GCOV_OPTS) $(OPUS_MAIN_HEAD)

include $(MODULE)

