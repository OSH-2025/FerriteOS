DEMOS_UTILITY_INCLUDE :=

ifeq ($(LOSCFG_DEMOS_ICONV), y)
include $(LITEOSTOPDIR)/demos/utility/iconv/iconv.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_ICONV_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_INIPARSER), y)
include $(LITEOSTOPDIR)/demos/utility/iniparser/iniparser.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_INIPARSER_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_JSON_C), y)
include $(LITEOSTOPDIR)/demos/utility/json-c/json-c.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_JSON_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_BIDIREFERENCE), y)
include $(LITEOSTOPDIR)/demos/utility/bidireference/bidireference.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_BIDIREFERENCE_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_FREETYPE), y)
include $(LITEOSTOPDIR)/demos/utility/freetype/freetype.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_FREETYPE_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_LIBXML2), y)
include $(LITEOSTOPDIR)/demos/utility/libxml2/libxml2.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_LIBXML2_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_CURL), y)
include $(LITEOSTOPDIR)/demos/utility/curl/curl.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_CURL_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_SQLITE), y)
include $(LITEOSTOPDIR)/demos/utility/sqlite/sqlite.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_SQLITE_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_JSONCPP), y)
include $(LITEOSTOPDIR)/demos/utility/jsoncpp/jsoncpp.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_JSONCPP_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_TINYXML2), y)
include $(LITEOSTOPDIR)/demos/utility/tinyxml2/tinyxml2.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_TINYXML2_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_HARFBUZZ), y)
include $(LITEOSTOPDIR)/demos/utility/harfbuzz/harfbuzz.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_HARFBUZZ_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_FASTLZ), y)
include $(LITEOSTOPDIR)/demos/utility/fastlz/fastlz.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_FASTLZ_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_QRENCODE), y)
include $(LITEOSTOPDIR)/demos/utility/qrencode/qrencode.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_QRENCODE_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_PDFGEN), y)
include $(LITEOSTOPDIR)/demos/utility/pdfgen/pdfgen.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_PDFGEN_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_INIH), y)
include $(LITEOSTOPDIR)/demos/utility/inih/inih.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_INIH_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_SFUD), y)
include $(LITEOSTOPDIR)/demos/utility/sfud/sfud.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_SFUD_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_MINMEA), y)
include $(LITEOSTOPDIR)/demos/utility/minmea/minmea.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_MINMEA_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_URIPARSER), y)
include $(LITEOSTOPDIR)/demos/utility/uriparser/uriparser.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_URIPARSER_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_C_ALGORITHMS), y)
include $(LITEOSTOPDIR)/demos/utility/c-algorithms/c-algorithms.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_C_ALGORITHMS_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_JSMN), y)
include $(LITEOSTOPDIR)/demos/utility/jsmn/jsmn.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_JSMN_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_GENANN), y)
include $(LITEOSTOPDIR)/demos/utility/genann/genann.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_GENANN_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_TINYFRAME), y)
include $(LITEOSTOPDIR)/demos/utility/tinyframe/tinyframe.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_TINYFRAME_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_EASYFLASH), y)
include $(LITEOSTOPDIR)/demos/utility/easyflash/easyflash.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_EASYFLASH_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_LIBEVENT), y)
include $(LITEOSTOPDIR)/demos/utility/libevent/libevent.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_LIBEVENT_INCLUDE)
endif

ifeq ($(LOSCFG_DEMOS_APRILTAG), y)
include $(LITEOSTOPDIR)/demos/utility/apriltag/apriltag.mk
DEMOS_UTILITY_INCLUDE += $(DEMOS_APRILTAG_INCLUDE)
endif

