include $(LITEOSTOPDIR)/config.mk

MODULE_NAME     := $(notdir $(CURDIR))

RUST_FEATURES :=

ifeq ($(LOSCFG_ENABLE_RUST_BITMAP), y)
RUST_FEATURES += bitmap
endif

ifeq ($(LOSCFG_ENABLE_RUST_EVENT), y)
RUST_FEATURES += event
endif

all:
	cargo build --features "$(RUST_FEATURES)"
	$(call add_lib_to_baselib_file,$(MODULE_NAME))

clean:
	$(HIDE)cargo clean

.PHONY: all clean