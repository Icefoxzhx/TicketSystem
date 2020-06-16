<?php

header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息
$tickettransfer_date = $_POST['tickettransfer_date'];
$tickettransfer_start_station = $_POST['tickettransfer_start_station'];
$tickettransfer_end_station = $_POST['tickettransfer_end_station'];
$sortt = $_POST['sort'];
if ($tickettransfer_start_station == "")
{     //判断输入是否为空
    echo "no";
}
else
{
    $commandd = "query_transfer -s $tickettransfer_start_station -t $tickettransfer_end_station -d $tickettransfer_date -p $sortt";
    $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
    socket_connect($socket,'123.57.252.230',8888);
    socket_write($socket, strlen($commandd).$commandd);
    $tmp_res = socket_read($socket, 2048);
    $res = substr($tmp_res, 4, substr($tmp_res, 0, 4));
    if($res == 0)
        echo "0";
    else
    {
        $array1 = explode("\n", $res); 
        for($i = 0; $i <= count($array1); $i++)
            $array2[$i] = explode(" ", $array1[$i]);
        for($i = 0; $i <= count($array1)-1; $i++)
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
}

?>