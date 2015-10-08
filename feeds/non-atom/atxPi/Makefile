include $(TOPDIR)/rules.mk

PKG_NAME:=atxPi
PKG_VERSION:=2014-26-14
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/LowPowerLab/ATX-Raspi/
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/atxPi
	SECTION:=utils
	CATEGORY:=Atlants-GPL
	TITLE:=atxPi
	SUBMENU:=Utilities
	DEPENDS:=wiringPi
endef

define Package/atxPi/description
	ATXRaspi is a smart power controller for RaspberryPi that allows \
	you to have an external ATX style shutdown button. \
	http://lowpowerlab.com/atxraspi/
endef

define Build/Configure
endef

define Build/Compile
endef

define Package/atxPi/install
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/shutdowncheck $(1)/etc/
	$(INSTALL_BIN) ./files/etc/init.d/atxPi $(1)/etc/init.d
endef

$(eval $(call BuildPackage,atxPi))
