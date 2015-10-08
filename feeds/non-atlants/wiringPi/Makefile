include $(TOPDIR)/rules.mk

PKG_NAME:=wiringPi
PKG_VERSION:=2015-09-01
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=svn
# TODO-REBRANDING: This might need to be refactored into it's own project, wiringPi
PKG_SOURCE_URL:=http://svn.code.sf.net/p/atomproducts/svn/trunk/src/wiringPi
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/wiringPi
	SECTION:=libs
	CATEGORY:=Atlants-GPL
	TITLE:=wiringPi
	SUBMENU:=Libraries
	DEPENDS:=+libpthread
endef

define Package/wiringPi/description
	C GPIO libraries for the RPI
endef


define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/wiringPi \
	LINUX="$(LINUX_DIR)" \
	CC="$(TARGET_CC)" \
	STAGING_DIR="$(STAGING_DIR)" \
	LDFLAGS="$(TARGET_LDFLAGS)"
	$(MAKE) -C $(PKG_BUILD_DIR)/devLib \
	LINUX="$(LINUX_DIR)" \
	CC="$(TARGET_CC)" \
	CFLAGS="$(TARGET_CFLAGS) -I$(PKG_BUILD_DIR)/wiringPi" \
	STAGING_DIR="$(STAGING_DIR)" \
	LDFLAGS="$(TARGET_LDFLAGS) -L$(PKG_BUILD_DIR)/wiringPi -L$(PKG_BUILD_DIR)/devLib -L$(STAGING_DIR)/lib -L$(STAGING_DIR)/usr/lib"
endef

define Package/wiringPi/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/bin
	cp $(PKG_BUILD_DIR)/devLib/libwiringPiDev.so.* $(1)/usr/lib/libwiringPiDev.so
	cp $(PKG_BUILD_DIR)/devLib/*.h $(1)/usr/include/
	cp $(PKG_BUILD_DIR)/wiringPi/libwiringPi.so.* $(1)/usr/lib/libwiringPi.so
	cp $(PKG_BUILD_DIR)/wiringPi/*.h $(1)/usr/include/
endef

define Build/InstallDev
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include/
	$(INSTALL_DIR) $(STAGING_DIR)/usr/lib
	cp $(PKG_BUILD_DIR)/devLib/libwiringPiDev.so* $(STAGING_DIR)/usr/lib/libwiringPiDev.so
	cp $(PKG_BUILD_DIR)/devLib/*.h $(STAGING_DIR)/usr/include/
	cp $(PKG_BUILD_DIR)/wiringPi/libwiringPi.so* $(STAGING_DIR)/usr/lib/libwiringPi.so
	cp $(PKG_BUILD_DIR)/wiringPi/*.h $(STAGING_DIR)/usr/include/
endef

$(eval $(call BuildPackage,wiringPi))
