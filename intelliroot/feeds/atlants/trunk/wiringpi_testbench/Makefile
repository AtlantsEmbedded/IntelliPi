include $(TOPDIR)/rules.mk

PKG_NAME:=wiringpi_testbench
PKG_VERSION:=2015-08-16
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=svn
PKG_SOURCE_URL:=https://github.com/AtlantsEmbedded/feed-atlants-wiringpi_testbench.git
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/wiringpi_testbench
	SECTION:=Applications
	CATEGORY:=Atlants
	TITLE:=wiringpi_testbench
	SUBMENU:=Applications
	DEPENDS:=wiringPi +libpthread +libezxml +glib2
endef

define Package/wiringpi_testbench/description
	Application to test gpio functionnalities
endef

define Package/wiringpi_testbench/install
	$(INSTALL_DIR) $(1)/intelli
	$(INSTALL_DIR) $(1)/intelli/gpio
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wiringpi_testbench $(1)/intelli/gpio
endef

$(eval $(call BuildPackage,wiringpi_testbench))
