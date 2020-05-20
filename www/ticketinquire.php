<?php

header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息
$ticketinquire_date = $_POST['ticketinquire_date'];
$ticketinquire_start_station = $_POST['ticketinquire_start_station'];
$ticketinquire_end_station = $_POST['ticketinquire_end_station'];
$sortt = $_POST['sort'];
if ($ticketinquire_start_station == "")
{     //判断输入是否为空
    echo "no";
}
else
{
    $commandd = "query_ticket -s $ticketinquire_start_station -t $ticketinquire_end_station -d $ticketinquire_date -p $sortt";
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
        $array3[$i]['trainid'] = $array2[$i][0];
        $array3[$i]['start_station'] = $array2[$i][1];
        $array3[$i]['start_station_time'] = $array2[$i][2].'<br/>'.$array2[$i][3];
        $array3[$i]['end_station'] = $array2[$i][5];
        $array3[$i]['end_station_time'] = $array2[$i][6].'<br/>'.$array2[$i][7];
        $array3[$i]['price'] = $array2[$i][8];
        $array3[$i]['seat'] = $array2[$i][9];
    }
    echo json_encode($array3);
}

?>