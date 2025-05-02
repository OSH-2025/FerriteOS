DEMOS_PDFGEN_INCLUDE += \
    -I $(LITEOSTOPDIR)/demos/utility/pdfgen

ifneq ($(OS), Linux)
    LITEOS_BASELIB += -lpdfgen_demo
endif

