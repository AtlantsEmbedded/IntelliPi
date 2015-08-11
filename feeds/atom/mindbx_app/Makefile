include $(TOPDIR)/rules.mk

PKG_NAME:=mindbx_app
PKG_VERSION:=2015-08-12
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=svn
PKG_SOURCE_URL:=http://svn.code.sf.net/p/atomproducts/svn/trunk/src/mindbox_app
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/mindbx_app
	SECTION:=Applications
	CATEGORY:=Atom
	TITLE:=mindbx_app
	SUBMENU:=Interfaces
	DEPENDS:=+wiringPi +libpthread +libezxml +glib2
endef

define Package/mindbx_app/description
	Application which runs the mindbx device
endef

define Package/mindbx_app/install
	$(INSTALL_DIR) $(1)/atom
	$(INSTALL_DIR) $(1)/atom/app
	$(INSTALL_DIR) $(1)/atom/app/config
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mindbox_app $(1)/atom/app
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/scripts/launch_mindbx.sh $(1)/atom/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/config/application_config.xml $(1)/atom/app/config
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/init.d
endef

$(eval $(call BuildPackage,mindbx_app))
