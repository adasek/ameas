#!/bin/sh

#"Cache" for 1 hour

datFILE=/tmp/adisp_bbc_data
timFILE=/tmp/adisp_bbc_time
now=`date +%s`
if [ `cat "$timFILE"` -gt `expr $now - 3600 ` ]
 then
 :
else
 wget -O /tmp/bbc_"$$" 'http://feeds.bbci.co.uk/news/world/rss.xml' > /dev/null
 awk '
 BEGIN {RS="<";FS=">"}
/^item/ {item++;}
/^\/item/ {item--;}

/^description/ && item>0 {
print $2
}


' /tmp/bbc_"$$" | head -n 4 > "$datFILE"
date +%s > "$timFILE"

fi

cat "$datFILE"
