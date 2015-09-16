#
# Copyright (C) 2006-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=bluez
PKG_VERSION:=4.101
PKG_RELEASE:=4

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://www.kernel.org/pub/linux/bluetooth/
PKG_MD5SUM:=fb42cb7038c380eb0e2fa208987c96ad

PKG_FIXUP:=autoreconf
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/bluez
	SECTION:=libs
	CATEGORY:=Atom-GPL
	TITLE:=Bluetooth library
	URL:=http://www.bluez.org/
	DEPENDS:=+udev +glib2 +dbus
endef

CONFIGURE_ARGS += \
		--enable-shared \
		--enable-static \

TARGET_CFLAGS += $(FPIC)

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include/bluetooth
	$(CP) $(PKG_INSTALL_DIR)/usr/include/bluetooth/*.h $(1)/usr/include/bluetooth
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libbluetooth.{a,so*} $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/bluez.pc $(1)/usr/lib/pkgconfig/
endef

define Package/bluez/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libbluetooth.so.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,bluez))
