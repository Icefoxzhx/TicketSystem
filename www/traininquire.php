<?php

header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息
$traininquire_id = $_POST['traininquire_id'];//接收前台post值
$traininquire_date = $_POST['traininquire_date'];
if ($traininquire_id == "")
{     //判断输入是否为空
	echo "no";
}
else
{
	$commandd = "query_train -i $traininquire_id -d $traininquire_date";
	$socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
	socket_connect($socket,'123.57.252.230',8888);
	socket_write($socket, strlen($commandd).$commandd);
	$tmp_res = socket_read($socket, 2048);
	$res = substr($tmp_res, 4, substr($tmp_res, 0, 4));
	$array1 = explode("\n", $res); 
	for($i = 0; $i <= count($array1); $i++)
		$array2[$i] = explode(" ", $array1[$i]);
	$array3[0]['trainid'] = $array2[0][0];
	$array3[0]['type'] = $array2[0][1];
	for($i = 1; $i <= count($array1)-2; $i++)
	{
		$array3[$i]['stations'] = $array2[$i][0];
		$array3[$i]['arriving_time'] = $array2[$i][1].'<br/>'.$array2[$i][2];
		$array3[$i]['leaving_time'] = $array2[$i][4].'<br/>'.$array2[$i][5];
		$array3[$i]['price'] = $array2[$i][6];
		$array3[$i]['seat'] = $array2[$i][7];
	}
	echo json_encode($array3);
}

?>