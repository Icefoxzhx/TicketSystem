<?php

header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息
$usr_input = $_POST['usr_input'];//接收前台post值
if ($usr_input == "")
{     //判断输入是否为空
    echo "-1";
}
else
{
    $commandd = "query_profile -c ".$_COOKIE['username']." -u ".$usr_input;
    $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
    socket_connect($socket,'123.57.252.230',8888);
    socket_write($socket, strlen($commandd).$commandd);
    $tmp_res = socket_read($socket, 2048);
    $res = substr($tmp_res, 4, substr($tmp_res, 0, 4));
    if($res == "-1")
        echo "-1";
    else
    {
        $array = explode(" ", $res); 
        $row[0]['username'] = $array[0];
        $row[0]['name'] = $array[1];
        $row[0]['mailAddr'] = $array[2];
        $row[0]['privilege'] = $array[3];
        echo json_encode($row);
    }
}

?>