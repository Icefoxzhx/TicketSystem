<?php

header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息
$add_trainid = $_POST['add_trainid'];
$add_seatnum = $_POST['add_seatnum'];
$add_stationnum = $_POST['add_stationnum'];
$add_stations = $_POST['add_stations'];
$add_prices = $_POST['add_prices'];
$add_starttime = $_POST['add_starttime'];
$add_traveltimes = $_POST['add_traveltimes'];
$add_stopovertimes = $_POST['add_stopovertimes'];
$add_saledate = $_POST['add_saledate'];
$add_type = $_POST['add_type'];
$commandd = "add_train -i $add_trainid -n $add_stationnum -m $add_seatnum -s $add_stations -p $add_prices -x $add_starttime -t $add_traveltimes -o $add_stopovertimes -d $add_saledate -y $add_type";
$socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
socket_connect($socket,'123.57.252.230',8888);
socket_write($socket, strlen($commandd).$commandd);
$tmp_res = socket_read($socket, 2048);
$res = substr($tmp_res, 4, substr($tmp_res, 0, 4));
// $array = explode(" ", $res); 
// $row[0]['username'] = $array[0];
// $row[0]['name'] = $array[1];
// $row[0]['mailAddr'] = $array[2];
// $row[0]['privilege'] = $array[3];
// echo json_encode($row);
echo var_dump($res);

?>