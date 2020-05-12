<?php
    header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
    error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息
    include 'connect.php';     //调用数据库连接文件

    $tg_id = $_POST['tg_id'];    //main表中电脑id
    $describe = $_POST['describe']; //接收前台post值
    $date = '20'.date('ymd',time());//获取添加的时间

    $pcsql = "SELECT * FROM main WHERE id = '$tg_id'";
    $pcres = $conn->query($pcsql);
    $pcrow = $pcres->fetch_object();
    $from_id = $pcrow->user_id;
    $sql = "INSERT INTO `repair` (`id`, `date`, `tg_id`, `describe`, `from_id`) VALUES (NULL, '$date', '$tg_id', '$describe', '$from_id')";
   	$send = $conn->query($sql);//向repair表增加记录
    if ($send)
        echo "success";
    else
        echo "failure";
?>