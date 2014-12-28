include $(TOPDIR)/rules.mk

PKG_NAME:=libRPIGPIO
PKG_VERSION:=2014-26-14
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=git://github.com/alanbarr/RaspberryPi-GPIO.git
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/libRPIGPIO
	SECTION:=libs
	CATEGORY:=Libraries
	TITLE:=libRPIGPIO
endef

define Package/libRPIGPIO/description
	C GPIO library for the RPI
endef

define Build/Prepare
	git clone git://github.com/alanbarr/RaspberryPi-GPIO.git $(PKG_BUILD_DIR)
	$(call Build/Prepare/Default,)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/src \
		LINUX="$(LINUX_DIR)" \
		CC="$(TARGET_CC) " \
		CCFLAGS:="$(TARGET_CCFLAGS) -Wno-error=implicit-function-declaration -I../include -Iinc" \
		STAGING_DIR="$(STAGING_DIR)" \
		LDFLAGS="$(TARGET_LDFLAGS)"
endef

define Package/libRPIGPIO/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include/
	cp $(PKG_BUILD_DIR)/library/librpigpio.a $(1)/usr/lib
	cp $(PKG_BUILD_DIR)/include/*.h $(1)/usr/include/
endef

define Build/InstallDev 
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include/
	cp $(PKG_BUILD_DIR)/include/*.h $(STAGING_DIR)/usr/include/
endef

$(eval $(call BuildPackage,libRPIGPIO))
