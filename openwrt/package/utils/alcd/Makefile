#
# Makefile for OpenWRT package alcd
# created by Adam Benda
# based on default OpenWRT Makefile
#

include $(TOPDIR)/rules.mk
                          
PKG_NAME:=alcd
PKG_VERSION:=0.5
PKG_RELEASE:=1

PKG_LICENSE:=CC-BY-SA-3.0

include $(INCLUDE_DIR)/package.mk

define Package/alcd
  SECTION:=utils
  CATEGORY:=Utilities
  MAINTAINER:=Adam Benda<bendaad1@fit.cvut.cz>
  TITLE:=alcd
  DEPENDS:=+i2c-tools +libstdcpp 
endef
             
define Package/alcd/description
 Alcd (binary lcd1602) is a program for displaying on LCD1602 display connected
 via I2C bus.
endef

                     
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef        
   

define Package/alcd/install
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lcd1602 $(1)/sbin/          
endef

$(eval $(call BuildPackage,alcd))
