#
# Makefile for OpenWRT package adisp
# created by Adam Benda
# based on default OpenWRT Makefile
#

include $(TOPDIR)/rules.mk
                          
PKG_NAME:=adisp
PKG_VERSION:=0.5
PKG_RELEASE:=1

PKG_LICENSE:=MIT

include $(INCLUDE_DIR)/package.mk

define Package/adisp
  SECTION:=utils
  CATEGORY:=Utilities
  MAINTAINER:=Adam Benda<adam@adasek.cz>
  TITLE:=adisp
  DEPENDS:=+alcd 
endef
             
define Package/adisp/description
 Program to display information from various sources on a small display(lcd1602).
endef
                      
define Build/Compile
  :
endef
                     
define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef        
   

define Package/adisp/install
	$(INSTALL_DIR) $(1)/sbin                                
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/adisp.sh $(1)/sbin/adisp   
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/adisp-loop.sh $(1)/sbin/adisp-loop      
	$(INSTALL_DIR) $(1)/usr/share/adisp-plugins 
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/plugins/* $(1)/usr/share/adisp-plugins/   
	$(INSTALL_DIR) $(1)/etc                         
	$(INSTALL_DIR) $(1)/etc/init.d                                       
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/adisp.conf $(1)/etc/adisp.conf     
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/adisp.plugins $(1)/etc/adisp.plugins               
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/adisp-init $(1)/etc/init.d/adisp          
endef

define Package/adisp/postinst
#!/bin/sh
# check if we are on real system
if [ -z "$${IPKG_INSTROOT}" ]; then
  echo "Enabling adisp-loop as service"
  /etc/init.d/adisp enable
fi
exit 0
endef


define Package/adisp/prerm
#!/bin/sh
# check if we are on real system
if [ -z "$${IPKG_INSTROOT}" ]; then
  echo "Stopping adisp-loop"
  /etc/init.d/adisp stop
  echo "Disabling adisp-loop as service"
  /etc/init.d/adisp disable
fi
exit 0
endef


$(eval $(call BuildPackage,adisp))
