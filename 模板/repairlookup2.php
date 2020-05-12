<?php
    header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
    error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息
    include 'connect.php';     //调用数据库连接文件
    $method = $_POST['method'];
    $usr_input = $_POST['usr_input'];
    if ($usr_input == "") {     //判断输入是否为空
        echo "<script>alert('输入格式错误');history.back();</script>";
    }
    else{
        switch ($method) {
            case "tg_id":
                $sql = "SELECT * FROM repair WHERE tg_id = '$usr_input' ORDER BY date desc";
                $res = $conn->query($sql);
                $row = $res->fetch_all(MYSQLI_ASSOC);
                if($row == NULL){
                    echo "PC404";
                }
                else{
                    $repair_amt = count($row);
                    for ($i=0; $i < $repair_amt; $i++) { 
                        $row[$i]['repair_id'] = $row[$i]['id'];//增加键，用以避免合并数组后键重叠，并给js传参
                        $pc_id = (int)$row[$i]['tg_id'];
                        $pcsql = "SELECT * FROM main WHERE id = '$pc_id'";
                        $pcres = $conn->query($pcsql);
                        $pcrow = $pcres->fetch_assoc();
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
                    echo json_encode($row);
                }
            break;




            case "usr":
                $name_encoded = base64_encode($usr_input);
                $usersql1 = "SELECT * FROM user WHERE username = '$name_encoded'";
                $userres1 = $conn->query($usersql1);
                $userrow1 = $userres1->fetch_assoc();
                $userid1 = $userrow1['id'];
                $sql = "SELECT * FROM repair WHERE from_id = '$userid1' ORDER BY date desc";
                $res = $conn->query($sql);
                $row = $res->fetch_all(MYSQLI_ASSOC);
                if($row == NULL){
                    echo "PC404";
                }
                else{
                    $repair_amt = count($row);
                    for ($i=0; $i < $repair_amt; $i++) { 
                        $row[$i]['repair_id'] = $row[$i]['id'];//增加键，用以避免合并数组后键重叠，并给js传参
                        $pc_id = (int)$row[$i]['tg_id'];
                        $pcsql = "SELECT * FROM main WHERE id = '$pc_id'";
                        $pcres = $conn->query($pcsql);
                        $pcrow = $pcres->fetch_assoc();
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
                    echo json_encode($row);
                }
            break;




            case "date":
                $sql = "SELECT * FROM repair WHERE date = '$usr_input'";
                $res = $conn->query($sql);
                $row = $res->fetch_all(MYSQLI_ASSOC);
                if($row == NULL){
                    echo "PC404";
                }
                else{
                    $repair_amt = count($row);
                    for ($i=0; $i < $repair_amt; $i++) { 
                        $row[$i]['repair_id'] = $row[$i]['id'];//增加键，用以避免合并数组后键重叠，并给js传参
                        $pc_id = (int)$row[$i]['tg_id'];
                        $pcsql = "SELECT * FROM main WHERE id = '$pc_id'";
                        $pcres = $conn->query($pcsql);
                        $pcrow = $pcres->fetch_assoc();
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
                    echo json_encode($row);
                }
            break;




             case "sn":
                $sql_for_id = "SELECT * FROM main WHERE pc_sn = '$usr_input'";
                $res_for_id=$conn->query($sql_for_id);
                $row_for_id=$res_for_id->fetch_object();
                $tg_id = $row_for_id->id;
                $sql = "SELECT * FROM repair WHERE tg_id = '$tg_id' ORDER BY date desc";
                $res = $conn->query($sql);
                $row = $res->fetch_all(MYSQLI_ASSOC);
                if($row == NULL){
                    echo "PC404";
                }
                else{
                    $repair_amt = count($row);
                    for ($i=0; $i < $repair_amt; $i++) { 
                        $row[$i]['repair_id'] = $row[$i]['id'];//增加键，用以避免合并数组后键重叠，并给js传参
                        $pc_id = (int)$row[$i]['tg_id'];
                        $pcsql = "SELECT * FROM main WHERE id = '$pc_id'";
                        $pcres = $conn->query($pcsql);
                        $pcrow = $pcres->fetch_assoc();
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
                    echo json_encode($row);
                }
            break;
        }
    }
?>