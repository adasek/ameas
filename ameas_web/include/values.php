<?php
/* Table of Devices and sensors. */

 //We need table of Devices and adjactment Sensors which do we have rights to 
 $select="SELECT 
    Sensor.name AS name,
    Sensor.id AS id,
    `sensorType-valueType`.pos AS pos,
    ValueType.name AS valueType,
    ValueType.color AS color,
    `has_rights_to`.type AS rights
   
 FROM `Sensor`
 LEFT JOIN SensorType ON Sensor.SensorType_id = SensorType.id
 LEFT JOIN `sensorType-valueType` ON `SensorType`.id=`sensorType-valueType`.SensorType_id
 LEFT JOIN `ValueType` ON `sensorType-valueType`.ValueType_id=`ValueType`.id
 LEFT JOIN `has_rights_to` ON Sensor.id=`has_rights_to`.Sensor_id
 WHERE has_rights_to.User_id=".$logged_user."";
 
 $loq=$mysqli->query($select);
 
 $tpl_content.="<table>\n";  
 $tpl_content.="<tr><th>Value</th><th>Plot</th></tr>\n";
 while($row=$loq->fetch_array()){
  $tpl_content.="<tr>\n";
  $tpl_content.="<td>".$row['name']." ".$row['valueType']."</td>\n";                          
  $tpl_content.="<td><a href=\"$BASEHREF?w=plot&amp;id=".$row['id']."&amp;pos=".$row['pos']."\">PLOT</a></td>\n";    
  $tpl_content.="</tr>\n\n";
 }
 $tpl_content.="</table>\n";
 
?>