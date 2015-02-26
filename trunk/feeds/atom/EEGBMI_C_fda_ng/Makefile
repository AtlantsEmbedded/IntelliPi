include $(TOPDIR)/rules.mk

PKG_NAME:=EEGBMI_C_fda_ng
PKG_VERSION:=2014-26-14
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=svn
PKG_SOURCE_URL:=http://svn.code.sf.net/p/atomproducts/svn/trunk/src/EEGBMI_C_fda_ng/
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/EEGBMI_C_fda_ng
	SECTION:=libs
	CATEGORY:=Libraries
	TITLE:=EEGBMI_C_fda_ng
	DEPENDS:=+librt
endef

define Package/EEGBMI_C_fda_ng/description
	EEG SVM library
endef

define Package/EEGBMI_C_fda_ng/install
	$(INSTALL_DIR) $(1)/usr
	$(INSTALL_DIR) $(1)/usr/lib
	cp $(PKG_BUILD_DIR)/libeeg_fda.so* $(1)/usr/lib/
endef

define Build/InstallDev 
	$(INSTALL_DIR) $(STAGING_DIR)/usr
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include/
	$(INSTALL_DIR) $(STAGING_DIR)/usr/lib
	cp $(PKG_BUILD_DIR)/libeeg_fda.so* $(STAGING_DIR)/usr/lib/
	cp $(PKG_BUILD_DIR)/*.h $(STAGING_DIR)/usr/include

endef

$(eval $(call BuildPackage,EEGBMI_C_fda_ng))
