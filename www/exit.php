<?php
header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息
$commandd = "logout -u ".$_COOKIE['username'];
$socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
socket_connect($socket,'123.57.252.230',8888);
socket_write($socket, strlen($commandd).$commandd);
$tmp_res = socket_read($socket, 2048);
$res = substr($tmp_res, 4, substr($tmp_res, 0, 4));
$commandd = "exit";
$socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
socket_connect($socket,'123.57.252.230',8888);
socket_write($socket, strlen($commandd).$commandd);
$tmp_res = socket_read($socket, 2048);
$res = substr($tmp_res, 4, substr($tmp_res, 0, 4));
setcookie('PHPSESSID','',time()-3600);
setcookie('username','',time()-3600);
setcookie('privilege','',time()-3600);
echo $res;
?>