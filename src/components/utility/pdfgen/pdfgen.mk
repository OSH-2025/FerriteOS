PDFGEN_VERSION = PDFGen-0.1.0

COMPONENTS_PDFGEN_INCLUDE += \
    -I $(LITEOSTOPDIR)/components/utility/pdfgen/$(PDFGEN_VERSION)

ifneq ($(OS), Linux)
    LITEOS_BASELIB += -lpdfgen
endif

