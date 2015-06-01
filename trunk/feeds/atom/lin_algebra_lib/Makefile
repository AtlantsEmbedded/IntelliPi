include $(TOPDIR)/rules.mk

PKG_NAME:=lin_algebra_lib
PKG_VERSION:=2015-28-05
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=svn
PKG_SOURCE_URL:=http://svn.code.sf.net/p/atomproducts/svn/trunk/src/lin_algebra_lib/
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/lin_algebra_lib
	SECTION:=libs
	CATEGORY:=Libraries
	TITLE:=lin_algebra_lib
	DEPENDS:=+librt
endef

define Package/lin_algebra_lib/description
	Linear algebra library
endef

define Package/lin_algebra_lib/install
	$(INSTALL_DIR) $(1)/usr
	$(INSTALL_DIR) $(1)/usr/lib
	cp $(PKG_BUILD_DIR)/libf2c.so* $(1)/usr/lib/
	cp $(PKG_BUILD_DIR)/libstegr.so* $(1)/usr/lib/
	cp $(PKG_BUILD_DIR)/liblin_algebra_lib.so* $(1)/usr/lib/
endef

define Build/InstallDev 
	$(INSTALL_DIR) $(STAGING_DIR)/usr
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include/
	$(INSTALL_DIR) $(STAGING_DIR)/usr/lib
	cp $(PKG_BUILD_DIR)/f2clib/libf2clib.so* $(STAGING_DIR)/usr/lib/
	cp $(PKG_BUILD_DIR)/stegr/libstegr.so* $(STAGING_DIR)/usr/lib/
	cp $(PKG_BUILD_DIR)/liblin_algebra_lib.so* $(STAGING_DIR)/usr/lib/
	cp $(PKG_BUILD_DIR)/*.h $(STAGING_DIR)/usr/include

endef

$(eval $(call BuildPackage,lin_algebra_lib))
