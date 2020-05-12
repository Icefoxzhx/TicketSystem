<?php
    header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
    error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息
    include 'connect.php';     //调用数据库连接文件
    session_start();
    $username = $_SESSION['username'];
    $name_encoded = base64_encode($username);//查找次姓名对应的id
    $idsql = "SELECT * FROM user WHERE username = '$name_encoded'";
    $idres = $conn->query($idsql);
    $idrow = $idres->fetch_object();
    $query_user_id = $idrow->id;
    $sql = "SELECT * FROM main WHERE user_id = '$query_user_id' OR owner_id = '$query_user_id'";
    $res = $conn->query($sql);
    $row = $res->fetch_all(MYSQLI_ASSOC);
    if($row == NULL){
        echo "PC404";
    }
    else{
        $pc_amt = count($row);
        for ($i=0; $i < $pc_amt; $i++) { 
            $owner_id = $row[$i]['owner_id'];
            $user_id = $row[$i]['user_id'];
            $ownersql = "SELECT * FROM user WHERE id = '$owner_id'";
            $ownerres = $conn->query($ownersql);
            $ownerrow = $ownerres->fetch_object();
            $owner = base64_decode($ownerrow->username);//根据id查找owner
            $usersql = "SELECT * FROM user WHERE id = '$user_id'";
            $userres = $conn->query($usersql);
            $userrow = $userres->fetch_object();
            $user = base64_decode($userrow->username);//根据id查找owner
            $row[$i]['owner_id'] = $owner;
            $row[$i]['user_id'] = $user;
        }
        echo json_encode($row);
    }
?>