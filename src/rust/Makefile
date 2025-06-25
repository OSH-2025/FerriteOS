include $(LITEOSTOPDIR)/config.mk

MODULE_NAME     := $(notdir $(CURDIR))

all:
	$(HIDE)cargo build
	$(call add_lib_to_baselib_file,$(MODULE_NAME))

clean:
	$(HIDE)cargo clean

.PHONY: all clean