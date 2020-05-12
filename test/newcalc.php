<?php
    header("content-type:text/html;charset=utf-8");
    error_reporting(E_ALL &~ E_NOTICE);
    $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
	socket_connect($socket,'127.0.0.1',8888);
	$num1 = $_POST['num1'];
	socket_write($socket, $num1);
 	$res = socket_read($socket, 2048);
    echo $res;
?>