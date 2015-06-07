include $(TOPDIR)/rules.mk

PKG_NAME:=AUDIO
PKG_VERSION:=2015-04-26
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=svn
PKG_SOURCE_URL:=http://svn.code.sf.net/p/atomproducts/svn/trunk/src/AUDIO
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/AUDIO
	SECTION:=Interfaces
	CATEGORY:=Atom
	TITLE:=AUDIO
	SUBMENU:=Interfaces
	DEPENDS:=wiringPi
endef

define Package/AUDIO/description
	Super irritating application
endef

define Package/AUDIO/install
	$(INSTALL_DIR) $(1)/atom
	$(INSTALL_DIR) $(1)/atom/data
	$(INSTALL_DIR) $(1)/atom/data/config
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/audio $(1)/atom/
endef

$(eval $(call BuildPackage,AUDIO))
