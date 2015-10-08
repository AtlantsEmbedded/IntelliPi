include $(TOPDIR)/rules.mk

PKG_NAME:=DHT22_interface
PKG_VERSION:=2014-26-14
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=svn
# TODO-REBRANDING: This might need to be refactored into it's own project, DHT22_sensor
PKG_SOURCE_URL:=http://svn.code.sf.net/p/atomproducts/svn/trunk/src/DHT22_sensor
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/DHT22_interface
	SECTION:=utils
	CATEGORY:=Atlants-GPL
	TITLE:=DHT22_interface
	SUBMENU:=Utilities
	DEPENDS:=wiringPi
endef

define Package/DHT22_interface/description
	Application which will read in the sensor data from a DHT22 sensor \
	or rather the AM2302, which can monitor humidity and temperature data \
	purchasable from here http://www.adafruit.com/products/393
endef

define Package/DHT22_interface/install
	$(INSTALL_DIR) $(1)/usr
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dht22_interface $(1)/usr/bin
endef

$(eval $(call BuildPackage,DHT22_interface))
