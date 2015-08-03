include $(TOPDIR)/rules.mk

PKG_NAME:=DATA_preprocessing
PKG_VERSION:=2015-07-28
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=svn
PKG_SOURCE_URL:=http://svn.code.sf.net/p/atomproducts/svn/trunk/src/DATA_preprocessing
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/DATA_preprocessing
	SECTION:=Analysis
	CATEGORY:=Atom
	TITLE:=DATA_preprocessing
	SUBMENU:=preprocessing
	DEPENDS:=+libpthread +libezxml +glib2
endef

define Package/DATA_preprocessing/description
	Application which applies the preprocessing stage \
 on data received through the DATA_interface
endef

define Package/DATA_preprocessing/install
	$(INSTALL_DIR) $(1)/atom
	$(INSTALL_DIR) $(1)/atom/data
	$(INSTALL_DIR) $(1)/atom/data/config
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/preprocess-daemon/data_preprocessing $(1)/atom/data
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/init.d/
endef

$(eval $(call BuildPackage,DATA_preprocessing))
