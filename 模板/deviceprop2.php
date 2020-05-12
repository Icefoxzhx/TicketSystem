<?php
    header("content-type:text/html;charset=utf-8");  //设置页面内容是html  编码是utf-8
    error_reporting(E_ALL &~ E_NOTICE);     //屏蔽错误信息
    include 'connect.php';     //调用数据库连接文件
    $method = $_POST['method'];
    $usr_input = $_POST['usr_input'];//接收前台post值
    if ($usr_input == "") {     //判断输入是否为空
        echo "<script>alert('输入格式错误');history.back();</script>";
    }
    else{
        switch ($method) {
            case "id"://用户按照电脑流水号，即数据库中main表的id查询
                $sql = "SELECT * FROM main WHERE id = '$usr_input'";
                $res=$conn->query($sql);
                $row=$res->fetch_all(MYSQLI_ASSOC);//在main表中读取该id的电脑信息
                // var_dump($row);
                if($row == NULL){//id错误返回为空，显示错误信息
                    echo "PC404";
                }
                else{
                    $owner_id = $row[0]['owner_id'];
                    $user_id = $row[0]['user_id'];

                    $ownersql = "SELECT * FROM user WHERE id = '$owner_id'";
                    $ownerres = $conn->query($ownersql);
                    $ownerrow = $ownerres->fetch_object();
                    $owner = base64_decode($ownerrow->username);//根据id查找owner
                    $row[0]['owner_id'] = $owner;

                    $usersql = "SELECT * FROM user WHERE id = '$user_id'";
                    $userres = $conn->query($usersql);
                    $userrow = $userres->fetch_object();
                    $user = base64_decode($userrow->username);//根据id查找owner
                    $row[0]['user_id'] = $user;

                    echo json_encode($row);
                }
                break;

            case "usr"://用户输入姓名
                $name_encoded = base64_encode($usr_input);//查找次姓名对应的id
                $idsql = "SELECT * FROM user WHERE username = '$name_encoded'";
                $idres = $conn->query($idsql);
                $idrow = $idres->fetch_object();
                $query_user_id = $idrow->id;
                $sql = "SELECT * FROM main WHERE user_id = '$query_user_id'";
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
                    // print("<pre>"); // 格式化输出数组
                    // print_r($row);
                    // print("</pre>"); 
                }
                break;

            case "owner"://用户输入姓名
                $name_encoded = base64_encode($usr_input);//查找次姓名对应的id
                $idsql = "SELECT * FROM user WHERE username = '$name_encoded'";
                $idres = $conn->query($idsql);
                $idrow = $idres->fetch_object();
                $query_user_id = $idrow->id;
                $sql = "SELECT * FROM main WHERE owner_id = '$query_user_id'";
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
                    // print("<pre>"); // 格式化输出数组
                    // print_r($row);
                    // print("</pre>"); 
                }
                break;

            case "date":
                $sql = "SELECT * FROM main WHERE buy_date = '$usr_input'";
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
                break;

            case "brand":
                $input_model = $_POST['input_model'];
                if($input_model == '')
                    $sql = "SELECT * FROM main WHERE pc_brand = '$usr_input'";
                else
                    $sql = "SELECT * FROM main WHERE pc_brand = '$usr_input' AND pc_model = '$input_model'";
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
                break;

            case "sn":
                $sql = "SELECT * FROM main WHERE pc_sn = '$usr_input'";
                $res=$conn->query($sql);
                $row=$res->fetch_all(MYSQLI_ASSOC);//在main表中读取该id的电脑信息
                // var_dump($row);
                if($row == NULL){//id错误返回为空，显示错误信息
                    echo "PC404";
                }
                else{
                    $owner_id = $row[0]['owner_id'];
                    $user_id = $row[0]['user_id'];

                    $ownersql = "SELECT * FROM user WHERE id = '$owner_id'";
                    $ownerres = $conn->query($ownersql);
                    $ownerrow = $ownerres->fetch_object();
                    $owner = base64_decode($ownerrow->username);//根据id查找owner
                    $row[0]['owner_id'] = $owner;

                    $usersql = "SELECT * FROM user WHERE id = '$user_id'";
                    $userres = $conn->query($usersql);
                    $userrow = $userres->fetch_object();
                    $user = base64_decode($userrow->username);//根据id查找owner
                    $row[0]['user_id'] = $user;

                    echo json_encode($row);
                }
                break;
        }
    }
?>