include $(LITEOSTOPDIR)/config.mk

MODULE_NAME = c++

LOCAL_SRCS := $(wildcard *.cc)

$(shell sh build.sh)

LOCAL_INCLUDE := -I $(LITEOSTOPDIR)/lib/libcpp \
    -I $(LITEOSTOPDIR)/lib/libc/include \
    -I $(LITEOSTOPDIR)/lib/libc

LOCAL_CPPFLAGS := $(LOCAL_INCLUDE) -Wno-error -D_GLIBCXX_HAS_GTHREADS

include $(MODULE_LIBC)