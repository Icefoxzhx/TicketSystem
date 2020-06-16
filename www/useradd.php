<?php

header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息
$add_username = $_POST['add_username'];
$add_name = $_POST['add_name'];
$add_password = $_POST['add_password'];
$add_mailAddr = $_POST['add_mailAddr'];
$add_privilege = $_POST['add_privilege'];
$commandd = "add_user -c ".$_COOKIE['username']." -u $add_username -p $add_password -n $add_name -m $add_mailAddr -g $add_privilege";
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
echo $res;

?>