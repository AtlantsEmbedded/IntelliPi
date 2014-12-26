include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/package.mk

PKG_VERSION:=2014-26-14
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/fangfufu/Adafruit-RPi-LCD.git
PKG_SOURCE:=$(PKG_NAME).tar.gz
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE_SUBDIR:=$(PKG_NAME)

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

define Package/rpiLCDApp
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=rpiLCDApp
endef

define Package/rpiLCDApp/description
	LCD echoer app
endef

define Package/rpiLCDApp/install
	$(INSTALL_DIR) $(1)/usr/bin
	cp $(PKG_BUILD_DIR)/adafruit-rpi-lcd $(1)/usr/bin/
endef

$(eval $(call BuildPackage,rpiLCDApp))
