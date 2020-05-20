<?php

header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息
$buy_trainid = $_POST['buy_trainid'];
$buy_date = $_POST['buy_date'];
$buy_start_station = $_POST['buy_start_station'];
$buy_end_station = $_POST['buy_end_station'];
$buy_num = $_POST['buy_num'];
$buy_queue = $_POST['buy_queue'];
$commandd = "buy_ticket -u ".$_COOKIE['username']." -i $buy_trainid -d $buy_date -f $buy_start_station -t $buy_end_station -n $buy_num";
if($buy_queue)
	$commandd = $commandd." -q true";
else
	$commandd = $commandd." -q false";
$socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
socket_connect($socket,'127.0.0.1',8888);
socket_write($socket, strlen($commandd).$commandd);
$res = socket_read($socket, 2048);
// $array = explode(" ", $res); 
// $row[0]['username'] = $array[0];
// $row[0]['name'] = $array[1];
// $row[0]['mailAddr'] = $array[2];
// $row[0]['privilege'] = $array[3];
// echo json_encode($row);
echo var_dump($res);
?>