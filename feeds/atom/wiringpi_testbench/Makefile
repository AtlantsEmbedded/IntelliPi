include $(TOPDIR)/rules.mk

PKG_NAME:=wiringpi_testbench
PKG_VERSION:=2015-08-16
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=svn
# TODO-REBRANDING: Cference, this might need to be refactored into it's own project, wiringpi_testbench
PKG_SOURCE_URL:=http://svn.code.sf.net/p/atomproducts/svn/trunk/src/wiringpi_testbench
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/wiringpi_testbench
	SECTION:=Interfaces
	CATEGORY:=Atlants
	TITLE:=wiringpi_testbench
	SUBMENU:=Interfaces
	DEPENDS:=wiringPi +libpthread +libezxml +glib2
endef

define Package/wiringpi_testbench/description
	Application to test gpio functionnalities
endef

define Package/wiringpi_testbench/install
	$(INSTALL_DIR) $(1)/Atlants
	$(INSTALL_DIR) $(1)/Atlants/gpio
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wiringpi_testbench $(1)/Atlants/gpio
endef

$(eval $(call BuildPackage,wiringpi_testbench))
