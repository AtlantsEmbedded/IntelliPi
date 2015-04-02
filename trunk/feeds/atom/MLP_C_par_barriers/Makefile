include $(TOPDIR)/rules.mk

PKG_NAME:=MLP_C_par_barriers
PKG_VERSION:=2014-26-14
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=svn
PKG_SOURCE_URL:=http://svn.code.sf.net/p/atomproducts/svn/trunk/src/MLP_C_par_barriers
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

PKG_FIXUP:=autoreconf

define Package/MLP_C_par_barriers
	SECTION:=libraries
	CATEGORY:=Atom
	SUBMENU:=Machine Learning Libraries
	TITLE:=MLP_C_par_barriers
	DEPENDS:=+libpthread +librt
endef

define Package/MLP_C_par_barriers/description
	Application which tests MLP batch sequential processing \
	NOTE: libm is provided by base-files
endef

# This is a bit messy, but this can be cleaned up later
define Build/Configure
	( cd $(PKG_BUILD_DIR); \
        chmod a+x autogen.sh; \
        ./autogen.sh; \
        chmod a+x configure; \
        ./configure; \
        );
endef

define Package/MLP_C_par_barriers/install
	$(INSTALL_DIR) $(1)/atom
	$(INSTALL_DIR) $(1)/atom/MLP_C_par_barriers/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/MLP_barrier_ex $(1)/atom/MLP_C_par_barriers/
	$(INSTALL_DIR) $(1)/atom/MLP_C_par_barriers/data
	$(CP) $(PKG_BUILD_DIR)/data/* $(1)/atom/MLP_C_par_barriers/data
endef

$(eval $(call BuildPackage,MLP_C_par_barriers))
