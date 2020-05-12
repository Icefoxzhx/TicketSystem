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

    $sql = "SELECT * FROM repair WHERE from_id = '$query_user_id' ORDER BY date desc";
    $res = $conn->query($sql);
    $row = $res->fetch_all(MYSQLI_ASSOC);
    if($row == NULL){
        echo "PC404";
    }
    else{
        $repair_amt = count($row);
        for ($i=0; $i < $repair_amt; $i++) { 
            $pc_id = (int)$row[$i]['tg_id'];
            $pcsql = "SELECT * FROM main WHERE id = '$pc_id'";
            $pcres = $conn->query($pcsql);
            $pcrow = $pcres->fetch_assoc();
            // $pc_row_sim = $pcrow['0']; //给拿回来的数组扒皮【可重构，上一行用fetch_assoc(),可删除这行
            $row[$i] = $row[$i]+$pcrow;
            if ($row[$i]['repair_status']) {
                $row[$i]['repair_status'] = "是";
            }
            else {
                $row[$i]['repair_status'] = "否";
            }
            $userid = $pcrow['user_id'];       //翻译电脑详细信息中使用者姓名
            $usersql = "SELECT username FROM user WHERE id ='$userid'";
            $userres = $conn->query($usersql);
            $userrow = $userres->fetch_object();
            $row[$i]['user_id'] = base64_decode($userrow->username);

            $ownerid = $pcrow['owner_id'];       //翻译电脑详细信息中拥有者姓名
            $ownersql = "SELECT username FROM user WHERE id ='$ownerid'";
            $ownerres = $conn->query($ownersql);
            $ownerrow = $ownerres->fetch_object();
            $row[$i]['owner_id'] = base64_decode($ownerrow->username);
        }
        // print_r($row);
        echo json_encode($row);
    }
?>