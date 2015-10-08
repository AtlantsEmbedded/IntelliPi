include $(TOPDIR)/rules.mk

PKG_NAME:=mindbx_testbench
PKG_VERSION:=2015-09-01
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=svn
# TODO-REBRANDING: Cference, this might need to be refactored into it's own project mindbx_testbench
PKG_SOURCE_URL:=http://svn.code.sf.net/p/atomproducts/svn/trunk/src/mindbx_testbench
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/mindbx_testbench
	SECTION:=Interfaces
	CATEGORY:=Atlants
	TITLE:=mindbx_testbench
	SUBMENU:=Interfaces
	DEPENDS:=wiringPi +libpthread +libezxml +glib2
endef

define Package/mindbx_testbench/description
	Application which test the GPIOs system that will be used with the mindbx
endef

define Package/mindbx_testbench/install
	$(INSTALL_DIR) $(1)/Atlants
	$(INSTALL_DIR) $(1)/Atlants/gpio
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/mindbx_testbench $(1)/Atlants/gpio
endef

$(eval $(call BuildPackage,mindbx_testbench))
