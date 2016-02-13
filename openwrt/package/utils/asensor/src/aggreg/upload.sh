#!/bin/ash

DATAFILE=/var/log/measured.data
SSH_KEYFILE="/root/.ssh/id_rsa" #private key
LOGIN=ameas
SERVER=server.ameas
TIMEOUT=5 #ssh timeout in seconds

lock "$DATAFILE".lock

scp -o "BatchMode yes" -o "PasswordAuthentication no" -o "ConnectionTimeout $TIMEOUT" -i "$SSH_KEYFILE" "$DATAFILE" "$LOGIN"@"$SERVER":upload  
ret=$?
if [ $ret -eq 0 ] 
 then
  #upload ok
  echo "Upload ok"
  rm -f "$DATAFILE"
  touch "$DATAFILE"
 else
  echo "scp RETURNS:$ret"
 fi
 
 
lock -u "$DATAFILE".lock


