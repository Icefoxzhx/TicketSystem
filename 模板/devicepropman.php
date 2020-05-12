<?php
	header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
    error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息
    include 'connect.php';     //调用数据库连接文件

    $username = base64_encode($_POST['username']);
    $ownername = base64_encode($_POST['ownername']);     //接收前台post值
    $buy_date = $_POST['buy_date'];
    $lent_date = $_POST['lent_date'];
    $pc_brand = $_POST['pc_brand'];
    $pc_model = $_POST['pc_model'];
    $pc_cpu = $_POST['pc_cpu'];
    $pc_ram = $_POST['pc_ram'];
    $pc_hdd = $_POST['pc_hdd'];
    $pc_gpu = $_POST['pc_gpu'];
    $pc_sn = $_POST['pc_sn'];
    
    $useridsql = "SELECT * FROM user WHERE username = '$username'";//根据提交的用户名查找用户id
    $useridres = $conn->query($useridsql);
    $useridrow = $useridres->fetch_object();
    if ($useridrow == NULL) {//姓名输入错误时报错
        echo "使用者用户不存在".PHP_EOL;
    }
    $user_id = $useridrow->id;

    $owneridsql = "SELECT * FROM user WHERE username = '$ownername'";
    $owneridres = $conn->query($owneridsql);
    $owneridrow = $owneridres->fetch_object();
    if ($owneridrow == NULL) {//姓名输入错误时报错
        echo "所有者用户不存在";
    }
    $owner_id = $owneridrow->id;

    if (($useridrow!=NULL) && ($owneridrow!=NULL)) {
        $addsql = "INSERT INTO main (owner_id, user_id, buy_date, pc_cpu, pc_ram, pc_hdd, pc_brand, pc_model, pc_gpu, pc_sn, lent_date, if_rubbish) 
        VALUES ('$owner_id', '$user_id', '$buy_date', '$pc_cpu', '$pc_ram', '$pc_hdd', '$pc_brand', '$pc_model', '$pc_gpu', '$pc_sn', '$lent_date', '0')";
        $addres = $conn->query($addsql);
        if (!$addres){
            echo "failure";
        }
        else{
            echo "success";
        }
    }
?>