<?php  
$server_ip="127.0.0.1";
$port = 8888;
$sock =@socket_create(AF_INET,SOCK_DGRAM,0);
if(!$sock)
{
    echo "socket create failure";
}
if($buf=="")
    $buf="hello,how are you!\n";
if(!@socket_sendto($sock,$buf,strlen($buf),0,"127.0.0.1",8888))
{
    echo "send error\n";
    socket_close($sock);
    exit();
}
$buf="";
$msg="";
if(!@socket_recvfrom($sock,$msg,256,0,$server_ip,$port))
{
    echo "recvieve error!";
    socket_close($sock);
    exit();
}
echo trim($msg)."\n";
socket_close($sock);
?>

