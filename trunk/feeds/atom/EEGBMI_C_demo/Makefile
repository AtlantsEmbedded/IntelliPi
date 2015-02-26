include $(TOPDIR)/rules.mk

PKG_NAME:=EEGBMI_C_demo
PKG_VERSION:=2014-26-14
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=svn
PKG_SOURCE_URL:=http://svn.code.sf.net/p/atomproducts/svn/trunk/src/EEGBMI_C_main
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/EEGBMI_C_demo
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=EEGBMI_C_demo
	DEPENDS:=+libpthread +EEGBMI_C_fda_ng +EEGBMI_C_fft +EEGBMI_C_svm
endef

define Package/EEGBMI_C_demo/description
	Application which will host the EEG BMI demo albeit eigin library
endef

define Package/EEGBMI_C_demo/install
	$(INSTALL_DIR) $(1)/atom
	$(INSTALL_DIR) $(1)/atom/demo
	$(INSTALL_DIR) $(1)/atom/demo/config_files
	$(INSTALL_DIR) $(1)/atom/demo/acii_files_3
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/EEGBMI.exe $(1)/atom/demo/eegbmi
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/ascii_files_3/* $(1)/atom/demo/acii_files_3/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/config_files/* $(1)/atom/demo/config_files/
endef

$(eval $(call BuildPackage,EEGBMI_C_demo))
