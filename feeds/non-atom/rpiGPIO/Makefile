include $(TOPDIR)/rules.mk

PKG_NAME:=Rpi.GPIO
PKG_VERSION:=0.5.8
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/Rpi.GPIO-utils-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/Rpi.GPIO
  SUBMENU:=Python
  CATEGORY:=Languages
  TITLE:=Python RPI
  URL:=https://sourceforge.net/p/raspberry-gpio-python
  DEPENDS:=python
endef

define Package/Rpi.GPIO/description
 Python GPIO library
endef

define Build/Prepare
 hg clone http://hg.code.sf.net/p/raspberry-gpio-python/code $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(call Build/Compile/PyMod,,install --prefix="$(PKG_INSTALL_DIR)/usr")
endef

define Package/Rpi.GPIO/install
	$(INSTALL_DIR) $(1)$(PYTHON_PKG_DIR)
	$(CP) \
	    $(PKG_INSTALL_DIR)$(PYTHON_PKG_DIR)/* \
	    $(1)$(PYTHON_PKG_DIR)
endef

$(eval $(call BuildPackage,Rpi.GPIO))
