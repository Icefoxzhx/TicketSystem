<?php
    header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
    error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息
    $username=$_POST['username'];
    $password=$_POST['password'];
    if ($username == "" || $password == "")
    {
		echo "<script>alert('输入格式错误');history.back();</script>";
    }
	else
    {
        $commandd = "add_user -c I_am_the_admin -u $username -p $password -n $username -m example@example.com -g 5";
        $socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
        socket_connect($socket,'123.57.252.230',8888);
        socket_write($socket, strlen($commandd).$commandd);
        $tmp_res = socket_read($socket, 2048);
        $res = substr($tmp_res, 4, substr($tmp_res, 0, 4));
        if ($res == "-1")
            echo "-1";
        else
            echo "0";
    }
?>
