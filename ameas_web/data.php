<?php
/*
 Data service - generating datasets for authenticated users

 Author: Adam Benda <adam@adasek.cz>, 2016

*/
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

/* Last day = default */
$timeStart = date("Y-m-d H:i:s", time() - 86400);
$timeEnd = date("Y-m-d H:i:s");
if (is_numeric($_GET['from'])) {
    $timeStart = date("Y-m-d H:i:s", $_GET['from']);
} else {
    throw new Exception("from not valid");
}
if (is_numeric($_GET['to'])) {
    $timeEnd = date("Y-m-d H:i:s", $_GET['to']);
} else {
    throw new Exception("to not valid");
}

$qu = $mysqli->query("SELECT val,UNIX_TIMESTAMP(TIME) AS t
  FROM Measurement
  LEFT JOIN has_rights_to ON has_rights_to.Sensor_id=Measurement.Sensor_id
  WHERE Measurement.Sensor_id=" . $sensorID . " AND pos=" . $sensorPos . " AND has_rights_to.User_id=" . $logged_user . "
   AND time>'" . $timeStart . "' AND time<='" . $timeEnd . "'
  ORDER BY time DESC");


$first = true;
echo "[";
while ($row = $qu->fetch_array()) {
    if (!$first) {
        echo ",\n";
    } else {
        $first = false;
    }
    echo "{";
    echo "\"time\":" . ($row['t']) . ",";
    echo "\"value\":" . $row['val'];
    echo "}";
}
echo "]";
