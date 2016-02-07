<?php

$mysqli = new mysqli($mysql_server, $mysql_login, $mysql_pass, $mysql_db);


if ($mysqli->connect_errno) {
    header("Content-Type:text/plain");
    
    $server_protocol = filter_input(INPUT_SERVER, 'SERVER_PROTOCOL', FILTER_SANITIZE_FULL_SPECIAL_CHARS);
    header($server_protocol . ' 500 Internal Server Error', true, 500);

    echo "500 Internal Server Error\n";
    echo "Connect failed: " . $mysqli->connect_error . "\n";
    exit();
}

$mysqli->set_charset("utf8");