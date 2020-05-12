<?php
    header("content-type:text/html;charset=utf-8");
    error_reporting(E_ALL &~ E_NOTICE); 
    $num1 = $_POST['num1'];
    $num2 = $_POST['num2'];
    $command= "test.exe $num1 $num2";
    $res = passthru($command);
    echo $res;
?>