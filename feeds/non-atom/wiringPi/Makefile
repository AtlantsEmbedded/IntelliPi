include $(TOPDIR)/rules.mk

PKG_NAME:=wiringPi
PKG_VERSION:=2014-26-14
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=git://git.drogon.net/wiringPi
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

LDFLAGS:="-L../wiringPi/ -L../devLib/ -L$(STAGING_DIR)/lib -L$(STAGING_DIR)/usr/lib"

include $(INCLUDE_DIR)/package.mk

define Package/wiringPi
	SECTION:=libs
	CATEGORY:=Libraries
	TITLE:=wiringPi
	DEPENDS:=+libpthread +libstdcpp +libc
endef

define Package/wiringPi/description
	C GPIO libraries for the RPI
endef

define Build/Prepare
	git clone git://git.drogon.net/wiringPi $(PKG_BUILD_DIR)
	$(call Build/Prepare/Default,)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/wiringPi/ \
                LINUX="$(LINUX_DIR)" \
                CC="$(TARGET_CC)" \
                STAGING_DIR="$(STAGING_DIR)" \
                LDFLAGS="$(TARGET_LDFLAGS)"
	$(MAKE) -C $(PKG_BUILD_DIR)/devLib/ \
                LINUX="$(LINUX_DIR)" \
                CC="$(TARGET_CC)" \
                STAGING_DIR="$(STAGING_DIR)" \
                LDFLAGS="$(TARGET_LDFLAGS)"
	$(MAKE) -C $(PKG_BUILD_DIR)/gpio/ \
                LINUX="$(LINUX_DIR)" \
                CC="$(TARGET_CC)" \
                STAGING_DIR="$(STAGING_DIR)" \
                LDFLAGS="$(TARGET_LDFLAGS) -L../wiringPi -L../devLib"

endef

define Package/wiringPi/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/bin
	cp $(PKG_BUILD_DIR)/devLib/libwiringPiDev.so $(1)/usr/lib/
	cp $(PKG_BUILD_DIR)/devLib/*.h $(1)/usr/include/
	cp $(PKG_BUILD_DIR)/wiringPi/libwiringPi.so $(1)/usr/lib/
	cp $(PKG_BUILD_DIR)/wiringPi/*.h $(1)/usr/include/
	cp $(PKG_BUILD_DIR)/gpio/gpio $(1)/usr/bin/
endef

define Build/InstallDev 
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include/
	$(INSTALL_DIR) $(STAGING_DIR)/usr/lib
	cp $(PKG_BUILD_DIR)/devLib/libwiringPiDev.so* $(STAGING_DIR)/usr/lib/
	cp $(PKG_BUILD_DIR)/devLib/*.h $(STAGING_DIR)/usr/include/
	cp $(PKG_BUILD_DIR)/wiringPi/libwiringPi.so* $(STAGING_DIR)/usr/lib/
	cp $(PKG_BUILD_DIR)/wiringPi/*.h $(STAGING_DIR)/usr/include/
endef

$(eval $(call BuildPackage,wiringPi))
