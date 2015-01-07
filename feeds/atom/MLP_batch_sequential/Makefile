include $(TOPDIR)/rules.mk

PKG_NAME:=MLP_batch_sequential
PKG_VERSION:=2014-26-14
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=svn
PKG_SOURCE_URL:=http://svn.code.sf.net/p/atomproducts/svn/trunk/src/MLP_batch_sequential
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/MLP_batch_sequential
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=MLP_batch_sequential
	DEPENDS:=+libpthread +librt
endef

define Package/MLP_batch_sequential/description
	Application which tests MLP batch sequential processing \
	NOTE: libm is provided by base-files
endef

define Build/Configure
	$(call Build/Configure/Default,)
endef

define Package/MLP_batch_sequential/install
	$(INSTALL_DIR) $(1)/atom
	$(INSTALL_DIR) $(1)/atom/MLP_batch_sequential/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/MLP_batch $(1)/atom/MLP_batch_sequential/
	$(INSTALL_DIR) $(1)/atom/MLP_batch_sequential/data
	$(CP) $(PKG_BUILD_DIR)/data/* $(1)/atom/MLP_batch_sequential/data
endef

$(eval $(call BuildPackage,MLP_batch_sequential))
