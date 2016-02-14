#!/bin/ash
#this file is not in utf-8
#beware symbol for degress
# should be byte of dec value 223

while read -r line || [ -n "$line" ]
do
 cmd=`echo "$line" | cut -d"|" -f 2`
 val=`$cmd`
 val1=`echo "$val"|cut -d " " -f 1`
 val2=`echo "$val"|cut -d " " -f 2`
 
 base_cmd=`echo $cmd|cut -d " " -f 1`
  
  case "$base_cmd" in
    "/sbin/measure_dht22")                  
    val1=`echo "$val1"|awk  '{printf("Rel.hum: %2.1f%%",$0)}'`
    val2=`echo "$val2"|awk  '{printf("%2.0fﬂC",$0)}'`
     ;;
     
    "/sbin/measure_bmp180")                  
    val1=`echo "$val1"|awk  '{printf("(temp %2.1fﬂC)",$0)}'`
    val2=`echo "$val2"|awk  '{printf("%7.0fhPa",$0)}'`
     ;;
  esac
  echo "$val1"
  echo "$val2"
                
done  < /root/sensors.txt