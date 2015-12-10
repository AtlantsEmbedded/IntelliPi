include $(TOPDIR)/rules.mk

PKG_NAME:=DATA_preprocessing
PKG_VERSION:=2015-08-12
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/AtlantsEmbedded/DATA_preprocessing.git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/DATA_preprocessing
	SECTION:=Preprocessing
	CATEGORY:=Atlants
	TITLE:=DATA_preprocessing
	SUBMENU:=Preprocessing
	DEPENDS:=+libpthread +libezxml +glib2 +wiringPi
endef

define Package/DATA_preprocessing/description
	Application which applies the preprocessing stage \
 on data received through the DATA_interface
endef

define Package/DATA_preprocessing/install
	$(INSTALL_DIR) $(1)/intelli
	$(INSTALL_DIR) $(1)/intelli/data
	$(INSTALL_DIR) $(1)/intelli/data/config
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/preprocess-daemon/data_preprocessing $(1)/intelli/data
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/preprocess-daemon/config/* $(1)/intelli/data/config
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/init.d/
endef

$(eval $(call BuildPackage,DATA_preprocessing))
