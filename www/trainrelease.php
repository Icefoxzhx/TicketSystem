<?php

header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息
$trainrelease_id = $_POST['trainrelease_id'];//接收前台post值
if ($trainrelease_id == "")
{     //判断输入是否为空
    echo "no";
}
else
{
    $commandd = "release_train -i $trainrelease_id";
    $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
    socket_connect($socket,'127.0.0.1',8888);
    socket_write($socket, strlen($commandd).$commandd);
    $res = socket_read($socket, 2048);
    echo $res;
}

?>