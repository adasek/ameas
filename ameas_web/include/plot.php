<?php

/* Table of Devices and sensors. */


if (is_numeric($_GET['id'])) {
    $sensorID = $_GET['id'];
} else {
    header("Content-Type:text/plain");
    header($_SERVER['SERVER_PROTOCOL'] . ' 400 Bad Request', true, 400);
    echo "400 Bad Request\n";
    exit();
}

if (is_numeric($_GET['pos'])) {
    $sensorPos = $_GET['pos'];
} else {
    header("Content-Type:text/plain");
    header($_SERVER['SERVER_PROTOCOL'] . ' 400 Bad Request', true, 400);
    echo "400 Bad Request\n";
    exit();
}


//We need table of Devices and adjactment Sensors which do we have rights to 
$select = "SELECT 
    *,Sensor.name as sensorName, ValueType.name as valueTypeName
 FROM `Sensor`
 LEFT JOIN SensorType ON Sensor.SensorType_id = SensorType.id
 LEFT JOIN `sensorType-valueType` ON `SensorType`.id=`sensorType-valueType`.SensorType_id
 LEFT JOIN `ValueType` ON `sensorType-valueType`.ValueType_id=ValueType.id
 LEFT JOIN `has_rights_to` ON Sensor.id=`has_rights_to`.Sensor_id
 WHERE `has_rights_to`.User_id=" . $logged_user . "
   AND Sensor.id=" . $sensorID . "
   AND `sensorType-valueType`.pos=" . $sensorPos . "";

$loq = $mysqli->query($select);

if ($loq->num_rows == 0) {
    header("Content-Type:text/plain");
    header($_SERVER['SERVER_PROTOCOL'] . ' 401 Unauthorized', true, 401);
    echo "401 Unauthorized\n";
    echo $select;
    exit();
}

//Sensor exists and we have the rights
$sensorRow = $loq->fetch_array();  //DATA about sensor

$lastValSelect = "SELECT 
    time,val
 FROM `Measurement`
  WHERE Sensor_id=" . $sensorID . " AND pos=" . $sensorPos . "
 ORDER BY time DESC
 LIMIT 1";
$lastValQuery = $mysqli->query($lastValSelect);
$lastValRow = $lastValQuery->fetch_array();
$lastValue = $lastValRow['val'];
$lastValueTime = $lastValRow['time'];

$tpl_content.="<h1>" . $sensorRow['sensorName'] . ": ";
$tpl_content.="" . $sensorRow['valueTypeName'] . "</h1>";
$tpl_content.="<h2>" . $lastValue . " at " . $lastValueTime . "</h2>";
$toTime = new Datetime();
$fromTime = clone $toTime;
$fromTime->sub(new DateInterval('PT24H')); 

$tpl_content.="<script type=\"text/javascript\">\n";
$tpl_content.="showPlot(" . $sensorID . "," . $sensorPos . ",'" . $sensorRow['color'] . "'," . $fromTime->getTimestamp() . "," . $toTime->getTimestamp() . ");";
$tpl_content.="</script>\n"; 

$tpl_content.="<form action=\"#\" onsubmit=\"return replot(" . $sensorID . "," . $sensorPos . ",'" . $sensorRow['color'] . "');\">";
$tpl_content.="<input type=\"text\" id=\"replotForm_fromTime\" value=\"".$fromTime->format("Y-m-d H:i")."\">\n";
$tpl_content.="<input type=\"text\" id=\"replotForm_toTime\" value=\"".$toTime->format("Y-m-d H:i")."\">\n";
$tpl_content.="<input type=\"submit\" value=\"Apply\">\n";
$tpl_content.="</form>"

?>
