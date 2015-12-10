include $(TOPDIR)/rules.mk

PKG_NAME:=HARDWARE_interface
PKG_VERSION:=2015-10-03
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/AtlantsEmbedded/feed-atlants-HARDWARE_interface.git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/HARDWARE_interface
	SECTION:=Interfaces
	CATEGORY:=Atlants
	TITLE:=HARDWARE_interface
	SUBMENU:=Interfaces
	DEPENDS:=+wiringPi +libpthread +libezxml +glib2 +bluez-libs
endef

define Package/HARDWARE_interface/description
	Application which will act as the controller and manager interfaces\
 which will allow the user to control the hardware.
endef

define Package/HARDWARE_interface/install
	$(INSTALL_DIR) $(1)/intelli
	$(INSTALL_DIR) $(1)/intelli/data
	$(INSTALL_DIR) $(1)/intelli/data/config
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/hardware-daemon/config/data_config.xml $(1)/intelli/data/config
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/hardware-daemon/hardware_interface $(1)/intelli/data
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DIR) $(1)/etc/init.d/
endef

$(eval $(call BuildPackage,HARDWARE_interface))
