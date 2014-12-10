include $(TOPDIR)/rules.mk

PKG_NAME:=adaFruitLCDPlate
PKG_VERSION:=0.5.8
PKG_RELEASE:=1

PKG_BUILD_DIR:=$(BUILD_DIR)/adaFruitLCDPlate-utils-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk
$(call include_mk, python-package.mk)

define Package/adaFruitLCDPlate
  SUBMENU:=Python
  CATEGORY:=Languages
  TITLE:=Python ADAfruit LCD
  URL:=https://github.com/adafruit/Adafruit_Python_CharLCD
  DEPENDS:=python
endef

define Package/adaFruitLCDPlate/description
 Python ADAfruit library and example code
endef

define Build/Prepare
 git clone https://github.com/adafruit/Adafruit_Python_CharLCD.git $(PKG_BUILD_DIR)
endef

define Build/Compile
	$(call Build/Compile/PyMod,,install --prefix="$(PKG_INSTALL_DIR)/usr")
endef

define Package/adaFruitLCDPlate/install
	$(INSTALL_DIR) $(1)$(PYTHON_PKG_DIR)
	$(CP) \
	    $(PKG_INSTALL_DIR)$(PYTHON_PKG_DIR)/* \
	    $(1)$(PYTHON_PKG_DIR)
endef

$(eval $(call BuildPackage,adaFruitLCDPlate))
