include $(TOPDIR)/rules.mk

PKG_NAME:=AUDIO
PKG_VERSION:=2015-04-26
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/AtlantsEmbedded/feed-atlants-AUDIO.git
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
	$(INSTALL_DIR) $(1)/intelli
	$(INSTALL_DIR) $(1)/intelli/data
	$(INSTALL_DIR) $(1)/intelli/data/config
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/audio $(1)/intelli/
endef

$(eval $(call BuildPackage,AUDIO))
