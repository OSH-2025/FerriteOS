COMPONENTS_NET_INCLUDE :=

ifeq ($(LOSCFG_COMPONENTS_NET_LWIP), y)
include $(LITEOSTOPDIR)/components/net/lwip/lwip.mk
COMPONENTS_NET_INCLUDE += $(LWIP_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_NET_AT), y)
include $(LITEOSTOPDIR)/components/net/net_at/net_at.mk
COMPONENTS_NET_INCLUDE += $(COMPONENTS_AT_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_NET_SAL), y)
include $(LITEOSTOPDIR)/components/net/sal/atiny_sal.mk
COMPONENTS_NET_INCLUDE += $(COMPONENTS_ATINY_SAL_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_IPERF), y)
include $(LITEOSTOPDIR)/components/net/los_iperf/iperf.mk
COMPONENTS_NET_INCLUDE += $(COMPONENTS_IPERF_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_TFTP_SERVER), y)
include $(LITEOSTOPDIR)/components/net/tftp_server/tftp_server.mk
COMPONENTS_NET_INCLUDE += $(COMPONENTS_TFTP_SERVER_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_PCAP), y)
include $(LITEOSTOPDIR)/components/net/pcap/pcap.mk
COMPONENTS_NET_INCLUDE += $(COMPONENTS_PCAP_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_LIBRWS), y)
include $(LITEOSTOPDIR)/components/net/librws/librws.mk
COMPONENTS_NET_INCLUDE += $(COMPONENTS_LIBRWS_INCLUDE)
endif

ifeq ($(LOSCFG_COMPONENTS_HTTP_PARSER), y)
include $(LITEOSTOPDIR)/components/net/http_parser/http_parser.mk
COMPONENTS_NET_INCLUDE += $(COMPONENTS_HTTP_PARSER_INCLUDE)
endif
