include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/harfbuzz/harfbuzz.mk

MODULE_NAME := harfbuzz_demo

HARFBUZZ_INC += $(COMPONENTS_HARFBUZZ_INCLUDE)

LOCAL_INCLUDE := $(HARFBUZZ_INC)

LOCAL_SRCS := $(wildcard *.cpp)

LOCAL_CPPDEFS := -DHB_NO_MT -std=c++11 -D_GLIBCXX_HAS_GTHREADS

LOCAL_EXT_FLAG := \
    -Wno-error -Wno-deprecated-declarations \
    -Wno-write-strings -fstack-protector-strong

LOCAL_CPPFLAGS := $(LOCAL_CPPDEFS) $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
