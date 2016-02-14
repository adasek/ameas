#!/bin/sh
#
# Adisp loop: Runs adisp main program in a loop
# Should have /etc/init.d/adisp script
#
# Author: Adam Benda, 2016
# License: MIT 
# https://github.com/adasek/ameas/
#
#
#


while true
do
 if [ ! -r "/sbin/adisp" ]
  then
  echo "Adisp main program not found!" >&2
  exit 5
 fi

/sbin/adisp
retVal=$?

 if [ $retVal -eq 0 ]
  then
  sleep 1 #Do not cause active wait if the main program was too quick
 else
  echo "Adisp main program failed with code $retVal!" >&2
  exit $retVal
 fi
done 

