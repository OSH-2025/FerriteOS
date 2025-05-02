COMPONENTS_UTILITY_INCLUDE :=

ifeq ($(LOSCFG_COMPONENTS_ICONV), y)
include $(LITEOSTOPDIR)/components/utility/iconv/iconv.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_ICONV_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_INIPARSER), y)
include $(LITEOSTOPDIR)/components/utility/iniparser/iniparser.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_INIPARSER_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_JSON_C), y)
include $(LITEOSTOPDIR)/components/utility/json-c/json-c.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_JSON_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_BIDIREFERENCE), y)
include $(LITEOSTOPDIR)/components/utility/bidireference/bidireference.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_BIDIREFERENCE_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_FREETYPE), y)
include $(LITEOSTOPDIR)/components/utility/freetype/freetype.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_FREETYPE_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_LIBXML2), y)
include $(LITEOSTOPDIR)/components/utility/libxml2/libxml2.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_LIBXML2_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_CURL), y)
include $(LITEOSTOPDIR)/components/utility/curl/curl.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_CURL_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_SQLITE), y)
include $(LITEOSTOPDIR)/components/utility/sqlite/sqlite.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_SQLITE_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_JSONCPP), y)
include $(LITEOSTOPDIR)/components/utility/jsoncpp/jsoncpp.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_JSONCPP_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_THTTPD), y)
include $(LITEOSTOPDIR)/components/utility/thttpd/thttpd.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_THTTPD_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_TINYXML2), y)
include $(LITEOSTOPDIR)/components/utility/tinyxml2/tinyxml2.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_TINYXML2_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_HARFBUZZ), y)
include $(LITEOSTOPDIR)/components/utility/harfbuzz/harfbuzz.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_HARFBUZZ_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_FASTLZ), y)
include $(LITEOSTOPDIR)/components/utility/fastlz/fastlz.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_FASTLZ_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_PDFGEN), y)
include $(LITEOSTOPDIR)/components/utility/pdfgen/pdfgen.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_PDFGEN_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_QRENCODE), y)
include $(LITEOSTOPDIR)/components/utility/qrencode/qrencode.mk
COMPONENTS_MEDIA_INCLUDE += $(COMPONENTS_QRENCODE_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_INIH), y)
include $(LITEOSTOPDIR)/components/utility/inih/inih.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_INIH_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_SFUD), y)
include $(LITEOSTOPDIR)/components/utility/sfud/sfud.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_SFUD_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_URIPARSER), y)
include $(LITEOSTOPDIR)/components/utility/uriparser/uriparser.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_URIPARSER_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_C_ALGORITHMS), y)
include $(LITEOSTOPDIR)/components/utility/c-algorithms/c-algorithms.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_C_ALGORITHMS_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_JSMN), y)
include $(LITEOSTOPDIR)/components/utility/jsmn/jsmn.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_JSMN_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_TINYFRAME), y)
include $(LITEOSTOPDIR)/components/utility/tinyframe/tinyframe.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_TINYFRAME_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_MINMEA), y)
include $(LITEOSTOPDIR)/components/utility/minmea/minmea.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_MINMEA_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_REGEX), y)
include $(LITEOSTOPDIR)/components/utility/regex/regex.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_REGEX_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_GENANN), y)
include $(LITEOSTOPDIR)/components/utility/genann/genann.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_GENANN_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_EASYFLASH), y)
include $(LITEOSTOPDIR)/components/utility/easyflash/easyflash.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_EASYFLASH_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_LIBEVENT), y)
include $(LITEOSTOPDIR)/components/utility/libevent/libevent.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_LIBEVENT_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_APRILTAG), y)
include $(LITEOSTOPDIR)/components/utility/apriltag/apriltag.mk
COMPONENTS_UTILITY_INCLUDE += $(COMPONENTS_APRILTAG_INCLUDE)
endif

