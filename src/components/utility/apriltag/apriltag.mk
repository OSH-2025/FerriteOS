APRILTAG_VERSION = apriltag-3.1.7
COMPONENTS_APRILTAG_INCLUDE += \
    -I $(LITEOSTOPDIR)/components/utility/apriltag/port \
    -I $(LITEOSTOPDIR)/components/utility/apriltag/$(APRILTAG_VERSION) \
    -I $(LITEOSTOPDIR)/components/utility/apriltag/$(APRILTAG_VERSION)/common
