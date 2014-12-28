include $(TOPDIR)/rules.mk

PKG_NAME:=rpiLCDApp
PKG_VERSION:=2014-26-14
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/fangfufu/Adafruit-RPi-LCD.git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/rpiLCDApp
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=rpiLCDApp
endef

define Package/rpiLCDApp/description
	LCD echoer app
endef

define Build/Prepare
  git clone https://github.com/fangfufu/Adafruit-RPi-LCD.git $(PKG_BUILD_DIR)
endef

define Build/Compile
    $(MAKE) -C $(PKG_BUILD_DIR) \
		LINUX="$(LINUX_DIR)" \
		CC="$(TARGET_CC)" \
		STAGING_DIR="$(STAGING_DIR)" \
		LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Package/rpiLCDApp/install
	$(INSTALL_DIR) $(1)/usr/bin
	cp $(PKG_BUILD_DIR)/adafruit-rpi-lcd $(1)/usr/bin/
endef

$(eval $(call BuildPackage,rpiLCDApp))
