<?php 
/* 
Template is in design/template.htm
 We get that and substitute something there.
*/

$data=file_get_contents('design/template.htm');

  $substitute=array("tpl_content","tpl_title","tpl_logged","BASEHREF");
  
foreach($substitute as $var){
     $data=str_replace("$".$var,${$var},$data); 
 }

echo $data;
?>