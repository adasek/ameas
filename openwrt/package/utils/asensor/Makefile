#
# Copyright (C) 2014 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
                          
PKG_NAME:=asensor
PKG_VERSION:=0.046
PKG_RELEASE:=1

PKG_LICENSE:=GPL-3.0

#include $(INCLUDE_DIR)/uclibc++.mk
include $(INCLUDE_DIR)/package.mk

define Package/asensor
  SECTION:=utils
  CATEGORY:=Utilities
  MAINTAINER:=Adam Benda<bendaad1@fit.cvut.cz>
  TITLE:=asensor
  DEPENDS:=+i2c-tools +libstdcpp 
endef
             # +libi2c
             #+uclibcxx +libc +libstdcpp +uclibcxx +uclibc++
             # +libc +libstdc++   +uclibc++   +uclibc 
             
             
define Package/asensor/description
 Asensor is set of programs used to read measurements from different sensors.
endef

                     
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef        
   

define Package/asensor/install
	$(INSTALL_DIR) $(1)/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/beep_gpio $(1)/sbin/   
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/measure_bmp180 $(1)/sbin/   
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/measure_dht11 $(1)/sbin/      
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/measure_dht22 $(1)/sbin/    
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/measure_bh1750fvi $(1)/sbin/   
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/measure_analog $(1)/sbin/     
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lcd1602 $(1)/sbin/          
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/measure_pcf8591 $(1)/sbin/     
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/milisleep $(1)/sbin/          
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/measure_dalas.sh $(1)/sbin/measure_dalas
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/aggreg/measure.sh $(1)/sbin/measure.sh
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/aggreg/upload.sh $(1)/sbin/upload.sh
endef

$(eval $(call BuildPackage,asensor))
