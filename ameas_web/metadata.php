<?php

header('Content-Type: application/json');

include_once('config.php');
include_once('include/connection.php');
include_once('include/login.php');


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


$qu = $mysqli->query("SELECT val,UNIX_TIMESTAMP(TIME) AS t
  FROM Measurement
  LEFT JOIN has_rights_to ON has_rights_to.Sensor_id=Measurement.Sensor_id
  WHERE Measurement.Sensor_id=" . $sensorID . " AND pos=" . $sensorPos . " AND has_rights_to.User_id=" . $logged_user . "
   AND time>'" . $timeStart . "' AND time<='" . $timeEnd . "'
  ORDER BY time DESC");


$first = 1;
while ($row = $qu->fetch_array()) {
    echo "{";
    echo "\"time\":" . ($row['t']) . ",";
    echo "\"value\":" . $row['val'];
    echo "}\n";
}