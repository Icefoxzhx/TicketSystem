<?php
	header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
    error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息
    include 'connect.php';     //调用数据库连接文件

    $tg_id = $_POST['tg_id'];     //接收前台post值
    $prop_type = $_POST['prop_type'];
    // $from = base64_encode($_POST['from']);
    $to = base64_encode($_POST['to']);
    $date = $_POST['date'];
    $describe = $_POST['describe'];

    // $fromidsql = "SELECT * FROM user WHERE username = '$from'";//根据提交的用户名查找用户id
    // $fromidres = $conn->query($fromidsql);
    // $fromidrow = $fromidres->fetch_object();
    // if ($fromidrow == NULL) {//姓名输入错误时报错
    //     echo "移交前用户不存在".PHP_EOL;
    // }
    // $from_id = $fromidrow->id;

    $pcsql = "SELECT * FROM main WHERE id = '$tg_id'";
    $pcres = $conn->query($pcsql);
    $pcrow = $pcres->fetch_object();


    $toidsql = "SELECT * FROM user WHERE username = '$to'";//根据提交的用户名查找用户id
    $toidres = $conn->query($toidsql);
    $toidrow = $toidres->fetch_object();
    if ($toidrow == NULL) {//姓名输入错误时报错
        echo "移交后用户不存在".PHP_EOL;
    }
    else{
        $to_id = $toidrow->id;
        switch ($prop_type) {
            case '1':
                $from_id = $pcrow->owner_id;
                $transsql = "INSERT INTO `trans` (`id`, `date`, `tg_id`, `describe`, `from_id`, `to_id`, `prop_type`) VALUES (NULL, '$date', '$tg_id', '$describe', '$from_id', '$to_id', '$prop_type')";
                $transres = $conn->query($transsql);//在trans表里添加一行数据

                $mainsql = "UPDATE main SET lent_date='$date', owner_id='$to_id' WHERE id='$tg_id'";
                $mainres = $conn->query($mainsql);

                if ((!$transres) || (!$mainres))
                    echo "failure";
                else
                    echo "success";
                break;
            case '2':
                $from_id = $pcrow->user_id;
                $transsql = "INSERT INTO `trans` (`id`, `date`, `tg_id`, `describe`, `from_id`, `to_id`, `prop_type`) VALUES (NULL, '$date', '$tg_id', '$describe', '$from_id', '$to_id', '$prop_type')";
                $transres = $conn->query($transsql);//在trans表里添加一行数据

                $mainsql = "UPDATE main SET lent_date='$date', user_id='$to_id' WHERE id='$tg_id'";
                $mainres = $conn->query($mainsql);

                if ((!$transres) || (!$mainres))
                    echo "failure";
                else
                    echo "success";
                break;
            case '3':
                $from_id1 = $pcrow->owner_id;
                $from_id2 = $pcrow->user_id;
                $transsql1 = "INSERT INTO `trans` (`id`, `date`, `tg_id`, `describe`, `from_id`, `to_id`, `prop_type`) VALUES (NULL, '$date', '$tg_id', '$describe', '$from_id1', '$to_id', '1')";
                $transres1 = $conn->query($transsql1);
                $transsql2 = "INSERT INTO `trans` (`id`, `date`, `tg_id`, `describe`, `from_id`, `to_id`, `prop_type`) VALUES (NULL, '$date', '$tg_id', '$describe', '$from_id2', '$to_id', '2')";
                $transres2 = $conn->query($transsql2);//在trans表里添加两行数据

                $mainsql = "UPDATE main SET lent_date='$date', user_id='$to_id', owner_id='$to_id' WHERE id='$tg_id'";
                $mainres = $conn->query($mainsql);

                if ($transres1 && $transres2 && $mainres)
                    echo "success";
                else
                    echo "failure";
        }
    }
?>

