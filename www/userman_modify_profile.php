<?php

header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息
$modify_username = $_POST['modify_username'];
$modify_name = $_POST['modify_name'];
$modify_password = $_POST['modify_password'];
$modify_mailAddr = $_POST['modify_mailAddr'];
$modify_privilege = $_POST['modify_privilege'];
$commandd = "modify_profile -c ".$_COOKIE['username']." -u ".$modify_username;
if($modify_password != "")
  	$commandd = "$commandd -p $modify_password";
$commandd = "$commandd -m $modify_mailAddr -g $modify_privilege";
$socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
socket_connect($socket,'123.57.252.230',8888);
socket_write($socket, strlen($commandd).$commandd);
$tmp_res = socket_read($socket, 2048);
$res = substr($tmp_res, 4, substr($tmp_res, 0, 4));
echo $res;

?>