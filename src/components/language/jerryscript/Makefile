include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/language/jerryscript/jerryscript.mk

MODULE_NAME := $(notdir $(CURDIR))

JERRYSCRIPT_SRCS := \
    $(wildcard $(JERRYSCRIPT_VERSION)/jerry-core/api/*.c) \
    $(wildcard $(JERRYSCRIPT_VERSION)/jerry-core/debugger/*.c) \
    $(wildcard $(JERRYSCRIPT_VERSION)/jerry-core/ecma/base/*.c) \
    $(wildcard $(JERRYSCRIPT_VERSION)/jerry-core/ecma/builtin-objects/*.c) \
    $(wildcard $(JERRYSCRIPT_VERSION)/jerry-core/ecma/builtin-objects/typedarray/*.c) \
    $(wildcard $(JERRYSCRIPT_VERSION)/jerry-core/ecma/operations/*.c) \
    $(wildcard $(JERRYSCRIPT_VERSION)/jerry-core/jcontext/*.c) \
    $(wildcard $(JERRYSCRIPT_VERSION)/jerry-core/jmem/*.c) \
    $(wildcard $(JERRYSCRIPT_VERSION)/jerry-core/jrt/*.c) \
    $(wildcard $(JERRYSCRIPT_VERSION)/jerry-core/lit/*.c) \
    $(wildcard $(JERRYSCRIPT_VERSION)/jerry-core/parser/js/*.c) \
    $(wildcard $(JERRYSCRIPT_VERSION)/jerry-core/parser/regexp/*.c) \
    $(wildcard $(JERRYSCRIPT_VERSION)/jerry-core/vm/*.c) \
    $(wildcard $(JERRYSCRIPT_VERSION)/jerry-ext/arg/*.c) \
    $(wildcard $(JERRYSCRIPT_VERSION)/jerry-ext/debugger/*.c) \
    $(wildcard $(JERRYSCRIPT_VERSION)/jerry-ext/handle-scope/*.c) \
    $(wildcard $(JERRYSCRIPT_VERSION)/jerry-ext/handler/*.c) \
    $(wildcard $(JERRYSCRIPT_VERSION)/jerry-ext/include/*.c) \
    $(wildcard $(JERRYSCRIPT_VERSION)/jerry-ext/module/*.c) \
    $(wildcard $(JERRYSCRIPT_VERSION)/jerry-port/default/*.c)

JERRYSCRIPT_CMD_SRCS := \
    $(wildcard jerry_port/jerry_main.c) \
    $(wildcard jerry_port/jerry_shellcmd.c)

JERRYSCRIPT_SRCS += $(JERRYSCRIPT_CMD_SRCS)

JERRYSCRIPT_DIR := $(LITEOSTOPDIR)/components/language/jerryscript/$(JERRYSCRIPT_VERSION)
JERRYSCRIPT_INCLUDE := \
    -I $(JERRYSCRIPT_DIR)/jerry-core \
    -I $(JERRYSCRIPT_DIR)/jerry-core/api \
    -I $(JERRYSCRIPT_DIR)/jerry-core/debugger \
    -I $(JERRYSCRIPT_DIR)/jerry-core/ecma/base \
    -I $(JERRYSCRIPT_DIR)/jerry-core/ecma/builtin-objects \
    -I $(JERRYSCRIPT_DIR)/jerry-core/ecma/builtin-objects/typedarray \
    -I $(JERRYSCRIPT_DIR)/jerry-core/ecma/operations \
    -I $(JERRYSCRIPT_DIR)/jerry-core/jcontext \
    -I $(JERRYSCRIPT_DIR)/jerry-core/include \
    -I $(JERRYSCRIPT_DIR)/jerry-core/jcontext \
    -I $(JERRYSCRIPT_DIR)/jerry-core/jmem \
    -I $(JERRYSCRIPT_DIR)/jerry-core/jrt \
    -I $(JERRYSCRIPT_DIR)/jerry-core/js \
    -I $(JERRYSCRIPT_DIR)/jerry-core/lit \
    -I $(JERRYSCRIPT_DIR)/jerry-core/parser/js \
    -I $(JERRYSCRIPT_DIR)/jerry-core/parser/regexp \
    -I $(JERRYSCRIPT_DIR)/jerry-core/vm \
    -I $(JERRYSCRIPT_DIR)/jerry-core/vargm \
    -I $(JERRYSCRIPT_DIR)/jerry-ext/arg \
    -I $(JERRYSCRIPT_DIR)/jerry-ext/handler \
    -I $(JERRYSCRIPT_DIR)/jerry-ext/include \
    -I $(JERRYSCRIPT_DIR)/jerry-ext/module \
    -I $(JERRYSCRIPT_DIR)/jerry-ext/common \
    -I $(JERRYSCRIPT_DIR)/jerry-port/default/include \
    -I $(LITEOSTOPDIR)/components/language/jerryscript/jerry-port \
    -I $(LITEOSTOPDIR)/include

LOCAL_DEFS := \
    -D JERRY_COMMIT_HASH=\"\" \
    -D JERRY_NDEBUG

LOCAL_EXT_FLAG := \
    -Wno-error=float-equal \
    -Wno-float-equal \
    -fstack-protector-strong \
    -Wno-error=discarded-qualifiers

LOCAL_SRCS := $(JERRYSCRIPT_SRCS)
LOCAL_INCLUDE := $(JERRYSCRIPT_INCLUDE)

LOCAL_FLAGS := $(LOCAL_INCLUDE) $(LOCAL_EXT_FLAG) $(LOCAL_DEFS)
include $(MODULE)

