include $(LITEOSTOPDIR)/config.mk
include $(LITEOSTOPDIR)/components/utility/freetype/freetype.mk

MODULE_NAME := $(notdir $(CURDIR))

LOCAL_INCLUDE := \
    -I $(FREETYPE_VERSION)/include \
    -I $(FREETYPE_VERSION)/src/autofit \
    -I $(FREETYPE_VERSION)/src/base \
    -I $(FREETYPE_VERSION)/src/bdf \
    -I $(FREETYPE_VERSION)/src/cache \
    -I $(FREETYPE_VERSION)/src/cff \
    -I $(FREETYPE_VERSION)/src/cid \
    -I $(FREETYPE_VERSION)/src/gxvalid \
    -I $(FREETYPE_VERSION)/src/gzip \
    -I $(FREETYPE_VERSION)/src/lzw \
    -I $(FREETYPE_VERSION)/src/otvalid \
    -I $(FREETYPE_VERSION)/src/pcf \
    -I $(FREETYPE_VERSION)/src/pfr \
    -I $(FREETYPE_VERSION)/src/psaux \
    -I $(FREETYPE_VERSION)/src/pshinter \
    -I $(FREETYPE_VERSION)/src/psnames \
    -I $(FREETYPE_VERSION)/src/raster \
    -I $(FREETYPE_VERSION)/src/sfnt \
    -I $(FREETYPE_VERSION)/src/smooth \
    -I $(FREETYPE_VERSION)/src/tools \
    -I $(FREETYPE_VERSION)/src/truetype \
    -I $(FREETYPE_VERSION)/src/type1 \
    -I $(FREETYPE_VERSION)/src/type42 \
    -I $(FREETYPE_VERSION)/src/winfonts

FT_BASE:= \
    basepic.c  ftbase.c    ftbitmap.c  ftdbgmem.c  ftgasp.c \
    ftgxval.c  ftotval.c   ftpfr.c     ftsnames.c  ftsynth.c \
    fttype1.c  ftadvanc.c  ftbbox.c    ftcalc.c    ftdebug.c \
    ftgloadr.c ftinit.c    ftmm.c      ftoutln.c   ftpic.c \
    ftstream.c ftsystem.c  ftutil.c    ftapi.c     ftbdf.c \
    ftcid.c    ftfstype.c  ftglyph.c   ftlcdfil.c  ftobjs.c \
    ftpatent.c ftrfork.c   ftstroke.c  fttrigon.c  ftwinfnt.c

LOCAL_SRCS = \
    $(wildcard $(FREETYPE_VERSION)/src/autofit/autofit.c) \
    $(wildcard $(FREETYPE_VERSION)/src/autofit/aflatin2.c) \
    $(foreach file,$(FT_BASE), $(FREETYPE_VERSION)/src/base/$(file)) \
    $(wildcard $(FREETYPE_VERSION)/src/bdf/bdf.c) \
    $(wildcard $(FREETYPE_VERSION)/src/cff/cff.c) \
    $(wildcard $(FREETYPE_VERSION)/src/gxvalid/gxvalid.c) \
    $(wildcard $(FREETYPE_VERSION)/src/otvalid/otvalid.c) \
    $(wildcard $(FREETYPE_VERSION)/src/pcf/pcf.c) \
    $(wildcard $(FREETYPE_VERSION)/src/pfr/pfr.c) \
    $(wildcard $(FREETYPE_VERSION)/src/psaux/psaux.c) \
    $(wildcard $(FREETYPE_VERSION)/src/pshinter/pshinter.c) \
    $(wildcard $(FREETYPE_VERSION)/src/psnames/psnames.c) \
    $(wildcard $(FREETYPE_VERSION)/src/raster/raster.c) \
    $(wildcard $(FREETYPE_VERSION)/src/sfnt/sfnt.c) \
    $(wildcard $(FREETYPE_VERSION)/src/smooth/smooth.c) \
    $(wildcard $(FREETYPE_VERSION)/src/truetype/truetype.c) \
    $(wildcard $(FREETYPE_VERSION)/src/type1/type1.c) \
    $(wildcard $(FREETYPE_VERSION)/src/type42/type42.c) \
    $(wildcard $(FREETYPE_VERSION)/src/cache/*.c) \
    $(wildcard $(FREETYPE_VERSION)/src/cid/*.c) \
    $(wildcard $(FREETYPE_VERSION)/src/gzip/*.c) \
    $(wildcard $(FREETYPE_VERSION)/src/lzw/*.c) \
    $(wildcard $(FREETYPE_VERSION)/src/winfonts/*.c)

LOCAL_CFLAGS := \
    -include $(FREETYPE_VERSION)/include/freetype/config/ftheader.h \
    -include $(FREETYPE_VERSION)/include/freetype/config/ftconfig.h \
    -include $(FREETYPE_VERSION)/include/freetype/config/ftoption.h \
    -include $(FREETYPE_VERSION)/include/freetype/config/ftstdlib.h \
    -include $(FREETYPE_VERSION)/include/freetype/internal/internal.h

LOCAL_CDEFS = -DFT_OPTION_AUTOFIT2 -DFT2_BUILD_LIBRARY -DHAVE_MEMCPY

LOCAL_EXT_FLAG := \
    -Wno-error -Wno-unused-but-set-variable -Wno-implicit-function-declaration \
    -Wno-unused-const-variable -Wno-implicit-function-declaration \
    -Wno-unused-label -Wno-unused-function -Wno-misleading-indentation \
    -Wno-return-type -Wno-unused-variable -Wno-unused-function \
    -fstack-protector-strong

LOCAL_FLAGS := $(LOCAL_CDEFS) $(LOCAL_EXT_FLAG) $(LOCAL_INCLUDE)

include $(MODULE)
