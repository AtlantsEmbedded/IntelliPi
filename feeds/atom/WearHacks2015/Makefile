include $(TOPDIR)/rules.mk

PKG_NAME:=WearHacks2015
PKG_VERSION:=2015-08-12
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=svn
PKG_SOURCE_URL:=http://svn.code.sf.net/p/atomproducts/svn/trunk/src/WearHacks2015
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/WearHacks2015
	SECTION:=Events
	CATEGORY:=Atom
	TITLE:=WearHacks2015
	SUBMENU:=Events
	DEPENDS:=
endef

define Package/WearHacks2015/description
	Wrapper package containing event specific configs,scri[t
endef

define Package/WearHacks2015/install
	$(INSTALL_DIR) $(1)/atom
	$(INSTALL_DIR) $(1)/atom/data
	$(INSTALL_DIR) $(1)/atom/data/config
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/configs/* $(1)/atom/data/config/
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/init.d/
endef

$(eval $(call BuildPackage,WearHacks2015))
