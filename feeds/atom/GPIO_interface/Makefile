include $(TOPDIR)/rules.mk

PKG_NAME:=GPIO_interface
PKG_VERSION:=2014-26-14
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=svn
PKG_SOURCE_URL:=svn://svn.code.sf.net/p/atomproducts/svn/trunk/src/GPIO_interface
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/GPIO_interface
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=GPIO_interface
	DEPENDS:=wiringPi
endef

define Package/GPIO_interface/description
	Application which will act as both a server for the LCD \
	and for button presses.
endef

define Package/GPIO_interface/install
	$(INSTALL_DIR) $(1)/atom
	$(INSTALL_DIR) $(1)/atom/bin
endef

$(eval $(call BuildPackage,GPIO_interface))
