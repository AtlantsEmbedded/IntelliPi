include $(TOPDIR)/rules.mk

PKG_NAME:=AUDIO
PKG_VERSION:=2015-04-26
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=svn
# TODO-REBRANDING: Cference, this needs to be refactored to git and possibly split AUDIO into it's individual project
PKG_SOURCE_URL:=http://svn.code.sf.net/p/atomproducts/svn/trunk/src/AUDIO
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/AUDIO
	SECTION:=Interfaces
	CATEGORY:=Atlants
	TITLE:=AUDIO
	SUBMENU:=Interfaces
	DEPENDS:=wiringPi
endef

define Package/AUDIO/description
	Super irritating application
endef

define Package/AUDIO/install
	$(INSTALL_DIR) $(1)/Atlants
	$(INSTALL_DIR) $(1)/Atlants/data
	$(INSTALL_DIR) $(1)/Atlants/data/config
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/audio $(1)/Atlants/
endef

$(eval $(call BuildPackage,AUDIO))
