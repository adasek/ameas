#!/bin/ash

SENSORLIST=/root/sensors.txt     #sensor list
DATAFILE=/var/log/measured.data  #measurement results
LOGFILE=/var/log/measured.log    #Log
SSH_KEYFILE="/root/.ssh/id_rsa"  #private key

#temporary
NUMBER_OF_MEASUREMENTS=8
MIN_MEASUREMENTS=5 #minimum successful measurements number to count this try

MAX_MEASUREMENTS=10
TIME_BETWEEN_MEASUREMENTS=200; #in miliseconds

while read sensor   #foreach sensor
 do
  sensor_id=`echo "$sensor"|cut -d"|" -f1`
  measurement_program=`echo "$sensor"|cut -d"|" -f2`
  measurement_program_bin=`echo "$measurement_program"|cut -d" " -f 1`
#  retries=`echo "$sensor"|cut -d"|" -f3`

    if [ ! -x "$measurement_program_bin" ]
     then
          #not executable measurable program
          echo "ERR:`date +%s`:"$sensor_id":Not executable measurent program: "
\'"$measurement_program_bin"\' >&2
          exit 1
    fi
    case "$sensor_id" in
        ''|*[!0-9]*)
          #non numeric sensorID
          echo "ERR:`date +%s`:Wrong sensorID: "\'"$sensor_id"\' >&2
          exit 2
         ;;
    esac

   cnt=0
   #multiple measurement
   successful=0
   values=""
    for m in `seq 1 $MAX_MEASUREMENTS`
      do
       value=`$measurement_program`
       status="$?"
        if [ $status -eq 0 ]
          then
           #successful measurement
           values="$values;$value"
           successful=`expr $successful + 1`
          elif [ `expr $status % 2` -eq 1 ]
          then
           #non-recoverable error
          echo ERR:`date +%s`:"$sensor_id:Not recoverable error - status $status" >&2
           break
         fi
        #else - recoverable error
        if [ $successful -ge $NUMBER_OF_MEASUREMENTS ]
         then
          #we have enough measurements
          break
         fi
       milisleep $TIME_BETWEEN_MEASUREMENTS
      done

    if [ $successful -ge $MIN_MEASUREMENTS ]
     then
       #make an average from values
       value=`echo "$values"|awk 'BEGIN {RS=";";maxNF=0;} {for(i=1;i<=NF;i++){vals[i]+=$i;num[i]++;}} END{for (i in vals){print (vals[i]/num[i]);}}'`

        lock "$DATAFILE".lock

	 #write last value
	 echo `date +%s` $sensor_id $value > "$DATAFILE".last"$sensor_id"

	 #write into shared data file
 	 echo `date +%s` $sensor_id $value >> "$DATAFILE"

	lock -u "$DATAFILE".lock
     else
          echo WARN:`date +%s`:"$sensor_id":"Only $successful out of $MAX_MEASUREMENTS measurements succeeded. Needed at least $MIN_MEASUREMENTS" >&2
     fi



#foreach sensor loop end
done < "$SENSORLIST"


