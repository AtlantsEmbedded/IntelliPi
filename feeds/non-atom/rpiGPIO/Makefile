include $(TOPDIR)/rules.mk

PKG_NAME:=Rpi.GPIO
PKG_VERSION:=1.0.6
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/Rpi.GPIO-utils-$(PKG_VERSION)
PKG_SOURCE_PROTO:=hr
PKG_SOURCE_URL:=http://hg.code.sf.net/p/raspberry-gpio-python/code
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk

define Package/Rpi.GPIO
  SUBMENU:=Python
  SECTION:=lang
  CATEGORY:=Languages
  TITLE:=Python RPI
  URL:=https://sourceforge.net/p/raspberry-gpio-python
  DEPENDS:=python
endef

define Package/Rpi.GPIO/description
 Python GPIO library
endef

define Package/Rpi.GPIO/install
        $(INSTALL_DIR) $(1)/usr/lib/
        $(CP) $(PKG_BUILD_DIR)/_install_tmp/*
endef

$(eval $(call BuildPackage,Rpi.GPIO))
