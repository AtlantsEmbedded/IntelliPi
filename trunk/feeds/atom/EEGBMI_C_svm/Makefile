include $(TOPDIR)/rules.mk

PKG_NAME:=EEGBMI_C_svm
PKG_VERSION:=2014-26-14
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=svn
PKG_SOURCE_URL:=http://svn.code.sf.net/p/atomproducts/svn/trunk/src/EEGBMI_C_svm
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/EEGBMI_C_svm
	SECTION:=libs
	CATEGORY:=Libraries
	TITLE:=EEGBMI_C_svm
	DEPENDS:=+librt
endef

define Package/EEGBMI_C_svm/description
	EEG SVM library
endef

define Package/EEGBMI_C_svm/install
	$(INSTALL_DIR) $(1)/usr
	$(INSTALL_DIR) $(1)/usr/lib
	#cp $(PKG_BUILD_DIR)/EEGBMI_C_svm/libEEGBMI_C_svm.so $(1)/usr/lib/libEEGBMI_C_svm.so
endef

define Build/InstallDev 
	$(INSTALL_DIR) $(STAGING_DIR)/usr
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include/
	$(INSTALL_DIR) $(STAGING_DIR)/usr/lib
endef

$(eval $(call BuildPackage,EEGBMI_C_svm))
