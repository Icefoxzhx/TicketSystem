<?php
    header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
    error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息
	if (empty($_COOKIE['PHPSESSID']))
        echo "no";
 	else
 		echo $_COOKIE['username'];
?>
