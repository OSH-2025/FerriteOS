CURL_VERSION=curl-7.54.1

COMPONENTS_CURL_INCLUDE := \
    -I $(LITEOSTOPDIR)/components/utility/curl/$(CURL_VERSION)/include \
    -I $(LITEOSTOPDIR)/components/utility/curl/$(CURL_VERSION)/src \
    -I $(LITEOSTOPDIR)/components/utility/curl