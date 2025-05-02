include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/libxml2/libxml2.mk

MODULE_NAME := xml2

LOCAL_DIRS_y := $(LIBXML2_VERSION)

XML2_DIR = $(LITEOSTOPDIR)/components/utility/libxml2

XML2_INC += \
    -I $(LITEOSTOPDIR)/lib/libc/include \
    -I $(XML2_DIR)/$(LIBXML2_VERSION) \
    -I $(XML2_DIR)/$(LIBXML2_VERSION)/include \
    -I $(XML2_DIR)/$(LIBXML2_VERSION)/include/libxml

LOCAL_SRCS := $(foreach dir, $(LOCAL_DIRS_y), $(wildcard $(dir)/*.c))
LOCAL_OUT := \
    $(LIBXML2_VERSION)/rngparser.c \
    $(LIBXML2_VERSION)/testThreadsWin32.c

LOCAL_SRCS := $(filter-out $(LOCAL_OUT),$(LOCAL_SRCS))

LOCAL_EXT_FLAG := \
    -Wno-error -Wno-misleading-indentation \
    -Wno-implicit-function-declaration -Wno-unused-function \
    -Wno-enum-compare -Wno-array-bounds -Wno-unused-const-variable \
    -Wno-strict-prototypes -Wno-unused-but-set-variable \
    -Wno-tautological-compare -Wno-switch -Wno-stringop-overflow \
    -Wno-float-equal -fstack-protector-strong

LOCAL_INCLUDE := $(XML2_INC)

LOCAL_FLAGS := $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
