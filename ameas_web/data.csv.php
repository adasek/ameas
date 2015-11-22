<?php
header('Content-Type: text/csv');
                            
include_once('config.php');
include_once('include/connection.php');
include_once('include/login.php');


if(is_numeric($_GET['id'])){
$sensorID=$_GET['id'];
}else{         
    header("Content-Type:text/plain");   
    header($_SERVER['SERVER_PROTOCOL'] . ' 400 Bad Request', true, 400);
    echo "400 Bad Request\n";
    exit();
} 

if(is_numeric($_GET['pos'])){
$sensorPos=$_GET['pos'];
}else{          
    header("Content-Type:text/plain");   
    header($_SERVER['SERVER_PROTOCOL'] . ' 400 Bad Request', true, 400);
    echo "400 Bad Request\n";
    exit();
}

/* Last day = default */
$timeStart=date("Y-m-d H:i:s",time()-86400);
$timeEnd=date("Y-m-d H:i:s");
if(is_numeric($_GET['from'])){
 $timeStart=date("Y-m-d H:i:s",$_GET['from']);
}
if(is_numeric($_GET['to'])){
 $timeEnd=date("Y-m-d H:i:s",$_GET['to']);
}

$qu=$mysqli->query("SELECT val,UNIX_TIMESTAMP(TIME) AS t
  FROM Measurement
  LEFT JOIN has_rights_to ON has_rights_to.Sensor_id=Measurement.Sensor_id
  WHERE Measurement.Sensor_id=".$sensorID." AND pos=".$sensorPos." AND has_rights_to.User_id=".$logged_user."
   AND time>'".$timeStart."' AND time<='".$timeEnd."'
  ORDER BY time DESC");

 
 $first=1;
 echo "time,value\n";
 while($row=$qu->fetch_array()){  
  echo ($row['t']).",";
  echo $row['val']."\n";
  }
    
   


?>
