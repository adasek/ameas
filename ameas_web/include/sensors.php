<?php
/* Table of Devices and sensors. */

 //We need table of Devices and adjactment Sensors which do we have rights to 
 $select="SELECT *,has_rights_to.type AS rights,Sensor.name AS sensor_name,SensorType.name AS sensor_type_name FROM Device RIGHT OUTER JOIN Sensor ON Device.id=Sensor.Device_id  JOIN SensorType on SensorType_id = SensorType.id  JOIN has_rights_to ON Sensor.id=has_rights_to.Sensor_id  WHERE has_rights_to.User_id=".$logged_user." AND has_rights_to.type>0";
 $loq=$mysqli->query($select);
 
 $tpl_content.="<table>\n";  
 $tpl_content.="<tr><th>Device</th><th>Sensor</th><th>Sensor Type</th></tr>\n";
 while($row=$loq->fetch_array()){
  $tpl_content.="<tr>\n";
  $tpl_content.="<td>".$row['hostname']."(".$row['ip4Addr'].")</td>\n";  
  $tpl_content.="<td>".$row['sensor_name']."</td>\n";                                 
  $tpl_content.="<td><a href=\"$BASEHREF?w=st&amp;id=".$row['SensorType_id']."\">".$row['sensor_type_name']."</a></td>\n";    
  $tpl_content.="</tr>\n\n";
 }
 $tpl_content.="</table>\n";
 
?>