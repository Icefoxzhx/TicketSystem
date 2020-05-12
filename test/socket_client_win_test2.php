<?php  

$socket = socket_create(AF_INET,SOCK_STREAM,SOL_TCP);
socket_connect($socket,'127.0.0.1',8888);
$msg = 'helloserver';
socket_write($socket,$msg);

?>

