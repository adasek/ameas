<?php

/*
*
* List of all sensors for specified device
* in /root/sensors.txt format
*
*
*/

require_once("config.php");
require_once("include/connection.php");


if(!empty($_GET['device']) && is_numeric($_GET['device'])){
  $q="SELECT *,Sensor.id AS Sensor_id FROM Device JOIN Sensor ON Device.id=Sensor.Device_id JOIN SensorType ON Sensor.SensorType_id=SensorType.id WHERE Device.id=".$_GET['device']."";
  $result=$mysqli->query($q);
  echo $mysqli->error;
   while($row=$result->fetch_array()){
     if($afterFirst){echo "\n";}else{$afterFirst=true;}
    echo $row['Sensor_id']."|".$row['script']." ".$row['script_param']."";  
   }

 }
 else{
  echo "Err.";
  exit;
 }
 
?>