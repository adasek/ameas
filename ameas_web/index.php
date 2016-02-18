<?php
/*
 * Main entry point of the web application
 *
 * Author: Adam Benda <adam@adasek.cz>, 2016
*/

include_once('config.php');
include_once('include/connection.php');
include_once('include/login.php');

$tpl_content = "";
$tpl_title = "Ameas";


if (!$logged) {
    //We are not logged = show login form     
    include('include/login_form.php');

    include_once('include/template.php');
    exit;
}

if ($_GET['w'] === "sensors") {
    include('include/sensors.php');
} else if ($_GET['w'] === "values") {
    include('include/values.php');
} else if ($_GET['w'] === "plot") {
    include('include/plot.php');
} else {
    // MENU
    $tpl_content.="<ul>";
    $tpl_content.="<li><a href=\"" . $BASEHREF . "?w=sensors\">Sensors</a></li>\n";
    $tpl_content.="<li><a href=\"" . $BASEHREF . "?w=values\">Values</a></li>\n";
    $tpl_content.="</ul>\n";
}



//$tpl_content.="Hello, logged user $logged_user!";


include_once('include/template.php');
?>
