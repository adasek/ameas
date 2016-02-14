#
# Makefile for OpenWRT package alcd
# created by Adam Benda
# based on default OpenWRT Makefile
#


include $(TOPDIR)/rules.mk
                          
PKG_NAME:=asensor
PKG_VERSION:=0.51
PKG_RELEASE:=1

PKG_LICENSE:=MIT

include $(INCLUDE_DIR)/package.mk

define Package/asensor
  SECTION:=utils
  CATEGORY:=Utilities
  MAINTAINER:=Adam Benda<adam@adasek.cz>
  TITLE:=asensor
  DEPENDS:=+i2c-tools +libstdcpp 
endef
             
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
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/measure_pcf8591 $(1)/sbin/     
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/milisleep $(1)/sbin/          
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/measure_dalas.sh $(1)/sbin/measure_dalas
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/aggreg/measure.sh $(1)/sbin/measure.sh
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/aggreg/upload.sh $(1)/sbin/upload.sh  
	$(INSTALL_DIR) $(1)/etc/crontabs 
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/aggreg/asensor-crontab $(1)/etc/crontabs/asensor-crontab
endef

$(eval $(call BuildPackage,asensor))
