<?php
    header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
    error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息
    include 'logout.php';
    $commandd = "clean";
    $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
    socket_connect($socket,'127.0.0.1',8888);
    socket_write($socket, strlen($commandd).$commandd);
    $res = socket_read($socket, 2048);
    header("location:index.html");
?>
