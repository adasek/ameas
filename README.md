# ameas
OpenWRT project that connects low cost sensors to wifi routers via GPIO and aggregates the measurements.

## parts:
Project consists of several parts;  

######  Alcd
Openwrt package for interfacing LCD1602 using I2C bus
 * openwrt/package/utils/alcd

######  Adisp
Openwrt package for displaying various information on that LED screen
 * openwrt/package/utils/adisp
                 
######  Asensor
Openwrt package with binaries measuring different sensors; aggregates measured
data and sends them to central server
 * openwrt/package/utils/asensor

######  Asensor-server
Server part of Asensor measurement gathering; shell scripts
 * asensor_server 

######  Ameas web
PHP+Javascript server for visualisation of measured data
 * ameas_web

This project begun as part of my bachelor thesis on The Faculty of Information Technology of the Czech Technical University in Prague.  