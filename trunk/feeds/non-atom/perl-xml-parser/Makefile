# 
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id: Makefile 9947 2007-12-27 02:12:04Z nbd $

include $(TOPDIR)/rules.mk

PKG_NAME:=perl-xml-parser
PKG_VERSION:=2.36
PKG_RELEASE:=1

PERL_PACKAGENAME:=XML-Parser
PKG_SOURCE:=$(PERL_PACKAGENAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://search.cpan.org/CPAN/authors/id/M/MS/MSERGEANT
PKG_MD5SUM:=x

PKG_BUILD_DEPENDS:=perl, expat

PKG_SOURCE_DIR:=$(BUILD_DIR)/$(PERL_PACKAGENAME)-$(PKG_VERSION)
PKG_BUILD_DIR:=$(BUILD_DIR)/perl-xml-parser-$(PKG_VERSION)
PKG_INSTALL_DIR:=$(PKG_BUILD_DIR)/ipkg-install

include $(INCLUDE_DIR)/package.mk

PERL_MAKE_OPTS=PASTHRU_INC="$(TARGET_CFLAGS) $(PERL_CPPFLAGS) -I$(STAGING_DIR)/include -I$(STAGING_DIR)/usr/include" \
		OTHERLDFLAGS="$(PERL_LDFLAGS)"
include $(TOPDIR)/package/perlmod.mk

define Package/perl-xml-parser
  SECTION:=lang
  CATEGORY:=Languages
  DEPENDS:=+perl +libexpat
  TITLE:=Perl module $(PERL_PACKAGENAME)
  URL:=
endef

define Build/Prepare
	$(call Build/Prepare/Default,)
	rm -rf $(PKG_BUILD_DIR)
	mv $(PKG_SOURCE_DIR) $(PKG_BUILD_DIR)
endef

define Build/Configure
	$(PERL_MM_CMD)
endef

define Build/Compile
	$(PERL_MAKE_CMD) DESTDIR=$(PKG_INSTALL_DIR) all install
endef

define Build/InstallDev
	$(PERL_MAKE_CMD) DESTDIR=$(STAGING_DIR) all install
endef

define Package/perl-xml-parser/install
	$(INSTALL_DIR) $(1)
	$(CP) -Rp $(PKG_INSTALL_DIR)/* $(1)/
endef

$(eval $(call BuildPackage,perl-xml-parser))

