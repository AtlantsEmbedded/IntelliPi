include $(TOPDIR)/rules.mk

PKG_VERSION:=2014-26-14
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=git://github.com/alanbarr/RaspberryPi-GPIO.git
PKG_SOURCE:=$(PKG_NAME).tar.gz
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE_SUBDIR:=$(PKG_NAME)/src

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

define Package/libRPIGPIO
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=libRPIGPIO
endef

define Package/libRPIGPIO/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include/
	cp $(PKG_BUILD_DIR)library/librpigpio.a $(1)/usr/lib
	cp $(PKG_BUILD_DIR)/include/*.h $(1)/usr/include/
endef

define Build/InstallDev 
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include/
	cp $(PKG_BUILD_DIR)/include/*.h $(STAGING_DIR)/usr/include/
endef

$(eval $(call BuildPackage,libRPIGPIO))
