#!/bin/ash
#Dalas temperature sensor $1 = ID
SENSORID="$1"                                                     

file=/sys/devices/w1_bus_master1/"$SENSORID"/w1_slave
echo "Dalas $1 invoked" >> /var/log/tmp

if lsmod|grep -q w1_gpio >/dev/null 2>/dev/null
 then
  :
 else #Required module is not loaded
  insmod /lib/modules/*/w1-gpio.ko  >/dev/null 2>/dev/null
fi

#Handle errors:

if [ ! -f "$file" ]
  then
    #File does not exist = not recovable error
    echo 0
    exit 1
 fi
 
if [ ! -r "$file" ]
  then
  #File is not readable (read permissions are not granted)
  # = not recovable error
  echo 0
  exit 3 
 fi
 
data=`cat $file 2>/dev/null`   
if [ $? -ne 0 ]
  then
  #Another Error reading the file  = maybe recovable?
  echo 0
  exit 4  
 fi
 
crcOK=`echo "$data"|grep "crc="|grep -o '[^ ]*$'` #cut last
 if [ $crcOK = "YES" ]
  then
   status=0
  else
   #bad crc = probably recoverable
   status=2
  fi 
 
val=`echo "$data" | grep t=|cut -d"=" -f2`

echo "$val"|sed -e "s/\(...\)$/.\1/"


exit $status