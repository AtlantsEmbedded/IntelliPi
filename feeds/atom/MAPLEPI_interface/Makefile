include $(TOPDIR)/rules.mk

PKG_NAME:=MAPLEPI_interface
PKG_VERSION:=2014-26-14
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=svn
PKG_SOURCE_URL:=http://svn.code.sf.net/p/atomproducts/svn/trunk/src/MAPLEPI_interface
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/MAPLEPI_interface
	SECTION:=system
	CATEGORY:=Atom
	SUBMENU:=Interfaces
	TITLE:=MAPLEPI_interface
	DEPENDS:=wiringPi
endef

define Package/MAPLEPI_interface/description
	Application which will act as both a server for the LCD \
	and for button presses.
endef

define Package/MAPLEPI_interface/install
	$(INSTALL_DIR) $(1)/atom
	$(INSTALL_DIR) $(1)/atom/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/gpio_interface $(1)/atom/bin
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/init.d/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/scripts/gpio $(1)/etc/init.d/
endef

$(eval $(call BuildPackage,MAPLEPI_interface))
