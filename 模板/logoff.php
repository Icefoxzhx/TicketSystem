<?php
header('Content-type:text/html;charset=utf-8');
session_start();
if(isset($_SESSION['username'])){
    session_unset($_SESSION['username']);
    session_destroy();//销毁一个会话中的全部数据
    setcookie(session_name(),'');//销毁与客户端的联系
    echo "<script>alert('LOGOFFED!');location.href='index.html';</script>";
}else{
}