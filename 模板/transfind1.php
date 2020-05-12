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
                $sql = "SELECT * FROM trans WHERE tg_id = '$usr_input'";
                $res=$conn->query($sql);
                $row=$res->fetch_all(MYSQLI_ASSOC);
                if($row == NULL){//id错误返回为空，显示错误信息
                    echo "PC404";
                }
                else{
                    $pc_amt = count($row);
                    for ($i=0; $i < $pc_amt; $i++)
                    { 
                        $from_id = $row[$i]['from_id'];
                        $to_id = $row[$i]['to_id'];
                        $fromsql = "SELECT * FROM user WHERE id = '$from_id'";
                        $fromres = $conn->query($fromsql);
                        $fromrow = $fromres->fetch_object();
                        $from = base64_decode($fromrow->username);//根据id查找from
                        $row[$i]['from_id'] = $from;
                        $tosql = "SELECT * FROM user WHERE id = '$to_id'";
                        $tores = $conn->query($tosql);
                        $torow = $tores->fetch_object();
                        $to = base64_decode($torow->username);//根据id查找to
                        $row[$i]['to_id'] = $to;
                        //以下获取处理deviceprop
                        $tg_id_prop_wanted = $row[$i]['tg_id'];
                     //   echo $tg_id_prop_wanted;
                        $devicepropsql = "SELECT * FROM main WHERE id = '$tg_id_prop_wanted'";
                        $devicepropres = $conn->query($devicepropsql);
                        $deviceproprow = $devicepropres->fetch_object();
                        $row[$i]['pc_cpu'] = $deviceproprow->pc_cpu;
                        $row[$i]['pc_ram'] = $deviceproprow->pc_ram;
                        $row[$i]['pc_hdd'] = $deviceproprow->pc_hdd;
                        $row[$i]['pc_brand'] = $deviceproprow->pc_brand;
                        $row[$i]['pc_model'] = $deviceproprow->pc_model;
                        $row[$i]['pc_gpu'] = $deviceproprow->pc_gpu;
                        $row[$i]['pc_sn'] = $deviceproprow->pc_sn;
                    }
                    echo json_encode($row);
                }
            break;




            case "date":
                $sql = "SELECT * FROM trans WHERE date = '$usr_input'";
                $res=$conn->query($sql);
                $row=$res->fetch_all(MYSQLI_ASSOC);
                if($row == NULL){//id错误返回为空，显示错误信息
                    echo "PC404";
                }
                else{
                    $pc_amt = count($row);
                    for ($i=0; $i < $pc_amt; $i++)
                    { 
                        $from_id = $row[$i]['from_id'];
                        $to_id = $row[$i]['to_id'];
                        $fromsql = "SELECT * FROM user WHERE id = '$from_id'";
                        $fromres = $conn->query($fromsql);
                        $fromrow = $fromres->fetch_object();
                        $from = base64_decode($fromrow->username);//根据id查找from
                        $row[$i]['from_id'] = $from;
                        $tosql = "SELECT * FROM user WHERE id = '$to_id'";
                        $tores = $conn->query($tosql);
                        $torow = $tores->fetch_object();
                        $to = base64_decode($torow->username);//根据id查找to
                        $row[$i]['to_id'] = $to;
                        //以下获取处理deviceprop
                        $tg_id_prop_wanted = $row[$i]['tg_id'];
                     //   echo $tg_id_prop_wanted;
                        $devicepropsql = "SELECT * FROM main WHERE id = '$tg_id_prop_wanted'";
                        $devicepropres = $conn->query($devicepropsql);
                        $deviceproprow = $devicepropres->fetch_object();
                        $row[$i]['pc_cpu'] = $deviceproprow->pc_cpu;
                        $row[$i]['pc_ram'] = $deviceproprow->pc_ram;
                        $row[$i]['pc_hdd'] = $deviceproprow->pc_hdd;
                        $row[$i]['pc_brand'] = $deviceproprow->pc_brand;
                        $row[$i]['pc_model'] = $deviceproprow->pc_model;
                        $row[$i]['pc_gpu'] = $deviceproprow->pc_gpu;
                        $row[$i]['pc_sn'] = $deviceproprow->pc_sn;
                    }
                    echo json_encode($row);
            }
            break;




            case "usr":
                $name_encoded = base64_encode($usr_input);//查找次姓名对应的id
                $idsql = "SELECT * FROM user WHERE username = '$name_encoded'";
                $idres = $conn->query($idsql);
                $idrow = $idres->fetch_object();
                $query_user_id = $idrow->id;
                $sql = "SELECT * FROM trans WHERE from_id = '$query_user_id' OR to_id = '$query_user_id'";
                $res=$conn->query($sql);
                $row=$res->fetch_all(MYSQLI_ASSOC);
                if($row == NULL){//id错误返回为空，显示错误信息
                    echo "PC404";
                }
                else{
                    $pc_amt = count($row);
                    for ($i=0; $i < $pc_amt; $i++)
                    { 
                        $from_id = $row[$i]['from_id'];
                        $to_id = $row[$i]['to_id'];
                        $fromsql = "SELECT * FROM user WHERE id = '$from_id'";
                        $fromres = $conn->query($fromsql);
                        $fromrow = $fromres->fetch_object();
                        $from = base64_decode($fromrow->username);//根据id查找from
                        $row[$i]['from_id'] = $from;
                        $tosql = "SELECT * FROM user WHERE id = '$to_id'";
                        $tores = $conn->query($tosql);
                        $torow = $tores->fetch_object();
                        $to = base64_decode($torow->username);//根据id查找to
                        $row[$i]['to_id'] = $to;
                        //以下获取处理deviceprop
                        $tg_id_prop_wanted = $row[$i]['tg_id'];
                     //   echo $tg_id_prop_wanted;
                        $devicepropsql = "SELECT * FROM main WHERE id = '$tg_id_prop_wanted'";
                        $devicepropres = $conn->query($devicepropsql);
                        $deviceproprow = $devicepropres->fetch_object();
                        $row[$i]['pc_cpu'] = $deviceproprow->pc_cpu;
                        $row[$i]['pc_ram'] = $deviceproprow->pc_ram;
                        $row[$i]['pc_hdd'] = $deviceproprow->pc_hdd;
                        $row[$i]['pc_brand'] = $deviceproprow->pc_brand;
                        $row[$i]['pc_model'] = $deviceproprow->pc_model;
                        $row[$i]['pc_gpu'] = $deviceproprow->pc_gpu;
                        $row[$i]['pc_sn'] = $deviceproprow->pc_sn;
                    }
                    echo json_encode($row);
            }
            break;




             case "sn":
                $sql_for_id = "SELECT * FROM main WHERE pc_sn = '$usr_input'";
                $res_for_id=$conn->query($sql_for_id);
                $row_for_id=$res_for_id->fetch_object();
                $tg_id = $row_for_id->id;
                $sql = "SELECT * FROM trans WHERE tg_id = '$tg_id'";
                $res=$conn->query($sql);
                $row=$res->fetch_all(MYSQLI_ASSOC);
                if($row == NULL){//id错误返回为空，显示错误信息
                    echo "PC404";
                }
                else{
                    $pc_amt = count($row);
                    for ($i=0; $i < $pc_amt; $i++)
                    { 
                        $from_id = $row[$i]['from_id'];
                        $to_id = $row[$i]['to_id'];
                        $fromsql = "SELECT * FROM user WHERE id = '$from_id'";
                        $fromres = $conn->query($fromsql);
                        $fromrow = $fromres->fetch_object();
                        $from = base64_decode($fromrow->username);//根据id查找from
                        $row[$i]['from_id'] = $from;
                        $tosql = "SELECT * FROM user WHERE id = '$to_id'";
                        $tores = $conn->query($tosql);
                        $torow = $tores->fetch_object();
                        $to = base64_decode($torow->username);//根据id查找to
                        $row[$i]['to_id'] = $to;
                        //以下获取处理deviceprop
                        $tg_id_prop_wanted = $row[$i]['tg_id'];
                     //   echo $tg_id_prop_wanted;
                        $devicepropsql = "SELECT * FROM main WHERE id = '$tg_id_prop_wanted'";
                        $devicepropres = $conn->query($devicepropsql);
                        $deviceproprow = $devicepropres->fetch_object();
                        $row[$i]['pc_cpu'] = $deviceproprow->pc_cpu;
                        $row[$i]['pc_ram'] = $deviceproprow->pc_ram;
                        $row[$i]['pc_hdd'] = $deviceproprow->pc_hdd;
                        $row[$i]['pc_brand'] = $deviceproprow->pc_brand;
                        $row[$i]['pc_model'] = $deviceproprow->pc_model;
                        $row[$i]['pc_gpu'] = $deviceproprow->pc_gpu;
                        $row[$i]['pc_sn'] = $deviceproprow->pc_sn;
                    }
                    echo json_encode($row);
            }
            break;
        }
    }
?>