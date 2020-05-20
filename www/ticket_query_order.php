<?php

header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息
$username = $_POST['username'];
if ($username == "")
{
    echo "no";
}
else
{
    $commandd = "query_order -u $username";
    $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
    socket_connect($socket,'127.0.0.1',8888);
    socket_write($socket, strlen($commandd).$commandd);
    $res = socket_read($socket, 2048);
    $array1 = explode("\n", $res); 
    for($i = 0; $i <= count($array1); $i++)
        $array2[$i] = explode(" ", $array1[$i]);
    $array3[0]['total_num'] = $array2[0][0];
    for($i = 1; $i <= count($array1)-2; $i++)
    {
        $array3[$i]['order_status'] = $array2[$i][0];
        $array3[$i]['trainid'] = $array2[$i][1];
        $array3[$i]['start_station'] = $array2[$i][2];
        $array3[$i]['start_station_time'] = $array2[$i][3].'<br/>'.$array2[$i][4];
        $array3[$i]['end_station'] = $array2[$i][6];
        $array3[$i]['end_station_time'] = $array2[$i][7].'<br/>'.$array2[$i][8];
        $array3[$i]['price'] = $array2[$i][9];
        $array3[$i]['order_number'] = $array2[$i][10];
    }
    echo json_encode($array3);
}

?>