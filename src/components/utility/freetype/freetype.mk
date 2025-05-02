FREETYPE_VERSION = freetype-2.4.0

COMPONENTS_FREETYPE_INCLUDE = \
    -I $(LITEOSTOPDIR)/components/utility/freetype/$(FREETYPE_VERSION)/include \
    -I $(LITEOSTOPDIR)/components/utility/freetype/$(FREETYPE_VERSION)/include/freetype/internal \
    -I $(LITEOSTOPDIR)/components/utility/freetype/$(FREETYPE_VERSION)/src/cff \
    -I $(LITEOSTOPDIR)/components/utility/freetype/$(FREETYPE_VERSION)/src/type1 \
    -I $(LITEOSTOPDIR)/components/utility/freetype/$(FREETYPE_VERSION)/src/cid

