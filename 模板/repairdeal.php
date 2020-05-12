<?php
    header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
    error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息

    $id = $_POST['repair_id'];    //main表中电脑id
        echo "success";
?>