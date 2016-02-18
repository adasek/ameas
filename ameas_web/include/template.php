<?php 
/* 
 * Template is in design/template.htm
 * This script substitutes some variables there. 
 *
* Author: Adam Benda <adam@adasek.cz>, 2016
*/

$data=file_get_contents('design/template.htm');

  $substitute=array("tpl_content","tpl_title","tpl_logged","BASEHREF");
  
foreach($substitute as $var){
     $data=str_replace("$".$var,${$var},$data); 
 }

echo $data;
?>
