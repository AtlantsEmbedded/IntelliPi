include $(TOPDIR)/rules.mk

PKG_NAME:=bluez
PKG_VERSION:=2015-30-07
PKG_RELEASE:=1
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=git://git.kernel.org/pub/scm/bluetooth/bluez.git
PKG_SOURCE_VERSION:=HEAD
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/bluez
	SECTION:=libs
	CATEGORY:=Atom-GPL
	TITLE:=bluez
	SUBMENU:=Libraries
	DEPENDS:=+libpthread
endef

define Package/bluez/description
	bluetooth package
endef

define Build/Prepare
	git clone $(PKG_SOURCE_URL) $(PKG_BUILD_DIR)
	$(call Build/Prepare/Default,)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)/bluez \
	LINUX="$(LINUX_DIR)" \
	CC="$(TARGET_CC)" \
	STAGING_DIR="$(STAGING_DIR)" \
	LDFLAGS="$(TARGET_LDFLAGS)"
	$(MAKE) -C $(PKG_BUILD_DIR)/devLib \
	LINUX="$(LINUX_DIR)" \
	CC="$(TARGET_CC)" \
	CFLAGS="$(TARGET_CFLAGS) -I$(PKG_BUILD_DIR)" \
	STAGING_DIR="$(STAGING_DIR)" \
	LDFLAGS="$(TARGET_LDFLAGS) -L$(PKG_BUILD_DIR) -L$(PKG_BUILD_DIR)/devLib -L$(STAGING_DIR)/lib -L$(STAGING_DIR)/usr/lib"
endef

define Package/wiringPi/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_DIR) $(1)/usr/include
	$(INSTALL_DIR) $(1)/usr/bin
	cp $(PKG_BUILD_DIR)/devLib/libbluez.so.* $(1)/usr/lib/libbluez.so
	cp $(PKG_BUILD_DIR)/devLib/*.h $(1)/usr/include/
	cp $(PKG_BUILD_DIR)/bluez/libbluez.so.* $(1)/usr/lib/libbluez.so
	cp $(PKG_BUILD_DIR)/bluez/*.h $(1)/usr/include/
endef

define Build/InstallDev 
	$(INSTALL_DIR) $(STAGING_DIR)/usr/include/
	$(INSTALL_DIR) $(STAGING_DIR)/usr/lib
	cp $(PKG_BUILD_DIR)/devLib/libbluezDev.so* $(STAGING_DIR)/usr/lib/libbluezDev.so
	cp $(PKG_BUILD_DIR)/devLib/*.h $(STAGING_DIR)/usr/include/
	cp $(PKG_BUILD_DIR)/bluez/libbluez.so* $(STAGING_DIR)/usr/lib/libbluez.so
	cp $(PKG_BUILD_DIR)/bluez/*.h $(STAGING_DIR)/usr/include/
endef

$(eval $(call BuildPackage,bluez))
