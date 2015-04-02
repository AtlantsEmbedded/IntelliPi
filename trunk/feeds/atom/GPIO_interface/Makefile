include $(TOPDIR)/rules.mk

PKG_NAME:=GPIO_interface
PKG_VERSION:=2014-26-14
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=svn
PKG_SOURCE_URL:=http://svn.code.sf.net/p/atomproducts/svn/trunk/src/GPIO_interface
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/GPIO_interface
	SECTION:=Interfaces
	CATEGORY:=Atom
	TITLE:=GPIO_interface
	SUBMENU:=Interfaces
	DEPENDS:=wiringPi +libpthread +libezxml +glib2
endef

define Package/GPIO_interface/description
	Application which will act as both a server for the LCD \
	and for button presses.
endef

define Package/GPIO_interface/install
	$(INSTALL_DIR) $(1)/atom
	$(INSTALL_DIR) $(1)/atom/gpio
	$(INSTALL_DIR) $(1)/atom/gpio/config
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/gpio-daemon/config/gpio_config.xml $(1)/atom/gpio/config
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/gpio-daemon/gpio_interface $(1)/atom/gpio
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/gpio-ipc-client/gpio_ipc_client $(1)/atom/gpio
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/scripts/gpio $(1)/etc/init.d/
endef

$(eval $(call BuildPackage,GPIO_interface))
